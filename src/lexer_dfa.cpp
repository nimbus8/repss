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

 If you wish to contact the author of REPSS, you may do so at
 kwillia.pub@gmail.com
 */

#include "base/includes/execution_phase/lexer/construction/model_representation/finite_autonoma/lexer_dfa.hpp"

#define DEBUG
//#undef DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
    #define DLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str)
#else
    #define DeLOG(str)
    #define DLOG(str)
#endif

const lexer_dfa* lexer_dfa::getNextDfa(const LexerStateAndInput& lexerStateAndInput) const
{
    const StateAndInput<int,char> stateAndInput(lexerStateAndInput.getState(), 
        lexerStateAndInput.getInput(), false);

    //_printInputHash(stateAndInput, "stateAndInput"); //commented in order to benchmark diff between ScanWords
    DeLOG(std::string{"\t_nextStates::size = "}.append(std::to_string(_nextStates.size())).append("\n").c_str());
    //_printTransitions(); //commented in order to benchmark diff between ScanWords

    lexer_dfa* ret;

    std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetched
        = _nextStates.find(stateAndInput);

    //if we can't find anything, it may be possible we enountered
    //the special "ranged" stateAndInput - which is guaranteed to be mapped
    //to a unique index in hashmap (its in the formulae)
    if (fetched == _nextStates.end())
    {
        ret = nullptr;

        char input = stateAndInput.getInput();
        if (input == '0')
        {
            StateAndInput<int,char> rangedInput(stateAndInput.getState(), SI_NUMBERS_0, true);

            //std::cout << "\t\t"; //commented in order to benchmark diff between ScanWords
            //_printInputHash(rangedInput, "rangedInput"); //commented in order to benchmark diff between ScanWords

            std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedNumbers0
                = _nextStates.find(rangedInput);

            if (fetchedNumbers0 != _nextStates.end())
            {
                //std::cout << "\tFound rangedNumber! (0)" << std::endl; //commented in order to benchmark diff between ScanWords
                ret = fetchedNumbers0->second;
            }
            else
            {
                StateAndInput<int,char> rangedInput2(stateAndInput.getState(), SI_NUMBERS_0to9, true);
                //std::cout << "\t\t"; //commented in order to benchmark diff between ScanWords
                //_printInputHash(rangedInput2, "SI_NUMBERS_0to9"); //commented in order to benchmark diff between ScanWords

                std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedNumbers0to9
                    = _nextStates.find(rangedInput2);

                if (fetchedNumbers0to9 != _nextStates.end())
                {
                    //std::cout << "\trangedNumber:[0-9]" << std::endl; //commented in order to benchmark diff between ScanWords
                    ret = fetchedNumbers0to9->second;
                }
            }
        }
        else if (input >= '1' && input <= '9')
        {
            StateAndInput<int,char> rangedInput0to9(stateAndInput.getState(), SI_NUMBERS_0to9, true);

            //std::cout << "\t\t"; //commented in order to benchmark diff between ScanWords
            //printInputHash(rangedInput0to9, "rangedInputNumbers0to9"); //commented in order to benchmark diff between ScanWords

            std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedNumbers0to9
                = _nextStates.find(rangedInput0to9);

            if (fetchedNumbers0to9 != _nextStates.end())
            {
                //std::cout << "\tFound rangedNumber! ([0-9])" << std::endl; //commented in order to benchmark diff between ScanWords
                ret = fetchedNumbers0to9->second;
            }
            else
            {
                StateAndInput<int,char> rangedInput1to9(stateAndInput.getState(), SI_NUMBERS_1to9, true);
                //std::cout << "\t\t"; //commented in order to benchmark diff between ScanWords
                //_printInputHash(rangedInput1to9, "rangedInputNumbers1to9"); //commented in order to benchmark diff between ScanWords

                std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedNumbers1to9
                    = _nextStates.find(rangedInput1to9);

                if (fetchedNumbers1to9 != _nextStates.end())
                {
                    //std::cout << "\trangedNumber:[1-9]" << std::endl; //commented in order to benchmark diff between ScanWords
                    ret = fetchedNumbers1to9->second;
                }
            }
        }   
        else if (input >= 'a' && input <= 'z')
        {
            //std::cout << "\tChecking lowerase ranged" << std::endl; //commented in order to benchmark diff between ScanWords

            StateAndInput<int,char> rangedInput(stateAndInput.getState(), SI_CHARS_LOWER, true);

            //std::cout << "\t\t";
            //_printInputHash(rangedInput, "rangedInput"); //commented in order to benchmark diff between ScanWords

            //std::cout << "\t\tlexer_dfa::getNextState(...): (state,input) = (" << stateAndInput.getState() << ", SI_CHARS_LOWER)" << std::endl;

            std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedCharsLower
                = _nextStates.find(rangedInput);

            if (fetchedCharsLower != _nextStates.end())
            {
                //std::cout << "\tFound rangedChars:[a-z]" << std::endl; //commented in order to benchmark diff between ScanWords
                ret = fetchedCharsLower->second;
            }
            else
            {
                StateAndInput<int,char> rangedInput2(stateAndInput.getState(), SI_CHARS_ANY, true);

                //std::cout << "\t\t"; //commented in order to benchmark diff between ScanWords
                //_printInputHash(rangedInput2, "rangedInput2"); //commented in order to benchmark diff between ScanWords

                //std::cout << "\t\tlexer_dfa::getNextState(...): (state,input) = (" 
                //<< stateAndInput.getState() << ", SI_CHARS_ANY)" << std::endl; //commented in order to benchmark diff between ScanWords

                std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedCharsAny
                    = _nextStates.find(rangedInput2);

                if (fetchedCharsAny != _nextStates.end())
                {
                    //std::cout << "rangedChars:([a-z]|[A-Z])" << std::endl; //commented in order to benchmark diff between ScanWords
                    ret = fetchedCharsAny->second;
                }

            }   
        }
        else if (input >= 'A' && input <= 'Z')
        {
            //std::cout << "\tChecking uppercase ranged" << std::endl; //commented in order to benchmark diff between ScanWords

            StateAndInput<int,char> rangedInput(stateAndInput.getState(), SI_CHARS_UPPER, true);

            //std::cout << "\t\t";
            //_printInputHash(rangedInput, "rangedInput"); //commented in order to benchmark diff between ScanWords

            //std::cout << "\t\tlexer_dfa::getNextState(...): (state,input) = (" << stateAndInput.getState() << ", SI_CHARS_UPPER)" << std::endl;

            std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedCharsUpper
                = _nextStates.find(rangedInput);

            if (fetchedCharsUpper != _nextStates.end())
            {
                //std::cout << "\tFound rangedChars:[A-Z]" << std::endl; //commented in order to benchmark diff between ScanWords
                ret = fetchedCharsUpper->second;
            }
            else
            {
                StateAndInput<int,char> rangedInput2(stateAndInput.getState(), SI_CHARS_ANY, true);

                //std::cout << "\t\t";
                //_printInputHash(rangedInput, "rangedInput"); //commented in order to benchmark diff between ScanWords

                //std::cout << "\t\tlexer_dfa::getNextState(...): (state,input) = (" << stateAndInput.getState() << ", SI_CHARS_ANY)" << std::endl;

                std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedCharsAny
                    = _nextStates.find(rangedInput2);

                if (fetchedCharsAny != _nextStates.end())
                {
                    //std::cout << "rangedChars:([a-z]|[A-Z])" << std::endl; //commented in order to benchmark diff between ScanWords
                    ret = fetchedCharsAny->second;
                }
            }
        }

        //if by now ret has not been set to soemething other than nullptr, we have one last restort in the empty char
        if (ret == nullptr)
        {
            //we check for 'anythingBut' before we finally check for empty string -- this is the going protocol for now
            if (_anythingButTransition != nullptr)
            {
                if (_anythingButTransition->getIsRanged())
                {
                    //todo: perform range checks for anything buts
                }
                else if (input != _anythingButTransition->getStateAndInput().getInput())
                {
                   ret = const_cast<lexer_dfa*>(_anythingButTransition->getDfaNode());
                }
            }

            if (ret == nullptr)
            {
                //check case of empty char
                StateAndInput<int,char> stateAndEmptyCharInput(stateAndInput.getState(), '\0');

                //std::cout << "\t\t";
                //_printInputHash(stateAndEmptyCharInput, "stateAndEmptyInput"); //commented in order to benchmark diff between ScanWords

                std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals>::const_iterator fetchedEmptyChar
                    = _nextStates.find(stateAndEmptyCharInput);                

                if (fetchedEmptyChar != _nextStates.end())
                {
                    //std::cout << "\tfound empty char!!!" << std::endl;
                    ret = fetchedEmptyChar->second;
                }
                else
                {
                    //std::cout << "\tkey not found" << std::endl;
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

#undef DEBUG
#undef DLOG
#undef DeLOG
