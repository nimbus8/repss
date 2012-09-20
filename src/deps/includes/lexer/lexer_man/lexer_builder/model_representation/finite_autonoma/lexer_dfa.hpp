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

//todo: are we using this...I dont think so, see if can delete this
class LexerDfaType
{
public:
    typedef enum {
        NORMAL=0, RANGED 		
    } ValidTypes;
};

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

        ~LexerTransition() {}
};

class lexer_dfa
{
private:
    mutable std::unordered_map<StateAndInput<int,char>, lexer_dfa*, StateAndInputHashFunction, StateAndInputEquals> _nextStates;

    const LexerDfaType::ValidTypes _type;
    int _id;

    void _printInputHash(const StateAndInput<int,char>& stateAndInput, const std::string& name) const
    {
        StateAndInputHashFunction hashFunc;
        std::cout << "\tHash(" << name << ") = " << hashFunc(stateAndInput) << std::endl;
    }
public:
    lexer_dfa(LexerDfaType::ValidTypes type, int id) : _type(type), _id(id) {}
    explicit lexer_dfa(int id) : _type(LexerDfaType::NORMAL), _id(id) {}

    ~lexer_dfa() {}

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

    void add_next_dfa(const StateAndInput<int,char> stateAndInput, const lexer_dfa* nextDfa) const 
    {
        std::pair<StateAndInput<int,char>, lexer_dfa*> stateInputAndDfa{stateAndInput, const_cast<lexer_dfa*>(nextDfa)}; 
        _nextStates.emplace(stateInputAndDfa); 
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
            if (inputKeyValue == input)
            {
                return true;
            }
       }

        return false;
    }

    //like the above function, this is only really meant to be used in the dfa merge process.
    //afterwards, during lexing, there should be a different interface (if not a completely new class)
    //that makes this operation quick
    const lexer_dfa* getNextDfaForInput(const char input) const
    {
        for (auto iter : _nextStates)
        {
            const auto inputKey = iter.first;
            const auto inputKeyValue = inputKey.getInput();
            if (inputKeyValue == input)
            {
                const auto retDfaPtr = iter.second;
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

        return allTransitions;
    }
};

typedef lexer_dfa lexer_word_repr;

#endif
