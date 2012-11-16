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

#ifndef _GRAMMAR_KEYWORD_DEFN_
#define _GRAMMAR_KEYWORD_DEFN_

#include <iostream>

enum class GrammarType_t : char
{
    UNDEFINED  = 'U',
    TERMINAL   = 'T',
    VARIABLE   = 'V',
};

class GrammarKeywordDefn
{
private:
    GrammarType_t _grammarType;
public:
    GrammarKeywordDefn() : _grammarType(GrammarType_t::UNDEFINED) {}
    explicit GrammarKeywordDefn(GrammarType_t grammarType) : _grammarType(grammarType) {}
    ~GrammarKeywordDefn() {}

    GrammarType_t getGrammarType() const { return _grammarType; }

    friend std::ostream &operator<<( std::ostream &out, const GrammarKeywordDefn &GKD );
};

#endif
