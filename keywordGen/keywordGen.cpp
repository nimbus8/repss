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
 */

//read in keywords-list.defn
//+ file should be organized, by line:
//                                                                
//    + IKeywords:obj {                
//         +_keyword-name_{ type, ... }                
//         +path { '../src/lib/includes/GeneratedIRecognizedKeywords.hpp' }          
//    }                                                 
//
//           or
//           --
//
// + RecognizedKeywordsFile:file {
//     + IKeywords:obj {
//         +_keyword-name_ { type, ... }
//     }
//
//     +path { '../src/lib/includes/GeneratedIRecognizedKeywords.hpp' }
// }
//+  'keyword-name' alone is sufficient for Lexer, the details implemented by language writer
//+  'type' is relevant to Grammar module (the 'keyword-name' is too).

//goes into filesystem of repss and replaces certain files/interfaces with new ones

/*
    Template for generated file:


        _gen_recognizedKeywords.hpp
        or autogenerated/recognizedKeywords.hpp

            class IKeywords
            {
            private:
                virtual ~IKeywords() = 0;
            protected:
                class KeywordsData
                {
                private:
                    const size_t _size = 12;
                    const AbstrKeywordDefn keywords[_size] =
                        /\
                        ||
*/
                 /* AbstrKeywordDefn is abstract w/ instance fields defined
                     AbstrKeywordDefn inherits from interface IKeywordDefn,
                       LexerKeywordDefn and GrammarKeywordDefn should also inherit
                       from IKeywordDefn. */
/*
                        { 
                          AbstrKeywordDefn{ "scope", GrammarType_t::VARIABLE },
                          AbstrKeywordDefn{ "general_end", GrammartType_t::TERMINAL }
                        }
                public:
                    KeywordsData()
                    {

                    }

                    size_t getSize() { return _size; }
                    IGrammarKeywordDefn getAt(size_t index)
                    {
                        ....
                    }     
                };

                const KeywordsData _data;
            public:
                template<T>
                //this way both lexer and grammar always have access to same set of keywords
                //and can restrict usage of uneeded parts in the set etc -- to be taken care.
                //in subclasses LexerKeywords, GrammarKeywords 
                virtual const std::vector<T> getKeywords();
            };
*/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:  %s\n", __FILE__, __LINE__, str);
    #define DeNEWLINE() printf("\n");
#else
    #define DeLOG(str)
    #define DeNEWLINE()
#endif

enum class ModeInfo_t : unsigned int
{
    EMPTY                         = 0x000,
    ERROR                         = 0xfff,

    READ_DONE                     = 0x002,
    WRITE_DONE                    = 0x004,

    SAVED_READ_DATA_TO_TEMP_FILE  = 0x008

    // exlusive hexadecimal counting: 0x0008, 0x0020, 0x0040, 0x0080, 0x0200, etc [1]
};

class Status
{
private:
    unsigned int _modeInfo;
public:
    Status() : _modeInfo(static_cast<unsigned int>(ModeInfo_t::EMPTY)) {}
    explicit Status(const ModeInfo_t& singleModeInfo) 
        : _modeInfo(static_cast<unsigned int>(singleModeInfo)) {}
    explicit Status(const unsigned int& compoundModeInfo) : _modeInfo(compoundModeInfo) {}

    Status(const Status& other) : _modeInfo(other._modeInfo) {}
    ~Status() {}

    void addModeInfo(const ModeInfo_t info); 

    bool isEmpty() const { return (_modeInfo & static_cast<unsigned int>(ModeInfo_t::EMPTY)); }
    bool isError() const { return (_modeInfo & static_cast<unsigned int>(ModeInfo_t::ERROR)); }
};

void Status::addModeInfo(const ModeInfo_t infoToAdd)
{
    unsigned int modeInfoToAddUINT = static_cast<unsigned int>(infoToAdd);
    if (!(_modeInfo & modeInfoToAddUINT))
    {
        _modeInfo ^= modeInfoToAddUINT;

        if (!(_modeInfo & modeInfoToAddUINT))
        {
            std::cout << "add mode info failed!!! oops!" << std::endl;
            exit(1);
        }
    }
}

bool confirmFirstNonWhiteSpaceInBuffer(char *buffer, size_t len, char character)
{
    for (size_t i = 0;  i < len; i++)
    {
        char val = *(buffer + i);
        if (val != ' ' &&  val != character)
        {
            return false;
        }
        else if (val == character)
        {
            return true;
        }
    }

    return false;
}

void clearArray(char *buffer, size_t size)
{
    memset(buffer, 0, size);
}


typedef std::vector<std::tuple<std::string,std::string,std::pair<std::string,std::string>>> ObjectDataVector_t;

