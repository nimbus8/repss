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
 
    int operator* () const;
 
    const GrammarKeywordIter& operator++ ()
    {
        ++_pos;
        return *this;
    }
 
    private:
    int _pos;
    const GrammarKeywords *_p_vec;
};

class GrammarKeywords
{
public:
    GrammarKeywords()
    {
    }
 
    int get (int col) const
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
 
    void set (int index, int val)
    {
        _data[ index ] = val;
    }
 
private:
    int _data[ 100 ];
};

int
GrammarKeywordIter::operator* () const
{
     return _p_vec->get( _pos );
}

void testGrammarKeywords()
{
    GrammarKeywords v;
    for ( int i = 0; i < 100; i++ )
    {
        v.set( i , i );
    }
    for ( int i : v ) { cout << i << endl; }
}

}

#endif
