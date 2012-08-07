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

#include "states/StateAndInput.hpp"
#include <string>

#ifndef _STATE_FUNCTORS_
#define _STATE_FUNCTORS_

//specialized functors for handling state objects.

class StateAndInputHashFunction {
  public:
    ::std::size_t operator ()(const StateAndInput<int,char> &stateAndInput) const {
        return (size_t) 100*stateAndInput.getState() + stateAndInput.getInput();
    }
};

class StateAndInputEquals {
  public:
    bool operator ()(const StateAndInput<int,char> &lhs, const StateAndInput<int,char> &rhs) const {
        if ((lhs.getState() != rhs.getState()) || (lhs.getInput() != rhs.getInput()))
                return false;

        return true;
    }
};

#endif