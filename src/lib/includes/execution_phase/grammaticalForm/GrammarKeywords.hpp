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

#ifndef _GRAMMAR_KEYWORDS_
#define _GRAMMAR_KEYWORDS_

#include <iostream>

#include "GrammarKeywordDefn.hpp"

namespace std
{

class GrammarKeywords;
 
class GrammarKeywordIter
{
    public:
    GrammarKeywordIter (const GrammarKeywords* p_vec, int pos)
        : _pos( pos ), _p_vec( p_vec )
    {
    }
 
    bool operator!= (const GrammarKeywordIter& other) const
    {
        return _pos != other._pos;
    }
 
    GrammarKeywordDefn operator* () const;
 
    const GrammarKeywordIter& operator++ ()
    {
        ++_pos;
        return *this;
    }
 
    private:
    int _pos;
    const GrammarKeywords *_p_vec;
};

//maybe we should have this generated? and have the 'array' defined in place from some sort
//of definition language?
class GrammarKeywords
{
public:
    GrammarKeywords() : _count(0)
    {
    }
 
    const GrammarKeywordDefn get (int col) const
    {
        return _data[ col ];
    }

    GrammarKeywordIter begin () const
    {
        return GrammarKeywordIter( this, 0 );
    }
 
    GrammarKeywordIter end () const
    {
        return GrammarKeywordIter( this, 100 );
    }

    void add(const GrammarKeywordDefn grammarKeywordDefn)
    {
        _data[_count] = grammarKeywordDefn;
        _count++;
    }
 
    void set (int index, GrammarKeywordDefn&& val)
    {
        _data[ index ] = val;
    }
 
private:
    GrammarKeywordDefn _data[ 100 ];
    size_t _count;
};


inline GrammarKeywordDefn GrammarKeywordIter::operator* () const
{
     return _p_vec->get( _pos );
}

inline bool testGrammarKeywords()
{
    size_t NUMBER_OF_KEYWORDS = 100;
    GrammarKeywords v;
    for ( int i = 0; i < NUMBER_OF_KEYWORDS; i++ )
    {
        v.set( i , GrammarKeywordDefn{(i % 2 == 0? GrammarType_t::VARIABLE : GrammarType_t::TERMINAL)});
    }

    size_t counted = 0;
    for ( GrammarKeywordDefn i : v ) 
    { 
        counted++;
    }

    return (counted == NUMBER_OF_KEYWORDS);
}

}

#endif
