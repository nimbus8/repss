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

#include "lib/includes/execution_phase/grammaticalForm/GrammarConfig.hpp"
#include "lib/includes/execution_phase/grammaticalForm/GrammarKeywordDefn.hpp"

#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif


GrammarConfig::GrammarConfig()
{
    //Defining GrammarKeywords -- this is not done here anymore: DELETE

    const std::string keywordName{"general_end"};

    AbstrKeyword keyword{ keywordName, GrammarType_t::VARIABLE};
    grammarKeywords.add(GrammarKeywordDefn{ &keyword });
}


#undef DEBUG
#undef DeLOG
