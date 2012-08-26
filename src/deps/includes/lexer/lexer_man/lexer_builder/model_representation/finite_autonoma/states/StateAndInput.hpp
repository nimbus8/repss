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

#ifndef STATE_AND_INPUT__
#define STATE_AND_INPUT__

//-- Low Tier Interface --//

template<class S, class I>
class StateAndInput
{
private:
        const S _state;
        const I _input;
public:
        StateAndInput(const S state, const I input) : _state(state), _input(input) {}
        StateAndInput(const StateAndInput&  other) : _state(other._state), _input(other._input) {}
        StateAndInput(const StateAndInput&& other) : _state(other._state), _input(other._input) {}

        S getState() const { return _state; }
        I getInput() const { return _input; }

        ~StateAndInput() {}
};

#endif
