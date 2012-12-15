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

#include "base/includes/execution_phase/grammaticalForm/GrammarKeywords.hpp"

/*#undef*/ #define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str)
#else
    #define DeLOG(str)
#endif

namespace std
{

bool testGrammarKeywords()
{
    size_t NUMBER_OF_KEYWORDS = 100;
    GrammarKeywords v;

    AbstrKeyword* keywords[NUMBER_OF_KEYWORDS];
    for (size_t index = 0; index < NUMBER_OF_KEYWORDS; index++)
    {
        keywords[index] = new AbstrKeyword(std::to_string(index), (index % 2 == 0? GrammarType_t::VARIABLE : GrammarType_t::TERMINAL));
    }

    for ( int i = 0; i < NUMBER_OF_KEYWORDS; i++ )
    {
        v.set( i , GrammarKeywordDefn{ keywords[i] });
    }

    size_t counted = 0;
    for ( GrammarKeywordDefn i : v )
    {
        std::string grammarKeywordName = i.getName();
        if (grammarKeywordName.compare(std::to_string(counted)) != 0)
        {
            //cleanup
            for (size_t index = 0; index < NUMBER_OF_KEYWORDS; index++)
            {
                delete keywords[index];
            }

            perror("Grammar Keyword Test FAILED!");
            return false;
        }

        std::cout << i << std::endl;

        counted++;
    }

    for (size_t index = 0; index < NUMBER_OF_KEYWORDS; index++)
    {
        delete keywords[index];
    }

    return (counted == NUMBER_OF_KEYWORDS);
}

}

#undef DEBUG
#undef DeLOG
