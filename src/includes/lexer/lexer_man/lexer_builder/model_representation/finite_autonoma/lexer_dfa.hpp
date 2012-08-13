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

#include <iostream>
#include <vector>
#include <string>

#include <stdarg.h>
#include <utility>
#include <functional>
#include <unordered_map>

#include <memory>

#include "states/StateAndInput.hpp"
#include "state_functors.hpp"

#ifndef _LEXER_DFA_
#define _LEXER_DFA_

class lexer_dfa
{
private:
        std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals> _nextStates;

        //debug
        int _id;

public:
        explicit lexer_dfa(int id) : _id(id) {}
        ~lexer_dfa() {}

        void add_next_dfa(StateAndInput<int,char> stateAndInput, lexer_dfa* nextDfa) { _nextStates.emplace(std::pair<StateAndInput<int,char>, lexer_dfa*>{stateAndInput, nextDfa}); }

        lexer_dfa* getNextDfa(StateAndInput<int,char> stateAndInput)
        {
		lexer_dfa* ret;
		std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetched 
			= _nextStates.find(stateAndInput);

		if (fetched == _nextStates.end())
		{
			StateAndInput<int,char> stateAndEmptyCharInput(stateAndInput.getState(), '\0');
		
			std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedEmptyChar 
				= _nextStates.find(stateAndEmptyCharInput);			

			if (fetchedEmptyChar != _nextStates.end())
			{
				std::cout << "found empty char!!!" << std::endl;
				ret = fetchedEmptyChar->second;
			}
			else
			{
				std::cout << "key not found" << std::endl;
				ret = nullptr;
			}
		}
		else
		{
			ret = fetched->second;
		}

		return ret;
        }

        //debug
        int getId() const { return _id; }
};

//phasing out the below calls (moving them to DfaMan)

inline lexer_dfa* CreateDfa(int id)
{
	return new lexer_dfa(id);
}

typedef lexer_dfa lexer_word_repr;

inline lexer_word_repr* CreateLexerWord(int id)
{
	return new lexer_word_repr(id);
}

#endif
