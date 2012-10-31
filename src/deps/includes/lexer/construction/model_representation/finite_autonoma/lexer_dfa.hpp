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

#ifndef _LEXER_DFA_
#define _LEXER_DFA_

#include "StateAndInput.hpp"
#include "state_functors.hpp"
#include "LexerStateAndInput.hpp"

#ifdef DEBUG
     #ifndef DLOG(str)
         #define DLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str)
     #endif
#else
    #define DLOG(str)
#endif

class lexer_dfa;

class LexerTransition
{
private:
        const StateAndInput<int,char> _stateAndInput;
        const lexer_dfa* _dfaNode;
public:
        LexerTransition(const StateAndInput<int,char> stateAndInput, const lexer_dfa* dfaNode)
                : _stateAndInput(stateAndInput), _dfaNode(dfaNode) {}

        LexerTransition(const LexerTransition& other) : _stateAndInput(other._stateAndInput), _dfaNode(other._dfaNode) {}
        LexerTransition(const LexerTransition&& other) : _stateAndInput(other._stateAndInput), _dfaNode(other._dfaNode) {}
        const LexerStateAndInput getStateAndInput() const {
            LexerStateAndInput lexerStateAndInput(_stateAndInput.getState(), _stateAndInput.getInput());
            return lexerStateAndInput;
        }
        const lexer_dfa* getDfaNode() const { return _dfaNode; }

        bool getIsRanged() const { return _stateAndInput.getIsRanged(); }
        bool getIsAnythingBut() const { return _stateAndInput.getIsAnythingBut(); }

        ~LexerTransition() {}
};

class lexer_dfa
{
private:
    mutable std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals> _nextStates;

    LexerTransition* _anythingButTransition;

    int _id;

    void _printInputHash(const StateAndInput<int,char>& stateAndInput, const std::string& name) const
    {
        StateAndInputHashFunction hashFunc;
        //std::cout << "\tHash(" << name << ") = " << hashFunc(stateAndInput) << std::endl; //commented in order to benchmark diff between ScanWords
    }
public:
    explicit lexer_dfa(int id) : _id(id), _anythingButTransition(nullptr) {}

    ~lexer_dfa()
    {
        if (_anythingButTransition != nullptr)
        {
            delete _anythingButTransition;
        }
    }

    void _printTransitions() const
    {
        StateAndInputHashFunction hashFunc;
        for (auto iter : _nextStates)
        {
            auto inputKey = iter.first;
            auto dfaPtr = iter.second;
            //std::cout << "\t(<" << inputKey.getState() << ", " << inputKey.getInput() << ", ranged?(" << (inputKey.getIsRanged()? "yes" : "no") << ")>, "
              //  << "dfa-id(" << dfaPtr->getId() << "))"
              //  << "\t- hash(" << hashFunc(inputKey)  << ")" << std::endl; //commented in order to benchmark diff between ScanWords
        }

        if (_anythingButTransition != nullptr)
        {
            auto stateAndInput = _anythingButTransition->getStateAndInput();
            auto dfaPtr = _anythingButTransition->getDfaNode();
            //std::cout << "\t(<" << stateAndInput.getState() << ", " << stateAndInput.getInput()
                      //<< ", ranged?(" << (stateAndInput.getIsRanged()? "yes" : "no") << ")>,"
                      //<< "dfa-id(" << dfaPtr->getId() << "))" << std::endl; //commented in order to benchmark diff b/w Scanwords
        }
    }

    void add_next_dfa(const StateAndInput<int,char> stateAndInput, const lexer_dfa* nextDfa) 
    {
        if (!stateAndInput.getIsAnythingBut())
        {
            std::pair<StateAndInput<int,char>, lexer_dfa*> stateInputAndDfa{stateAndInput, const_cast<lexer_dfa*>(nextDfa)}; 
            _nextStates.emplace(stateInputAndDfa);
        }
        else
        {
            if (_anythingButTransition != nullptr)
            {
                std::cout << "Ooops, cannot define more tha one anythingButTransition for a single Dfa!!! Exiting." << std::endl;

                exit(1);
            }

            std::cout << "Adding AnythingButTransition!" << std::endl;
            const LexerTransition* anythingButTransition = new LexerTransition(stateAndInput, nextDfa);
            _anythingButTransition = const_cast<LexerTransition*>(anythingButTransition);
        }
    }

    int getId() const { return _id; }

    //this function is relatively expensive, remember that lexer_dfa is for language description
    //todo: create a similar class that is solel for language traversal (lighter, faster), e.g. similar
    //func to the one below who NOT enumerate all possible values in map
    bool hasNextDfaForInput(const char input) const
    {
        for (auto iter : _nextStates)
        {
            const auto inputKey = iter.first;
            const auto inputKeyValue = inputKey.getInput();

            if (inputKey.getIsAnythingBut() && inputKeyValue != input)
            {
                return true;           
            }
            else if (inputKeyValue == input)
            {
                return true;
            }
       }

        return false;
    }

