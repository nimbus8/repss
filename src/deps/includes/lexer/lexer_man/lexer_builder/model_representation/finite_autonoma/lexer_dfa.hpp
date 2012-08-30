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

class LexerDfaType
{
public:
	typedef enum {
		NORMAL=0, RANGED 		
	} ValidTypes;
};

class lexer_dfa
{
private:
        std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals> _nextStates;

	const LexerDfaType::ValidTypes _type;
        int _id;

	//debug
	void _printTransitions() const
	{
		StateAndInputHashFunction hashFunc;
		for (auto iter : _nextStates)
		{
			auto inputKey = iter.first;
			auto dfaPtr = iter.second;
			std::cout << "\t(<" << inputKey.getState() << ", " << inputKey.getInput() << ", ranged?(" << (inputKey.getIsRanged()? "yes" : "no") << ")>, " 
				<< "dfa-id(" << dfaPtr->getId() << "))"
				<< "\t- hash(" << hashFunc(inputKey)  << ")" << std::endl;
		}
	}

	void _printInputHash(const StateAndInput<int,char>& stateAndInput, const std::string& name) const
	{
                StateAndInputHashFunction hashFunc;
		std::cout << "\tHash(" << name << ") = " << hashFunc(stateAndInput) << std::endl;
	}


public:
        lexer_dfa(LexerDfaType::ValidTypes type, int id) : _type(type), _id(id) {}
        explicit lexer_dfa(int id) : _type(LexerDfaType::NORMAL), _id(id) {}

        ~lexer_dfa() {}

        void add_next_dfa(const StateAndInput<int,char> stateAndInput, const lexer_dfa* nextDfa) 
	{
		std::pair<StateAndInput<int,char>, lexer_dfa*> stateInputAndDfa{stateAndInput, const_cast<lexer_dfa*>(nextDfa)}; 
		_nextStates.emplace(stateInputAndDfa); 
	}

