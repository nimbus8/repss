/*
 REPSS
 Copyright (C) 2013  Khalique Williams

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

#include "base/includes/execution_phase/lexer/construction/model_representation/dfa_manager.hpp"

lexer_word_repr* DfaManager::createRecursiveLexerWordRepr()
{
    /*
        Jan 21, 2013
        This I plan to change. The "encasing" RecursiveWordRepresentation
        should not initially be an upcount indicator, and maybe we don't even
        need the stack until we encounter one. Reducing time complexity and etc.

        But just to see how this gets going we'll leave it like this. We have we
        VERY MUCH do intend on changing this.
    */
    unsigned int lexerDfaProperties = static_cast<unsigned int>(Lexer_Dfa_Properties::ISA_PUSH_DOWN_ACTIVATOR);
    addProperty(lexerDfaProperties, Lexer_Dfa_Properties::ISA_UPCOUNT_INDICATOR);

    lexer_dfa* ret = new lexer_dfa(++_idCount, lexerDfaProperties);
    _dfas.push_back(ret);
    return ret;
}

lexer_word_repr* DfaManager::createInternalRecursiveLexerWordRepresentation()
{
    /*
        We don't plan on using the same technique we do for tentative naming,
        recursive constructs are something else entirely. The recursiveness is
        ALL in the properties we give the dfa.
    */
    unsigned int lexerDfaProperties = static_cast<unsigned int>(Lexer_Dfa_Properties::ISA_UPCOUNT_INDICATOR);

    lexer_dfa* ret = new lexer_dfa(++_idCount, lexerDfaProperties);
    _dfas.push_back(ret);
    return ret;
}

lexer_dfa* DfaManager::createInternalRecursiveAcceptingDfa()
{
    unsigned int lexerDfaProperties = static_cast<unsigned int>(Lexer_Dfa_Properties::ISA_DOWNCOUNT_INDICATOR);

    lexer_dfa* ret = new lexer_dfa(++_idCount, lexerDfaProperties);
    _dfas.push_back(ret);
    return ret;
}

lexer_dfa* DfaManager::createRecursiveAcceptingDfa(const std::string endStateName)
{
    int nextIdCount = _idCount + 1;
    _idCount++;

    std::cout << "CREATING end state mapping for RECURSIVE-DFA: endStateToName=(" << nextIdCount
              << "=>" << endStateName << ")" << std::endl;

    std::pair<int, std::string> idToEndStateName{nextIdCount, endStateName};
    _endStateNameMap.emplace(idToEndStateName);

    unsigned int lexerDfaProperties = static_cast<unsigned int>(Lexer_Dfa_Properties::ISA_DOWNCOUNT_INDICATOR);

    lexer_dfa* ret = new lexer_dfa(nextIdCount, lexerDfaProperties);
    _dfas.push_back(ret);
    return ret;
}


