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

#include "deps/includes/lexer/construction/lexer_dfa_builder.hpp"
#include "deps/includes/lexer/construction/model_representation/finite_autonoma/LexerStateAndInput.hpp"

#include <vector>
#include <iostream>

//merges dfas to one dfa for traversal
lexer_word_repr* lexer_dfa_builder::mergeDfas(const std::vector<lexer_word_repr*>* const words, DfaManager& dfaManager) const
{
    lexer_word_repr* start = dfaManager.createLexerWordRepr();

    auto jobQueue = new std::vector<std::vector<std::pair<lexer_dfa*, LexerTransition>>*>();

    for (int i = 0; i < words->size(); i++)
    {
        auto jobVector = new std::vector<std::pair<lexer_dfa*, LexerTransition>>();
        jobQueue->push_back(jobVector);
    }

    std::cout << std::endl << "Merging dfas to one, words size: " << words->size() << std::endl;

    //iterate through words
    int jobLineIndex = 0;
    for (int i =0; i < words->size(); i++)
    {
        lexer_dfa* word = words->at(i);
        std::cout << word << std::endl;
        std::cout << word->getId() << std::endl;
        lexer_dfa* mergeToDfaPtr = start;
        lexer_dfa* mergeFromDfaPtr = word;

        std::cout << "is word null?? " << (mergeFromDfaPtr == nullptr? "true" : "false") << std::endl;
        std::vector<LexerTransition> nextTransitions = mergeFromDfaPtr->getTransitions();

        std::cout << "Getting transitions for word" << std::endl;

        for (auto transition : nextTransitions)
        {
            std::cout << "Adding job to job Queue" << std::endl;

            std::pair<lexer_dfa*, LexerTransition> job(mergeToDfaPtr, transition);
            (jobQueue->at(jobLineIndex))->push_back(job);
        }

        jobLineIndex++;
    }

    std::cout << "Merge Process: " << jobQueue->size() << " jobs total" << std::endl;
    for (jobLineIndex = 0; jobLineIndex < jobQueue->size(); jobLineIndex++)
    {
        std::cout << "Processing Job line #" << jobLineIndex << std::endl;
        auto jobVector = jobQueue->at(jobLineIndex);
        while (jobVector->size() != 0)
        {
            std::cout << "There are " << jobVector->size() << " in job vector. Processing Job #" << jobLineIndex + 1 << std::endl;
            auto currJobPair = jobVector->back();
            jobVector->pop_back();

            auto currMergeToDfaPtr = currJobPair.first;

            auto transitionFromCurrMergeFromDfaPtr = currJobPair.second;            
            auto nextMergeFromDfaPtr = transitionFromCurrMergeFromDfaPtr.getDfaNode();

            const auto si = transitionFromCurrMergeFromDfaPtr.getStateAndInput();
            auto nextMergeToDfaPtr = (currMergeToDfaPtr != nullptr? currMergeToDfaPtr->getNextDfaForInput(si.getInput()) : nullptr);
          
            currMergeToDfaPtr->_printTransitions();
            std::cout << "is there nextDfaPtr? " << (currMergeToDfaPtr->getNextDfaForInput(si.getInput()) != nullptr ? "yes" : "no")
                      << nextMergeToDfaPtr << std::endl;

            if (nextMergeFromDfaPtr != nullptr)
            {
                if (nextMergeToDfaPtr == nullptr)
                {
                    LexerStateAndInput aLexerStateAndInput = transitionFromCurrMergeFromDfaPtr.getStateAndInput();
                    std::cout << "::adding transition([" << aLexerStateAndInput.getState() << ", '"
                              << aLexerStateAndInput.getInput() << "']->" << nextMergeFromDfaPtr->getId() << ") to dfa("
                              << currMergeToDfaPtr->getId() << ")" <<  std::endl;
                    StateAndInput<int,char> aStateAndInput(aLexerStateAndInput.getState(), aLexerStateAndInput.getInput(), transitionFromCurrMergeFromDfaPtr.getIsRanged());
                    currMergeToDfaPtr->add_next_dfa(aStateAndInput, nextMergeFromDfaPtr);
                }
                else
                {
                    std::vector<LexerTransition> nextTransitions = nextMergeFromDfaPtr->getTransitions();

                    for (auto transitionFromNextMergeFromDfa : nextTransitions)
                    {
                        const auto si = transitionFromNextMergeFromDfa.getStateAndInput();
      
                        std::cout << "Couldn't find an opening, pushing back job" << std::endl;
                        std::pair<lexer_dfa*, LexerTransition> job(const_cast<lexer_dfa*>(nextMergeToDfaPtr), transitionFromNextMergeFromDfa);
                        jobVector->push_back(job);
                    }
                }
            }
            else
            {
                std::cout << "Undefined language specification: duplicate lexer words?" << std::endl;
                exit(1);
            }
        }

        delete jobVector;
    }

    delete jobQueue;

    std::cout << "Finished jobs!" << std::endl << std::endl;

    return start;
}
