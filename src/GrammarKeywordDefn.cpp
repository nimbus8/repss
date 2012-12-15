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

#include <iostream>

#include "base/includes/execution_phase/grammaticalForm/GrammarKeywordDefn.hpp"

#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

std::ostream &operator<<( std::ostream &out, const GrammarKeywordDefn &GKD )
{
    GrammarType_t gkdType = GKD.getGrammarType();
    out << "GrammarKeywordDef{ name: ";

    out << GKD.getName() << " ";

    out << "type: ";

    if (gkdType == GrammarType_t::TERMINAL)
    {
        out << "Terminal";
    }
    else if (gkdType == GrammarType_t::VARIABLE)
    {
        out << "Variable";
    }
    else
    {
        out << "Unrecognized";
    }

    out << " }";

    return out;
}

#undef DEBUG
#undef DeLOG
