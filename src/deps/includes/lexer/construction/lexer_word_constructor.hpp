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

#include "model_representation/finite_autonoma/lexer_dfa.hpp"
#include "model_representation/dfa_manager.hpp"
#include "lexer_dfa_builder.hpp"
#include "ScanWordNode.hpp"

#include "../../utils/AggregatePtrsAndDelete.hpp"

#ifndef _LEXER_WORD_CONSTRUCTOR_
#define _LEXER_WORD_CONSTRUCTOR_

typedef std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> wordrepr_and_transition_Pair_t;

//lexer_word constructor
class lexer_word_constructor
{
private:
    DfaManager dfaManager;      //dfa manager handles references, create/destroy fn pairs

    //first phase of construction - language component definition & aggregation
    std::vector<std::pair<lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>> _words;
    std::vector<AggregatePtrsAndDelete<DfaTransition*>*> _dfaTransitions;

    //second phase of construction - merging of aggregated language components
    lexer_word_repr* _startWordForMergedRepr;

    //third/final phase of construction - run time optimization / (maybe todo: phase 1 & 2 cleanup)
    ScanWords* _scanWords;
  
    typedef std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> wordrepr_and_transition_Pair_t;

    //intermediate functions for first phase
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> _constructPercentReps();
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> _constructSquareBracketReps();

    wordrepr_and_transition_Pair_t __insertNamedRepitionParamsDfa(lexer_dfa* fromDfa, lexer_dfa* toDfa, unsigned int tentativeNameKey);
    wordrepr_and_transition_Pair_t _constructEnd();
    wordrepr_and_transition_Pair_t _constructKeyword_REPS_WithNamedIteration();
    wordrepr_and_transition_Pair_t _constructAlteration();
    wordrepr_and_transition_Pair_t _constructAlterationAndJoin();

    void _destructDfasAndTransitions();

    //intermediate functions for second phase
    bool _testMergedRepresentation();

    //intermediate functions for third/final phase
    bool _testScanWords();

    //internal interface functions for each phase
    void _initWords();
    bool _mergeWords()
    {
        std::vector<lexer_dfa*>* dfaWords = new std::vector<lexer_dfa*>();

        for (auto aWordTuple : _words)
        {
            std::cout << "Pushing back dfa to mergeDfas parameter: dfa(" << aWordTuple.first->getId() << ")" << std::endl;
            dfaWords->push_back(aWordTuple.first);
        }

        const lexer_dfa_builder lexerBuilder;
        _startWordForMergedRepr = lexerBuilder.mergeDfas(dfaWords, dfaManager);

        bool resultOfMergedDataTest = _testMergedRepresentation();
 
        std::cout << "Merge Test " << (resultOfMergedDataTest? "SUCCEEDED!" : "FAILED!!") << std::endl << std::endl;
    
        return resultOfMergedDataTest;
    }
    bool _constructScanWords();
public:
    lexer_word_constructor()
    {
        _initWords();
        _mergeWords();
        _constructScanWords();
    }

    ~lexer_word_constructor()
    {
        _destructDfasAndTransitions();
        delete  _startWordForMergedRepr;

        std::cout << "Sucessfully deleted lexer word constructor!" << std::endl;
    }

    const ScanWords* getScanWords() const
    {
        return _scanWords;
    }

    const DfaManager* getDfaManager() const
    {
        const DfaManager* retDfaManager = &dfaManager;
        return retDfaManager;
    }

    const std::vector<std::pair<lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>>& getWords() 
    { 
        return _words;
    }
};

#endif
