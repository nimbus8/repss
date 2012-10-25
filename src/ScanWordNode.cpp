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

#include "deps/includes/lexer/construction/ScanWordNode.hpp"

//this is a bit tricky, haven't thought about it too deeply. (something tells me this isn't trivial)
//-we create a ScanWordNode with a lexerDfa reference that we hold on to.
//-this is basically only because we want to have a list of ScanWordNode objects we
//-create implicitly as a consequence of creating a ScanWordNode explicitly.
//-so as it goes, outside this class, we'll maintain a list of implicitly/explicitly created
//-ScanWordNodes, so we know not to create one (with existing id) explicitly or even
//-implicitly (hence vector param in init()) - if one already exists we just use the existing reference.
//maybe we can actually, move all this init stuff into constructor make ScanWordNode const??
void ScanWordNode::init(ScanWordTransitionMap* const transitionMap, const std::unordered_set<ScanWordNode*>& existingScanWordNodes, std::vector<ScanWordNode*>& wordsToBeInitd)
{
    if (_lexerDfa == nullptr)
    {
        std::cout << "API Misuse: Either init has already been called, or nullptr was given to ScanWordNode constructor. Both bad." << std::endl;
        exit(1);
    }

    //note: To handle case where two transitions point to same lexer dfa ( say with different
    //       inputs, eg: partially ranged input (a|b|c)->some_lexer_dfa ) we have locallyCreatedScanWordNodes.
    //note: This is not so important now, but should we ever want to support accepting partially
    //       ranged input (construction is already possible) [todo:will] we may want to test to see     
    //       if what we're doing here is acceptable. Maybe we need to check the code for the merge process as well.
    std::vector<ScanWordNode*> locallyCreatedScanWordNodes;

    _id = _lexerDfa->getId();
    auto transitions = _lexerDfa->getTransitions();

    _lexerDfa->_printTransitions();

    for (auto aTransition : transitions)
    {
        const auto stateAndInput = aTransition.getStateAndInput();
        const auto input = stateAndInput.getInput();

        //1) check if the transition points to lexer_dfa for which a ScanWordNode already exists (has same id)

        const auto nextDfa = aTransition.getDfaNode();
        const auto nextDfaId = nextDfa->getId();

        ScanWordNode* nextScanWordNode = nullptr;

        //check locally created scan words first
        for (auto existingScanWordNode : locallyCreatedScanWordNodes)
        {
            if (existingScanWordNode->getId() == nextDfaId)
            {
                nextScanWordNode = existingScanWordNode;
                break;
            }
        }

        //if scan word node was already created locally, we can skip ahead
        if (nextScanWordNode == nullptr)
        {
            //if transition already exists in unordered_set param, use it
            for (auto existingScanWordNode : existingScanWordNodes)
            {
                if (existingScanWordNode->getId() == nextDfaId)
                {
                    nextScanWordNode = existingScanWordNode;
                    break;
                }
            }

            //if it exists it neither unordered_set param and not in locallyCreatedScanWordNodes container, we make it
            if (nextScanWordNode == nullptr)
            {//if no corresponding ScanWordNode already exists in vector param, create it, place in toBeInitd
                nextScanWordNode = new ScanWordNode(nextDfa);
                wordsToBeInitd.push_back(nextScanWordNode); //the calling function should take care of transferring elements in wordsToBeInitd to existingScanWordNodes
                locallyCreatedScanWordNodes.push_back(nextScanWordNode); //to handle case where two transitions point to same lexer dfa (say with different inputs)
            }
        }

        //2) at this point nextScanWordNode is guaranteed to be set to something (not nullptr)

        //we need to check to see if input is ranged for this state (stateAndInput). If so we MUST accomodate for this
        //begining: oct 11, 2012

        //todo: make this a private method, and use (maybe) in _getNext...RangedInput(..) to map category to index       
        std::function<size_t (char)> rangedSIToIndex = 
            [](char rangedSItype)->size_t
            {
                size_t index;

                switch (rangedSItype)
                {
                    case SI_EMPTY:
                        index = 0;
                        break;
                    case SI_CHARS_LOWER:
                        index = 1;
                        break;
                    case SI_CHARS_UPPER:
                        index = 2;
                        break;
                    case SI_CHARS_ANY:
                        index = 3;
                        break;
                    case SI_NUMBERS_0:
                        index = 4;
                        break;
                    case SI_NUMBERS_1to9:
                        index = 5;
                        break;
                    case SI_NUMBERS_0to9:
                        index = 6;
                        break;
                    default:
                        index = 7; //should never happen, but if so lets have it blow up
                        break;
                };

                return index;
            };

        //todo:will remember we have to check for isAnythingBut and then handle that case
 
        auto isRangedTransition = aTransition.getIsRanged();
        auto isAnythingBut = aTransition.getIsAnythingBut();        

        if (isRangedTransition)
        {
            std::cout << "\tisRangedTransition=true" << std::endl;

            //for ranged transitions we have a special data structure in place (an array :o)
            //btw: rangedSItoIndex is fn internal to ScanWord
            auto rangedInputCategory = stateAndInput.getInput();

            char rangedPossibilities[] = {SI_CHARS_LOWER, SI_CHARS_UPPER, SI_CHARS_ANY, SI_NUMBERS_0, SI_NUMBERS_1to9, SI_NUMBERS_0to9, SI_EMPTY};

            for (auto possibility : rangedPossibilities)
            {
                bool shouldAddToTransitionMap;
                switch (possibility)
                {
                    case SI_EMPTY:
                        shouldAddToTransitionMap = false; //revisit this. keep false. maybe treat in same spirit as _anythingBut
                        break;
                    case SI_CHARS_LOWER:
                        shouldAddToTransitionMap = islower(rangedInputCategory);
                        break;
                    case SI_CHARS_UPPER:
                        shouldAddToTransitionMap = isupper(rangedInputCategory);
                        break;
                    case SI_CHARS_ANY:
                        shouldAddToTransitionMap = isalpha(rangedInputCategory);
                        break;
                    case SI_NUMBERS_0:
                        shouldAddToTransitionMap = rangedInputCategory == '0';
                        break;
                    case SI_NUMBERS_1to9:
                        shouldAddToTransitionMap = (rangedInputCategory >= '1' && rangedInputCategory <= '9');
                        break;
                    case SI_NUMBERS_0to9:
                        shouldAddToTransitionMap = (rangedInputCategory >= '1' && rangedInputCategory <= '9');
                        break;
                    default:
                        shouldAddToTransitionMap = false;
                        break;
                };

                if (shouldAddToTransitionMap)
                {
                    TransitionInputKey transitionMapKey(getId(), possibility, true, false, true);
                    std::pair<TransitionInputKey, ScanWordNode*> transitionMapKeyAndValue{ transitionMapKey, nextScanWordNode };
                    transitionMap->emplace(transitionMapKeyAndValue);

                    _hasRangedTransition = true;
                }
            }

            std::cout << "\tSuccessfully set value for index in _RangedTransitionsByCategory" << std::endl;
        }
        else
        {
            std::cout << "\tisRangedTransition=false" << std::endl;

            TransitionInputKey transitionMapKey(getId(), input, false, false, true);
            std::pair<TransitionInputKey, ScanWordNode*> transitionMapKeyAndValue{ transitionMapKey, nextScanWordNode };
            transitionMap->emplace(transitionMapKeyAndValue);
        }
    }

    _lexerDfa = nullptr; //we don't need lexerDfa anymore. todo: after scanwords are made lexerDfas not needed at all
}

