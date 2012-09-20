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

#ifndef _DFA_TRANSITION_
#define _DFA_TRANSITION_

#include "lexer_dfa.hpp"

class DfaTransition
{
private:
        const StateAndInput<int,char>* _stateAndInput;
        const lexer_dfa* _dfaNode;
public:
        DfaTransition(const StateAndInput<int,char>* stateAndInput, const lexer_dfa* dfaNode)
                : _stateAndInput(stateAndInput), _dfaNode(dfaNode) {}

        const StateAndInput<int,char>* getStateAndInput() const { return _stateAndInput; }
        const lexer_dfa* getDfaNode() const { return _dfaNode; }

        ~DfaTransition() {}
};

DfaTransition* CreateDfaTransition(const StateAndInput<int,char>* stateAndInput, const lexer_dfa* dfa_ptr);

#endif
