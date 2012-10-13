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

//todo:will change this please, I don't like the defines here

#define SI_EMPTY 'e'
#define SI_CHARS_LOWER 'a'
#define SI_CHARS_UPPER 'Z'
#define SI_CHARS_ANY 'n'
#define SI_NUMBERS_0 '0'
#define SI_NUMBERS_1to9 '1'
#define SI_NUMBERS_0to9 '2'

#define NUMBER_OF_RANGED_SI_CATEGORIES 7

template<class S, class I>
class StateAndInput
{
private:
    const S _state;
    const I _input;
    const bool _isRanged;
public:
    StateAndInput(const S state, const I input) : _state(state), _input(input), _isRanged(false) {}
    StateAndInput(const S state, const I input, bool isRanged) : _state(state), _input(input), _isRanged(isRanged) {}
    StateAndInput(const StateAndInput&  other) : _state(other._state), _input(other._input), _isRanged(other._isRanged) {}
    StateAndInput(const StateAndInput&& other) : _state(other._state), _input(other._input), _isRanged(other._isRanged) {}
    ~StateAndInput() {}

    S getState() const { return _state; }
    I getInput() const { return _input; }
    bool getIsRanged() const { return _isRanged; }
};

#endif
