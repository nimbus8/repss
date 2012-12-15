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

#include "../../Genrtd_Keywords.hpp"
 
class GrammarKeywordDefn
{
private:
    //(1) rename AbstrKeyword to Keyword. (2) we're going to have to give Keyword a copy/move constructor
    AbstrKeyword *_keyword;
public:
    GrammarKeywordDefn() : _keyword(nullptr) {} //not sure if we keep this
    GrammarKeywordDefn(const GrammarKeywordDefn& other) : _keyword(other._keyword) {}
    explicit GrammarKeywordDefn(const AbstrKeyword* keyword) : _keyword(const_cast<AbstrKeyword*>(keyword)) {}
    ~GrammarKeywordDefn() {}

    void setKeyword(const AbstrKeyword* keyword)
    {
        _keyword = const_cast<AbstrKeyword*>(keyword);
    } 

    std::string getName() const { return (_keyword != nullptr? _keyword->getName() : ""); }
    GrammarType_t getGrammarType() const { return (_keyword != nullptr? _keyword->getGrammarType() : GrammarType_t::UNDEFINED); }

    friend std::ostream &operator<<( std::ostream &out, const GrammarKeywordDefn &GKD );
};

#endif
