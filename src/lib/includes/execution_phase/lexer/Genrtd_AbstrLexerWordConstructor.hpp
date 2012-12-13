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

//Should you modify this file? NO

#include "construction/ConstructionTypedefs.hpp"

#ifndef _ABSTR_LEXER_WORD_CONSTRUCTOR_
#define _ABSTR_LEXER_WORD_CONSTRUCTOR_

class AbstrLexerWordConstructor
{
protected:
    std::vector<word_start_and_aggregated_nodes_Pair_t> _words;
    std::vector<aggregated_transitions_ptr_t> _dfaTransitions;

    virtual ~AbstrLexerWordConstructor()
    {
        //...
    }
public:
    void _initWords()
    {
        //the first in container is a pair with lexer_dfa accessible
        //the second in container is a reminder for us to delete DfaTransitions when we're done

        auto aKeyword_REPS_withNamedIterationWord = _constructKeyword_REPS_withNamedIteration();
        _words.push_back(aKeyword_REPS_withNamedIterationWord.first);
        _dfaTransitions.push_back(aKeyword_REPS_withNamedIterationWord.second);

        auto aSquareBracketRepsWord = _constructSquareBracketReps();
        _words.push_back(aSquareBracketRepsWord.first);
        _dfaTransitions.push_back(aSquareBracketRepsWord.second);

        auto aAlterationAndJoinWord = _constructAlterationAndJoin();
        _words.push_back(aAlterationAndJoinWord.first);
        _dfaTransitions.push_back(aAlterationAndJoinWord.second);

        auto aAlterationWord = _constructAlteration();
        _words.push_back(aAlterationWord.first);
        _dfaTransitions.push_back(aAlterationWord.second);

        auto aRecursiveAlterationWord = _constructRecursiveAlteration();
        _words.push_back(aRecursiveAlterationWord.first);
        _dfaTransitions.push_back(aRecursiveAlterationWord.second);

        auto aKeyword_evalWord = _constructKeyword_eval();
        _words.push_back(aKeyword_evalWord.first);
        _dfaTransitions.push_back(aKeyword_evalWord.second);

        auto aEndWord = _constructEnd();
        _words.push_back(aEndWord.first);
        _dfaTransitions.push_back(aEndWord.second);

        auto aKeyword_REPS_withNamedListIterationWord = _constructKeyword_REPS_withNamedListIteration();
        _words.push_back(aKeyword_REPS_withNamedListIterationWord.first);
        _dfaTransitions.push_back(aKeyword_REPS_withNamedListIterationWord.second);
    }

    virtual wordrepr_and_transition_Pair_t _constructKeyword_REPS_withNamedIteration() = 0;
    virtual wordrepr_and_transition_Pair_t _constructSquareBracketReps() = 0;
    virtual wordrepr_and_transition_Pair_t _constructAlterationAndJoin() = 0;
    virtual wordrepr_and_transition_Pair_t _constructAlteration() = 0;
    virtual wordrepr_and_transition_Pair_t _constructRecursiveAlteration() = 0;
    virtual wordrepr_and_transition_Pair_t _constructKeyword_eval() = 0;
    virtual wordrepr_and_transition_Pair_t _constructEnd() = 0;
    virtual wordrepr_and_transition_Pair_t _constructKeyword_REPS_withNamedListIteration() = 0;

    virtual wordrepr_and_transition_Pair_t __insertNamedListIterationParamsDfa(lexer_dfa_ptr_t fromDfa, lexer_dfa_ptr_t toDfa, unsigned int tentativeNameKey) = 0;
    virtual wordrepr_and_transition_Pair_t __insertNamedIterationParamsDfa(lexer_dfa_ptr_t fromDfa, lexer_dfa_ptr_t toDfa, unsigned int tentativeNameKey) = 0;
    virtual wordrepr_and_transition_Pair_t __insertAndJoinParamsDfa(lexer_dfa_ptr_t fromDfa, lexer_dfa_ptr_t toDfa, unsigned int tentativeNameKey) = 0;
};

#endif
