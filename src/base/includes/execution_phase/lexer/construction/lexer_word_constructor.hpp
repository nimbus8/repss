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

#include "model_representation/dfa_manager.hpp"

#include "ConstructionTypedefs.hpp"

#include "lexer_dfa_builder.hpp"
#include "ScanWordNode.hpp"
#include "ScanWordTransitionMap.hpp"

#include "../../../utils/AggregatePtrsAndDelete.hpp"
#include "../Genrtd_AbstrLexerWordConstructor.hpp"

#ifndef _LEXER_WORD_CONSTRUCTOR_
#define _LEXER_WORD_CONSTRUCTOR_

#define DEBUG
//#undef DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
    #define DLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str)
#else
    #define DeLOG(str)
    #define DLOG(str)
#endif

//lexer_word constructor
class lexer_word_constructor : public AbstrLexerWordConstructor
{
private:
    DfaManager dfaManager;      //dfa manager handles references, create/destroy fn pairs

    //second phase of construction - merging of aggregated language components
    lexer_word_repr* _startWordForMergedRepr;

    //third/final phase of construction - run time optimization / (maybe todo: phase 1 & 2 cleanup)
    std::pair<ScanWords*, AggregatePtrsAndDelete<ScanWordNode*>*>* _scanWordStartAndPtrAggregation; 
    ScanWordTransitionMap* _scanWordTransitionMap;
    ScanWords* _scanWords;
  
    //intermediate functions for first phase (req. virtual fn's in AbstrLexerWordConstructor)

    virtual wordrepr_and_transition_Pair_t _constructPercentReps();
    virtual wordrepr_and_transition_Pair_t _constructSquareBracketReps();
    virtual wordrepr_and_transition_Pair_t _constructEnd();
    virtual wordrepr_and_transition_Pair_t _constructKeyword_REPS_withNamedIteration();
    virtual wordrepr_and_transition_Pair_t _constructKeyword_REPS_withNamedListIteration();
    virtual wordrepr_and_transition_Pair_t _constructKeyword_eval();
    virtual wordrepr_and_transition_Pair_t _constructAlteration();
    virtual wordrepr_and_transition_Pair_t _constructAlterationAndJoin();
    virtual wordrepr_and_transition_Pair_t _constructRecursiveAlteration();

    virtual wordrepr_and_transition_Pair_t __insertNamedIterationParamsDfa(lexer_dfa* fromDfa, lexer_dfa* toDfa, unsigned int tentativeNameKey);
    virtual wordrepr_and_transition_Pair_t __insertNamedListIterationParamsDfa(lexer_dfa* fromDfa, lexer_dfa* toDfa, const unsigned int tentativeNameKey);
    virtual wordrepr_and_transition_Pair_t __insertAndJoinParamsDfa(lexer_dfa_ptr_t fromDfa, lexer_dfa_ptr_t toDfa, unsigned int tentativeNameKey);

    void _destructDfasAndTransitions();

    //intermediate functions for second phase
    bool _testMergedRepresentation();

    //intermediate functions for third/final phase
    bool _testScanWords();

    //internal interface functions for each phase

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

        delete dfaWords;

        bool resultOfMergedDataTest = _testMergedRepresentation();
 
        DeLOG(std::string{"Merge Test "}.append(resultOfMergedDataTest? "SUCCEEDED!\n" : "FAILED!!\n").c_str());
    
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
        delete _scanWordTransitionMap;
        delete _scanWordStartAndPtrAggregation->second;
        delete _scanWordStartAndPtrAggregation;
        DeLOG("Successfully Deleted ScanWords!\n");

        DeLOG("Sucessfully deleted lexer word constructor!\n");
    }

    const ScanWordTransitionMap* getScanWordTransitionMap() const
    {
        return _scanWordTransitionMap;
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

#undef DEBUG
#undef DLOG
#undef DeLOG

#endif
