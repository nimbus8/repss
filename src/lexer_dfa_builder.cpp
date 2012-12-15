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

 If you wish to contact the author of REPSS, you may do so at
 kwillia.pub@gmail.com
 */

#include "base/includes/execution_phase/lexer/construction/lexer_dfa_builder.hpp"
#include "base/includes/execution_phase/lexer/construction/model_representation/finite_autonoma/LexerStateAndInput.hpp"

#include <vector>
#include <iostream>

#define DEBUG
//#undef DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

//merges dfas to one dfa for traversal
lexer_word_repr* lexer_dfa_builder::mergeDfas(const std::vector<lexer_word_repr*>* const words, DfaManager& dfaManager) const
{
    lexer_word_repr* start = dfaManager.createLexerWordRepr();

    //So each JOB consists of:
    //  1) a ptr to dfa node where we left off in mergeToWord
    //  2) a transition that we suspect is placeable in mergetToWord
    //  3) a vector of dfa nodes already visited in mergeTo, this prevents folding the fromDfa back "itself"
    //      but in reality there should be at path dinstinguishing mergeFrom from mergeTo that doesn't visit
    //      a node in MergeTo twice. This only makes sense, because the "going back itself" thing is strictly for
    //      kleen closure like behaviour on runtime. The minimal "description" of automata is actually constant 
    //      and doesn't need to have two nodes twice.
    //      repeated.
    auto jobQueue = new std::vector<std::vector<std::tuple<lexer_dfa*, LexerTransition, std::vector<lexer_dfa*>>>*>();

    for (int i = 0; i < words->size(); i++)
    {
        auto jobVector = new std::vector<std::tuple<lexer_dfa*, LexerTransition, std::vector<lexer_dfa*>>>();
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

        DeLOG(std::string(" -- Is word null?? ").append((mergeFromDfaPtr == nullptr? "true\n" : "false\n")).c_str());

        std::vector<LexerTransition> nextTransitions = mergeFromDfaPtr->getTransitions();

        DeLOG("Getting transitions for word\n")

        for (auto transition : nextTransitions)
        {
            DeLOG("Adding job to job Queue\n");

            std::tuple<lexer_dfa*, LexerTransition, std::vector<lexer_dfa*>> job(mergeToDfaPtr, transition, std::vector<lexer_dfa*>{});
            (jobQueue->at(jobLineIndex))->push_back(job);
        }

        jobLineIndex++;
    }

    DeLOG(std::string("Merge Process: ").append(std::to_string(jobQueue->size())).append(" job lines(queues) total").c_str());

    for (jobLineIndex = 0; jobLineIndex < jobQueue->size(); jobLineIndex++)
    {
        DeLOG(std::string("Processing Job line #").append(std::to_string(jobLineIndex+1)).c_str());
        auto jobVector = jobQueue->at(jobLineIndex);
        while (jobVector->size() != 0)
        {
            DeLOG(std::string("There are ").append(std::to_string(jobVector->size())).append(" in job vector. Processing Job #").append(std::to_string(jobLineIndex + 1)).c_str());

            auto currJobTuple = jobVector->back();
            jobVector->pop_back();

            auto currMergeToDfaPtr = std::get<0>(currJobTuple);

            //here we'll check the currMergeToDfaPtr against the previously visited ptrs in MergeTo
            //if we've already visited it, we know to ignore it this transition (and NOT to put job back in queue)
            auto ptrsInMergeToAlreadyVisited = std::get<2>(currJobTuple);
            auto skipAndContinue = false;
            for (auto mergeToDfaPtrVisited : ptrsInMergeToAlreadyVisited)
            {
                if (currMergeToDfaPtr == mergeToDfaPtrVisited)
                {
                    skipAndContinue = true;
                    break;
                }
            }

            if (skipAndContinue)
            {
                continue;
            }

            ptrsInMergeToAlreadyVisited.push_back(currMergeToDfaPtr);

            auto transitionFromCurrMergeFromDfaPtr = std::get<1>(currJobTuple);            
            auto nextMergeFromDfaPtr = transitionFromCurrMergeFromDfaPtr.getDfaNode();
            const auto si = transitionFromCurrMergeFromDfaPtr.getStateAndInput();

            auto nextMergeToDfaPtr = (currMergeToDfaPtr != nullptr? currMergeToDfaPtr->getNextDfaForInput(si.getInput()) : nullptr);
          
            currMergeToDfaPtr->_printTransitions();
            std::cout << "is there nextDfaPtr? " << (currMergeToDfaPtr->getNextDfaForInput(si.getInput()) != nullptr ? "yes" : "no")
                      << nextMergeToDfaPtr << std::endl;

            //I'm assuming this means if the toDfaPtr is null, we can insert there
            if (nextMergeToDfaPtr == nullptr)
            {
                    LexerStateAndInput aLexerStateAndInput = transitionFromCurrMergeFromDfaPtr.getStateAndInput();
                    std::cout << "::adding transition([" << aLexerStateAndInput.getState() << ", '"
                              << aLexerStateAndInput.getInput() << "']->" << nextMergeFromDfaPtr->getId() << ") to dfa("
                              << currMergeToDfaPtr->getId() << ")" <<  std::endl;
                    StateAndInput<int,char> aStateAndInput(aLexerStateAndInput.getState(), aLexerStateAndInput.getInput(), transitionFromCurrMergeFromDfaPtr.getIsRanged());
                    currMergeToDfaPtr->add_next_dfa(aStateAndInput, nextMergeFromDfaPtr);
            }
            else if (nextMergeFromDfaPtr != nullptr)
            {
                    std::vector<LexerTransition> nextTransitions = nextMergeFromDfaPtr->getTransitions();

                    for (auto transitionFromNextMergeFromDfa : nextTransitions)
                    {
                        const auto si = transitionFromNextMergeFromDfa.getStateAndInput();
                        std::cout << "Couldn't find an opening, pushing back job { to add (" << si.getState() << "," << si.getInput()  << ") from dfa-id(" << nextMergeToDfaPtr->getId() << ") " << std::endl;

                        std::tuple<lexer_dfa*, LexerTransition, std::vector<lexer_dfa*>> job(const_cast<lexer_dfa*>(nextMergeToDfaPtr), transitionFromNextMergeFromDfa, ptrsInMergeToAlreadyVisited);
                        jobVector->push_back(job);
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

#undef DEBUG
#undef DeLOG
