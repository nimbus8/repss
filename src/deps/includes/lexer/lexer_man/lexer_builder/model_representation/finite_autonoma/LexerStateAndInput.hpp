/*
 REPSS
 Copyright (C) 2012  Khalique Williams

 This file is part of REPSS.

 REPSS is free software: you can redistribute it and/or modify
 it under the terms of the   GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 REPSS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with REPSS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LEXER_STATE_AND_INPUT_
#define _LEXER_STATE_AND_INPUT_


//what seperates this from StateAndInput is there is no concept of ranged.
class LexerStateAndInput
{
private:
    const int _state;
    const char _input;
public:
    LexerStateAndInput(const int state, const char input) : _state(state), _input(input) {}
    LexerStateAndInput(const LexerStateAndInput&  other) : _state(other._state), _input(other._input) {}
    LexerStateAndInput(const LexerStateAndInput&& other) : _state(other._state), _input(other._input) {}
    ~LexerStateAndInput() {}

    int getState() const { return _state; }
    char getInput() const { return _input; }
};

#endif
