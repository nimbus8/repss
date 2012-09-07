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

#include "deps/includes/lexer/lexer_man/lexer_builder/lexer_dfa_builder.hpp"
#include "deps/includes/lexer/LexerStateAndInput.hpp"

//merges dfas to one dfa for traversal
lexer_word_repr* lexer_dfa_builder::mergeDfas(std::vector<lexer_word_repr*>& words, DfaManager& dfaManager)
{
    lexer_word_repr* start = dfaManager.createLexerWordRepr();

    std::vector<std::vector<std::pair<lexer_dfa*, LexerTransition>>> jobQueue(words.size());

    for (int i = 0; i < words.size(); i++)
    {
        std::vector<std::pair<lexer_dfa*, LexerTransition>> jobVector;
        jobQueue.push_back(jobVector);
    }

    //iterate through words
    int jobLineIndex = 0;
    for (auto word : words)
    {
        lexer_dfa* mergeToDfaPtr = start;
        lexer_dfa* mergeFromDfaPtr = word;

        std::vector<LexerTransition> nextTransitions = mergeFromDfaPtr->getTransitions();

        for (auto transition : nextTransitions)
        {
            const auto si = transition.getStateAndInput();
            const LexerStateAndInput stateAndInput(si.getState(), si.getInput());
            
            std::pair<lexer_dfa*, LexerTransition> job(mergeToDfaPtr, transition);
            jobQueue.at(jobLineIndex).push_back(job);
        }

        jobLineIndex++;
    }

    for (jobLineIndex = 0; jobLineIndex < jobQueue.size(); jobLineIndex++)
    {
        auto jobVector = jobQueue.at(jobLineIndex);
        while (jobVector.size() != 0)
        {
            auto currJobPair = jobVector.back();
            jobVector.pop_back();

            auto currMergeToDfaPtr = currJobPair.first;

            auto transitionFromCurrMergeFromDfaPtr = currJobPair.second;            
            auto nextMergeFromDfaPtr = transitionFromCurrMergeFromDfaPtr.getDfaNode();

            const auto si = transitionFromCurrMergeFromDfaPtr.getStateAndInput();
            const LexerStateAndInput stateAndInput(si.getState(), si.getInput());
            auto nextMergeToDfaPtr = currMergeToDfaPtr->getNextDfa(stateAndInput);
          
            if (nextMergeFromDfaPtr != nullptr)
            {
                if (nextMergeToDfaPtr == nullptr)
                {
                    LexerStateAndInput aLexerStateAndInput = transitionFromCurrMergeFromDfaPtr.getStateAndInput();
                    StateAndInput<int,char> aStateAndInput(aLexerStateAndInput.getState(), aLexerStateAndInput.getInput(), transitionFromCurrMergeFromDfaPtr.getIsRanged());
                    nextMergeToDfaPtr->add_next_dfa(aStateAndInput, nextMergeFromDfaPtr);
                }
                else
                {
                    std::vector<LexerTransition> nextTransitions = nextMergeFromDfaPtr->getTransitions();

                    for (auto transitionFromNextMergeFromDfa : nextTransitions)
                    {
                        const auto si = transitionFromNextMergeFromDfa.getStateAndInput();
                        const LexerStateAndInput stateAndInput(si.getState(), si.getInput());

                        auto nextMergeToDfaPtr = currMergeToDfaPtr->getNextDfa(stateAndInput);
        
                        std::pair<lexer_dfa*, LexerTransition> job(const_cast<lexer_dfa*>(nextMergeToDfaPtr), transitionFromNextMergeFromDfa);
                        jobVector.push_back(job);
                    }
                }
            }
            else
            {
                std::cout << "Undefined language specification: duplicate lexer words?" << std::endl;
                exit(1);
            }
        }
    }

    return start;
}