    //we actually plan on using this one, its simple but its important in ScanWord Construction and/or evaluation
    //this returns a function can verify whether input(char) is valid with a particular 'state'(int)
    //use case: isValidRangedInput(range_type, input) = true/false
    //       -- forward notice: we keep 'range' data on this level (embedded), but it should be explicitly defined minimal set for
    //            say, use, in ScanWordNode.
    //                ScanWordNode{ id:int, valid_ranged_transitions:list<rangedTransition{ranged_input_type:int, nextNode:ScanWordNode}> }
    //                   [Note: we're okay with iterating through this list as a second option to the hash table. Unlike the hashtable,
    //                   this list is at max like 5 entries. It can be stored just as an array, and iterated through quickly.
    std::function<bool (char, char)> getFnIsInputWithinRange() const
    {
        std::function<bool (char, char)> isInputWithinRange = 
            [](char rangeType, char aInput)->bool
            {
                switch(rangeType)
                {
                    case SI_EMPTY:
                        return true;
                        break;
                    case SI_CHARS_LOWER:
                        return islower(aInput);
                        break;
                    case SI_CHARS_UPPER:
                        return isupper(aInput);
                        break;
                    case SI_CHARS_ANY:
                        return isalpha(aInput);
                        break;
                    case SI_NUMBERS_0:
                        return aInput == '0';
                        break;
                    case SI_NUMBERS_1to9:
                        return (aInput >= '1' && aInput <= '9');
                        break;
                    case SI_NUMBERS_0to9:
                        return isdigit(aInput);
                        break;
                    default:
                        return false;
               };
          };

          return isInputWithinRange;
    }

    //not quite sure why I made this, possessed perhaps...delete if no use found
    bool isValidRangedInput(const char range_type, const char input) const
    {
        auto fnIsInRange = getFnIsInputWithinRange();

        return fnIsInRange(range_type, input);
    }

    //like the above function, this is only really meant to be used in the dfa merge process.
    //afterwards, during lexing, there should be a different interface (if not a completely new class)
    //that makes this operation quick
    const lexer_dfa* getNextDfaForInput(const char input) const
    {
    //std::cout << "\t_nextStates::size = " << _nextStates.size() << std::endl; //commented in order to benchmark diff between ScanWords
    //_printTransitions();

        //here we shoudl accout for the special ANYTHING BUT
        
        for (auto iter : _nextStates)
        {
            const auto inputKey = iter.first;
            const auto inputKeyValue = inputKey.getInput();
            //std::cout << "\tlexer_dfa::getNextDfaForInput(...) (<testing input '" << inputKeyValue << "'>)" << std::endl; //commented in order to benchmark diff between ScanWords

            //first check case if state transition is ranged
            if (!inputKey.getIsRanged())
            {
                //std::cout << "\tlexerDfa::getNextDfaForInput -- input key UNranged\n"; //commented in order to benchmark diff between ScanWords

                if (inputKeyValue == input)
                {
                    const auto retDfaPtr = iter.second;
                    return retDfaPtr;
                }
            }
            else
            {
                //std::cout << "\tlexerDfa::getNextDfaForInput -- input key RANGED\n"; //commented in order to benchmark diff between ScanWords

                //inputKeyValue = rangeCategor
                //std::string isValidRangedInputLog("calling isValidRangedInput(");
                //isValidRangedInputLog.append(1, inputKeyValue);
                //isValidRangedInputLog.append(1,',');
                //isValidRangedInputLog.append(1,input);
                //isValidRangedInputLog.append(")\n");


                //std::cout << "\t" << isValidRangedInputLog; //commented in order to benchmark diff between ScanWords
                //DLOG(isValidRangedInputLog.c_str());

                 if (isValidRangedInput(inputKeyValue, input))
                 {
                    const auto retDfaPtr = iter.second;
                    return retDfaPtr;
                 }
            }
        }    

        if (_anythingButTransition != nullptr)
        {
            const auto stateAndInput = _anythingButTransition->getStateAndInput();
            const auto anythingButInput = stateAndInput.getInput();
            if (_anythingButTransition->getIsRanged() && !isValidRangedInput(anythingButInput, input))
            {
                const auto retDfaPtr = _anythingButTransition->getDfaNode();
                return retDfaPtr;
            }
            else if (anythingButInput != input)
            {
                const auto retDfaPtr = _anythingButTransition->getDfaNode();
                return retDfaPtr;
            }
        }
            return nullptr;
    }
    
    const lexer_dfa* getNextDfa(const LexerStateAndInput& lexerStateAndInput) const;

    std::vector<LexerTransition> getTransitions() const
    {
        std::vector<LexerTransition> allTransitions;

        for (auto iter : _nextStates)
        {
            auto inputKey = iter.first;
            auto dfaPtr = iter.second;

            const LexerTransition aTransition(inputKey, dfaPtr);
            allTransitions.push_back(aTransition);
        }

        if (_anythingButTransition != nullptr)
        {
            allTransitions.push_back(*_anythingButTransition);
        }

        return allTransitions;
    }
};

typedef lexer_dfa lexer_word_repr;

#endif
