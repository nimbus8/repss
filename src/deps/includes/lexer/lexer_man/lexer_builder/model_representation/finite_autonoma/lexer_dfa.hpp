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

#include "StateAndInput.hpp"
#include "state_functors.hpp"

#include "../../../../LexerStateAndInput.hpp"

#ifndef _LEXER_DFA_
#define _LEXER_DFA_

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

    void add_next_dfa(const StateAndInput<int,char> stateAndInput, const lexer_dfa* nextDfa) const 
    {
        std::pair<StateAndInput<int,char>, lexer_dfa*> stateInputAndDfa{stateAndInput, const_cast<lexer_dfa*>(nextDfa)}; 
        _nextStates.emplace(stateInputAndDfa); 
    }

    const lexer_dfa* getNextDfa(const LexerStateAndInput& lexerStateAndInput) const;

    //debug
    int getId() const { return _id; }

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