        const lexer_dfa* getNextDfa(const StateAndInput<int,char> stateAndInput) const
        {
		_printInputHash(stateAndInput, "stateAndInput");

		std::cout << "\t_nextStates::size = " << _nextStates.size() << std::endl;
		_printTransitions();

		lexer_dfa* ret;

		std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetched 
			= _nextStates.find(stateAndInput);

		//if we can't find anything, it may be possible we enountered 
		//the special "ranged" stateAndInput - which is guaranteed to be mapped
		//to a unique index in hashmap (its in the formulae)
		if (fetched == _nextStates.end())
		{
			//todo: create checks for ranged StateAndInput
			//	- simple: use isdigit, isUpper, and whatever to ascertain type
			//		of input and check for the cases relative to it
			//		- we shouldn't try and handle ambiguities here, its up
			//			to caller to not include any ambiguities.

			bool matchedRangedInput = false;
			ret = nullptr;

			char input = stateAndInput.getInput();
			if (input == '0')
			{
				StateAndInput<int,char> rangedInput(stateAndInput.getState(), SI_NUMBERS_0, true);

				
				std::cout << "\t\t";
				_printInputHash(rangedInput, "rangedInput");

				std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedNumbers0
                        		= _nextStates.find(rangedInput);

				if (fetchedNumbers0 != _nextStates.end())
				{
					std::cout << "\tFound rangedNumber! (0)" << std::endl;
					ret = fetchedNumbers0->second;
				} 
				else
				{
					StateAndInput<int,char> rangedInput2(stateAndInput.getState(), SI_NUMBERS_0to9, true);
                                	std::cout << "\t\t";
                                	_printInputHash(rangedInput2, "SI_NUMBERS_0to9");

              		                std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedNumbers0to9
                        	                = _nextStates.find(rangedInput2);

					if (fetchedNumbers0to9 != _nextStates.end())
					{
	                                        std::cout << "\trangedNumber:[0-9]" << std::endl;
						ret = fetchedNumbers0to9->second;
					}
				}
			}
			else if (input >= '1' && input <= '9')
			{
				StateAndInput<int,char> rangedInput0to9(stateAndInput.getState(), SI_NUMBERS_0to9, true);

                                std::cout << "\t\t";
                                _printInputHash(rangedInput0to9, "rangedInputNumbers0to9");

                                std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedNumbers0to9
                                        = _nextStates.find(rangedInput0to9);

                                if (fetchedNumbers0to9 != _nextStates.end())
                                {
                                        std::cout << "\tFound rangedNumber! ([0-9])" << std::endl;
                                        ret = fetchedNumbers0to9->second;
                                }
				else
				{
					StateAndInput<int,char> rangedInput1to9(stateAndInput.getState(), SI_NUMBERS_1to9, true);
                                        std::cout << "\t\t";
                                        _printInputHash(rangedInput1to9, "rangedInputNumbers1to9");

                                        std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedNumbers1to9
                                                = _nextStates.find(rangedInput1to9);

                                        if (fetchedNumbers1to9 != _nextStates.end())
                                        {
                                                std::cout << "\trangedNumber:[1-9]" << std::endl;
                                                ret = fetchedNumbers1to9->second;
                                        }		
				}
			}
			else if (input >= 'a' && input <= 'z')
			{
				std::cout << "\tChecking lowerase ranged" << std::endl;

				StateAndInput<int,char> rangedInput(stateAndInput.getState(), SI_CHARS_LOWER, true);

                                std::cout << "\t\t";
                                _printInputHash(rangedInput, "rangedInput");

				std::cout << "\t\tlexer_dfa::getNextState(...): (state,input) = (" << stateAndInput.getState() << ", SI_CHARS_LOWER)" << std::endl;

                                std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedCharsLower
                                        = _nextStates.find(rangedInput);

				if (fetchedCharsLower != _nextStates.end())
				{
					std::cout << "\tFound rangedChars:[a-z]" << std::endl;
					ret = fetchedCharsLower->second;
				}
				else
				{
					StateAndInput<int,char> rangedInput2(stateAndInput.getState(), SI_CHARS_ANY, true);

                                	std::cout << "\t\t";
                                	_printInputHash(rangedInput2, "rangedInput2");

	                                std::cout << "\t\tlexer_dfa::getNextState(...): (state,input) = (" << stateAndInput.getState() << ", SI_CHARS_ANY)" << std::endl;

                                	std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedCharsAny
                                        	= _nextStates.find(rangedInput2);

					if (fetchedCharsAny != _nextStates.end())
					{
						std::cout << "rangedChars:([a-z]|[A-Z])" << std::endl;
						ret = fetchedCharsAny->second;
					}
				}
			}
			else if (input >= 'A' && input <= 'Z')
			{
				StateAndInput<int,char> rangedInput(stateAndInput.getState(), SI_CHARS_UPPER, true);
				StateAndInput<int,char> rangedInput2(stateAndInput.getState(), SI_CHARS_ANY, true);
			}
			
			//if by now ret has not been set to soemething other than nullptr, we have one last restort in the empty char
			if (ret == nullptr)
			{
				if (!matchedRangedInput)
				{
					//check case of empty char
					StateAndInput<int,char> stateAndEmptyCharInput(stateAndInput.getState(), '\0');

	                                std::cout << "\t\t";
                                	_printInputHash(stateAndEmptyCharInput, "stateAndEmptyInput");
	
					std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedEmptyChar 
						= _nextStates.find(stateAndEmptyCharInput);			

					if (fetchedEmptyChar != _nextStates.end())
					{
						std::cout << "\tfound empty char!!!" << std::endl;
						ret = fetchedEmptyChar->second;
					}
					else
					{
						std::cout << "\tkey not found" << std::endl;
						ret = nullptr;
					}
				}
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

//this really doesn't make sense any more, only really for easier identification, well be branching
//behaviour based on type of StateAndInput instead.
inline lexer_dfa* CreateRangedDfa(int id)
{
	return new lexer_dfa(LexerDfaType::RANGED, id);
}

typedef lexer_dfa lexer_word_repr;

inline lexer_word_repr* CreateLexerWord(int id)
{
	return new lexer_word_repr(id);
}

inline lexer_word_repr* CreateRangedLexerWord(int id)
{
	return new lexer_word_repr(LexerDfaType::RANGED, id); 
}

#endif