Status
readInformation(ObjectDataVector_t& objects, FILE* const file)
{
    const size_t MAX_LINE_LENGTH = 1028;

    if (file != NULL)
    {
        typedef enum
        {
             LOOKING_FOR_OBJECT,
             LOOKING_FOR_LEFT_CURLY_BRACKET,
             LOOKING_FOR_OBJECT_DETAILS,
             LOOKING_FOR_RIGHT_CURLY_BRACKET
        } AttentionState_t;

        AttentionState_t currentAttentionState = LOOKING_FOR_OBJECT;
        char buffer[MAX_LINE_LENGTH];

        std::string                                     objId;
        std::string                                     objType;
        std::vector<std::pair<std::string,std::string>> objDetails;

        while (!feof(file))
        {
            fgets (buffer , MAX_LINE_LENGTH, file);

            size_t len = strlen(buffer);
            if(buffer[len-1] == '\n')
            {
                buffer[len-1] = 0;
            }

            //tokenize line based on 'attention state'
            switch (currentAttentionState)
            {
                case LOOKING_FOR_OBJECT:
                {
                    char* token;

                    DeLOG(
                        std::string{"Splittng string '"}.append(buffer).append("' into tokens.").c_str()
                    );
                    token = strtok(buffer, ":");
                    if (token == NULL)
                    {
                        //insert temp object cleanup...here
                        break;
                    }
                    std::string identifierName(token);
                    objId = identifierName;
                    DeLOG(
                        std::string{"Identifier string: '"}.append(identifierName).append("' into tokens.").c_str()
                    );


                    token = strtok(NULL, " ");
                    if (token == NULL)
                    {
                        //insert temp object cleanup...here
                        break;
                    }
                    std::string objectType(token);
                    objType = objectType;
                    DeLOG(
                        std::string{"Object-type string: '"}.append(token).append("'\n").c_str()
                    );


                    token = strtok(NULL, " ");
                    if (token == NULL)
                    {
                        //insert temp object cleanup...here
                        break;
                    }
                    if (*token == '{')
                    {
                        currentAttentionState = LOOKING_FOR_OBJECT_DETAILS;
                    }
                    else
                    {
                        currentAttentionState = LOOKING_FOR_LEFT_CURLY_BRACKET;
                    }
                    break;
                }
                case LOOKING_FOR_LEFT_CURLY_BRACKET:
                {
                    DeLOG("case: looking for left curly brace\n");

                    bool isLeftCurlyBraceFirstNonWhiteSpace =
                        confirmFirstNonWhiteSpaceInBuffer(buffer, strlen(buffer), '{');

                    if (isLeftCurlyBraceFirstNonWhiteSpace)
                    {
                        currentAttentionState = LOOKING_FOR_OBJECT_DETAILS;
                    }
                    else
                    {
                        //insert temp object cleanup...here
                        currentAttentionState = LOOKING_FOR_OBJECT;
                    }

                    break;
                }
                case LOOKING_FOR_OBJECT_DETAILS:
                {
                    DeLOG("case: looking for object details");

                    char* token;
          
                    //first look for right curly brace as first non whitespace
                    bool isRightCurlyBraceFirstNonWhiteSpace =
                        confirmFirstNonWhiteSpaceInBuffer(buffer, strlen(buffer), '}');
                    if (isRightCurlyBraceFirstNonWhiteSpace)
                    {
                        //make recorded objects into a tuple, add it to vector
                        //then cleanup recorded temporary objects
                        //...

                        currentAttentionState = LOOKING_FOR_OBJECT_DETAILS;
                        break;
                    }

                    //if at any place we encounter something unexpected erase, temp objects and 
                    //change attentionState to LOOKING_FOR_OBJECT (reset)
                    DeLOG(
                        std::string{"Splittng string '"}.append(buffer).append("' into tokens.").c_str()
                    );
                    token = strtok(buffer, "+");
                    if (token == NULL)
                    {
                        //insert temp object cleanup...here
                        DeNEWLINE(); 
                        break;
                    }
                    token = strtok(NULL, "{");
                    if (token == NULL)
                    {
                        //insert temp object cleanup...here
                        DeNEWLINE();
                        break;
                    }
                    std::string keywordName(token);
                    DeLOG(
                        std::string{"KeywordName string: '"}.append(keywordName).append("'.").c_str()
                    );
                    token = strtok(NULL, " ");
                    if (token == NULL)
                    {
                        //insert temp object cleanup...here
                        DeNEWLINE();
                        break;
                    }
                    std::string keywordGrammarType(token);
                    DeLOG(
                        std::string{"KeywordGrammarType string: '"}.append(keywordGrammarType).append("'.\n").c_str()
                    );


                    objDetails.push_back(std::make_pair(keywordName, keywordGrammarType));                    
                    break;
                }
                default:
                {
                    DeLOG("case: default\n");
                    break;
                }
            };

            clearArray(buffer, MAX_LINE_LENGTH);
        }

        DeLOG(std::string("Obj-Id:\t").append(objId).c_str());
        DeLOG(std::string("Obj-Type:\t").append(objType).c_str());
        for (auto details : objDetails)
        {
            DeLOG(
                std::string("  Obj-Detail:  ").append(details.first).append("=").append(details.second).c_str()
            );
        }
    }
    else
    {
        perror ("Error opening file");
    }
    return Status{ ModeInfo_t::READ_DONE };
}

