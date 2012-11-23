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

#ifndef _STATE_FUNCTORS_
#define _STATE_FUNCTORS_

#include "StateAndInput.hpp"
#include <string>

//specialized functors for handling state objects.

//todo: move this into a more relevatnt class
//number of dinstict symbols: number defined below (15 as of now) may be a bit small - adjust as needed
//	1. 0 2. 1-9 3. a-z 4. A-Z, etc..
#define STATE_FUNCTORS_number_of_distinct_symbols 257

class StateAndInputHashFunction {
public:
    ::std::size_t operator ()(const StateAndInput<int,char> &stateAndInput) const 
    {
        return (size_t) STATE_FUNCTORS_number_of_distinct_symbols * (stateAndInput.getIsRanged()? 2 : 1) * stateAndInput.getState() + stateAndInput.getInput();
    }
};

//oops, here I think we need to use hash function...
class StateAndInputEquals {
public:
    bool operator ()(const StateAndInput<int,char> &lhs, const StateAndInput<int,char> &rhs) const 
    {
        if (lhs.getIsAnythingBut() != rhs.getIsAnythingBut())
        {
            return false;
        }

        StateAndInputHashFunction hashFunc;        

        return (hashFunc(lhs) == hashFunc(rhs));
    }
};

#endif
