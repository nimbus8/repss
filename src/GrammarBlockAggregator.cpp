/*
 REPSS
 Copyright (C) 2012  Khalique Williams

 This file is part of REPSS.

 REPSS is free software: you can redistribute it and/or modify
 it under the terms of the   GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 REPSS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with REPSS.  If not, see <http://www.gnu.org/licenses/>.

 If you wish to contact the author of REPSS, you may do so at
 kwillia.pub@gmail.com
 */

#include <stdio.h>
#include <string>

#include "base/includes/execution_phase/grammaticalForm/GrammarBlockAggregator.hpp"

#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

//this is where the main processing of annotate data occurs

std::pair<std::string, std::string> getKeywordNameAndContentFromAnnotatedData(const std::string annotatedData);

void GrammarBlockAggregator::processAnnotatedData(IGrammarContext* const context)
{
    DeLOG("Beggining processing of annotated data from Lexer\n");
    DeLOG("-------------------------------------------------\n");
    auto annotatedData = context->getAnnotatedData();

    size_t sizeOfAnnotatedData = annotatedData.size();

    for (size_t index = 0; index < sizeOfAnnotatedData; index++)
    {
        DeLOG(std::string{"Processing annotatedData #"}.append(std::to_string(index)).append("\n").c_str());
        auto dataString = annotatedData.getAt(index);

        //parse string for type
        // - it can be either a keyword or data
        //    data can be recognized as just data
        //    anything else is considered a keyword.
        //      -- currently returns "invalid" for first in std::pair if malformed
        auto keywordNameAndContent = getKeywordNameAndContentFromAnnotatedData(dataString);

        DeLOG(std::string{keywordNameAndContent.first}.append(", ").append(keywordNameAndContent.second).append("\n").c_str());
        //if data push it to the current Block.

        //if keyword, analyze it.
        // 'analysis' consists of, is this expected? if not ignore it.
        //   Also if it's a Variable we add a new Block as the newest child
        //   to this current line of block.
        //   If it's a variable then we have to be on the lookout for a
        //   corresponding 'closure' to end the current Block.
        //    So we must make note of this somehow.
        //   Everything else (terminals) are added to the current block
        //    or line(if not within a block). The system as a whole can be
        //    viewed as a block I guess.

        if (keywordNameAndContent.first.compare("data_line") != 0)
        {

        }
        else
        {
            //theres not much here to do, just add it to whatever the current active grammar block is
        }

        //put gramarDataBlock into lexerGrammarProxy 
    }

    auto grammarDataProxy = context->getGrammarDataProxy();
    context->setGrammarBlockAggregate(static_cast<GrammarBlockAggregate&&>(_aggregatedBlocks));
}

std::pair<std::string, std::string> getKeywordNameAndContentFromAnnotatedData(const std::string annotatedData)
{
    char buffer[annotatedData.size()];
    strcpy(buffer, annotatedData.c_str());

    char *token = NULL;

    //begin tokenizing
    token = strtok(buffer, ":");

    if (token == NULL)
    {
        DeLOG("Error: Ooops. It looks like the grammar module is expecting a syntax different from the annotated output of lexer\n");
        return std::make_pair("invalid", "");
    }

    std::string content;

    std::string endOfContentDelimeter;

    if (strlen(token) == 1)
    {
        DeLOG("We shouldn't have empty string keyword names.\n");
        return std::make_pair("invalid", "");
    }
    
    std::string keywordName(token+1);
    if (keywordName.compare("data_line") == 0)
    {
        DeLOG("\tIdentified data_line\n");

        endOfContentDelimeter = "}";
    }
    else if (keywordName.compare("key_word") == 0)
    {
        DeLOG("\tIdentified keyword\n");
        token = strtok(NULL, "=");

        if (token == NULL)
        {
            DeLOG("Error: Ooops. It looks like the grammar module is expecting a syntax different from the annotated output of lexer\n");
            return std::make_pair("invalid", "");
        }

        endOfContentDelimeter = ">";
    }

    std::string lastToken;
    std::string secondLastToken;

    token = strtok(NULL, endOfContentDelimeter.c_str());

    //check to see if there are anymore '}'s. Because we allow '{'
    //within the data part, this needs to be covered
    if (token == NULL)
    {
        DeLOG("Error: Ooops. It looks like the grammar module is expecting a syntax different from the annotated output of lexer");
        return std::make_pair("invalid", "");
    }

    if (keywordName.compare("key_word") == 0 && strlen(token) > 1)
    {
        //This takes care of leading '<' imediately after the = sign in annotated data.
        token = (token+1);
    }

    size_t count = 0;
    do
    {
        content.append(secondLastToken);
        if (keywordName.compare("key_word") == 0 && count > 1 && strcmp("}",token) != 0)
        {
            content.append(endOfContentDelimeter);
        }

        secondLastToken = lastToken;
        lastToken = std::string(token);

        token = strtok(NULL, endOfContentDelimeter.c_str());

        count++;
    } while(token != NULL); 

    if (lastToken.compare("}") == 0)
    {
        if (keywordName.compare("key_word") == 0 && count > 1)
        {
            content.append(endOfContentDelimeter);
        }
    }

    content.append(secondLastToken);

    //the last Token should be the '}'
    if (secondLastToken.empty())
    {
        content.append(lastToken);
    }
    else if (lastToken.compare("}") != 0)
    {
        DeLOG("Warning: The last character in annotated data is expected to be '}'. It appears not to be (grammar module), you should deal wth this.\n");
    }

    return std::make_pair(keywordName, content);
}