Status writeGeneratedContent(const ObjectDataVector_t& objects, const Status& prevStatus)
{
    Status currentStatus{prevStatus};

    return currentStatus;   
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        DeLOG("Usage: ./keywordGen [input-filename.ext] [output-filename.ext]"); 
    }

    ObjectDataVector_t objects;
    FILE* defnFile = fopen(argv[1], "rt");

    DeLOG("About to read file\n");
    const Status readStatus = readInformation(objects, defnFile);

    fclose(defnFile);

    const Status writeStatus = writeGeneratedContent(objects, readStatus);

    return 0;
}

#undef DeLOG
#undef DeNEWLINE

/*
 + Footnote:

     + [1] : Exclusive Hexadecimal Counting
              - made up phrasing, meaning the
                way we count in hexadecimal so that,
                by assigning each number to a enumeration value,
                we can use the logical AND ('&') in combination 
                with the Exclusive OR operation (XOR, '^' in c) to let one
                variable represent a great number of combinations of options
                and states defined in a given enumeration.

                   eg:
                       Thursday=0x0, Friday=0x2, Saturday=0x4, RecognizedNonPartyDay=0x8
                       Partying=0x20, Studying=0x40, DrunkAt2am=0x80

                       This is a particularly terrible example, because, for example we're
                       insuating that it can be Thursday, Friday, AND Saturday all at the same
                       time. Also, Friday may be one of those 'Recognized Non Party Days',
                       which is of course a false reaility. And I've personally never partied and
                       studyed in the same day (we all know that ONE guy, but of course he
                       lies). College students in particular are exempt from the whole
                       RecognizedNonPartyDay concept, alot of people don't like that -
                       especially the ones who missed out.

                       In all seriousnesss,
                       here, correctness isn't as important as is the representation. Ensuring
                       all interesting possibilies are defined and properly enumerated.
                       Maybe we SHOULD enforce correctness here. But my only oppsition to that
                       is that it would require much more thought and definately a bit more
                       work. Furthermore, there's no reason that the rules to restrict this 'system'
                       could not be done at a higher level, where these representations are
                       a given and from that hugher level are unchangeable.

                           Whats particularly good about this, is that we can accurately describe
                           my current situation. It let's me state, correctly, that it's
                           Friday=0x2, I'm Studying=0x40 and I'm NOT partying (which lucky
                           for us, just kindof goes without saying).
                                To represent this as a number stored in 
                                hexadecimal (re:computers) format:

                                    Friday        0x02
                                 +  Studying      0x40
                                 ---------------------
                                    Lame          0x42

                                And it's that simple.
                                 => Notice that leading zeros mean nothing; its to help out the
                                    poor soul who doesn't find hexidecimal intuitive (i.e. me).


                           And just for laugh's (plus some extra education)
                           ...
                           It's  a bit too early (1:00am) to say whether I'll be
                           drunk at 2:00am. But say I am, such a state can be represented
                           by: Friday=0x2, Studying=0x40, DrunkAt2am=0x80
                               To represent this as a number stored in hexadecimal:

                                    Friday        0x02
                                    Studying      0x40
                                 +  DrunkAt2am    0x80
                                 ---------------------
                                    Less Lame     0xc2

                               This may look foreign, but it's no coincidence the 2 at the end
                               is present both here and in our previous example. It's because
                               each digit in a hexadecimal number is like a column, and
                               additions take place inside the columns of those stacked
                               hexadecimal numbers. Your likely familiar with ths. It's not
                               new, it's the same as with the more common decimal represention. 
                               But here, (1) instead of 10 being the limit that can never be 
                               reached it's 16. Oh and (2)because we counted so nicely and don't 
                               permit multiple additions of the same number(name), we needn't 
                               worry about overflow. So technically we can add multiple
                               add hexadecimal numbers, specifically  the contents
                               of each column in any order we please and the end result will be
                               the same. And so here in this example:
                                  4 + 8 = 12 => c          | a=10, b=11, c=12, ..., f=15.  
                               And so well, what if we were Partying=0x20, Studying=0x40, 
                               and DrunkAt2am=0x80? (The infamous Trifecta) Partying btw 
                               involving at least 3 other people than yourself. Well then we'd 
                               have:
                                   2 + 4 + 8 = 14 => e
                               Which given our scheme, is just about the only way 14(e) will
                               show up. To recap, we have a set of options that can be combined 
                               into a single number, then later picked apart knowing only the 
                               domain of possible options available - this schema works, and it
                               isn't terrible.
*/
