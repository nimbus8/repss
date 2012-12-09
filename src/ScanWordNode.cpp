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

#include "lib/includes/execution_phase/lexer/construction/ScanWordNode.hpp"

#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

//Outside this class (lexer_word_constructor right now), we maintain a list of implicitly/explicitly created
// ScanWordNodes, this ensures we know not to create a ScanWordNode with an existing id explicitly
// if we children are needed for an object we only create it if there doesn't already exist a reference for it -
// this node created here or elsewhere need not have been init'd yet.
//     tothinkabout:will 'maybe' we can actually move all this init stuff into constructor make ScanWordNode const??
void ScanWordNode::init(ScanWordTransitionMap* const transitionMap, const std::unordered_set<ScanWordNode*>& existingScanWordNodes, std::vector<ScanWordNode*>& wordsToBeInitd)
{
    if (_lexerDfa == nullptr)
    {
        DeLOG("Error - API Misuse: Either init has already been called, or nullptr was given to ScanWordNode constructor. Both bad.\n");
        exit(1);
    }

    //To handle case where 2 or more transitions point to same lexer dfa we have locallCreatedScanWordNodes.
    // eg: 'partially ranged' input (a|b|c)->some_lexer_dfa [1]
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

        const auto isRangedTransition = aTransition.getIsRanged();
        const auto isAnythingBut = aTransition.getIsAnythingBut();        

        if (isRangedTransition)
        {
            DeLOG("\tisRangedTransition=true\n");

            char rangedPossibilities[] = {SI_CHARS_LOWER, SI_CHARS_UPPER, SI_CHARS_ANY, SI_NUMBERS_0, SI_NUMBERS_1to9, SI_NUMBERS_0to9, SI_EMPTY};

            auto rangedInputCategory = stateAndInput.getInput();

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
                    if (!isAnythingBut)
                    {
                        TransitionInputKey transitionMapKeyRanged(getId(), possibility, true, false, true);
                        std::pair<TransitionInputKey, ScanWordNode*> transitionMapKeyAndValue{ transitionMapKeyRanged, nextScanWordNode };
                        transitionMap->emplace(transitionMapKeyAndValue);

                        addProperty(_properties, ScanWordProperties_t::SCAN_WORD_PROPERTY_HAS_RANGED_TRANSITION);                             }
                    else
                    {
                        if (_anythingButTransition != nullptr)
                        {
                            DeLOG("Ooops, somehow we managed to define two 'anythingBut' transitions, this almost certainly leads to undefined behaviour. Quitting.\n");
                            exit(1);
                        }

                        TransitionInputKey transitionKeyAnythingButRange(getId(), possibility, true, true, false);
                        std::pair<TransitionInputKey, ScanWordNode*>* transitionKeyAndValue = new std::pair<TransitionInputKey, ScanWordNode*>(transitionKeyAnythingButRange, nextScanWordNode);
                        _anythingButTransition = transitionKeyAndValue;

                        addProperty(_properties, ScanWordProperties_t::SCAN_WORD_PROPERTY_HAS_ANYTHING_BUT_TRANSITION);
                    }
                }
            }

            DeLOG("\tSuccessfully set value for index in _RangedTransitionsByCategory\n");
        }
        else if (isAnythingBut)
        {
            if (_anythingButTransition != nullptr)
            {
                DeLOG("Ooops, somehow we managed to define two 'anythingBut' transitions, this almost certainly leads to undefined behaviour. Quitting.\n");
                exit(1);
            }

            TransitionInputKey transitionKeyAnythingButUnranged(getId(), input, false, true, false);
            std::pair<TransitionInputKey, ScanWordNode*>* transitionKeyAndValue = new std::pair<TransitionInputKey, ScanWordNode*>(transitionKeyAnythingButUnranged, nextScanWordNode);
            _anythingButTransition = transitionKeyAndValue;

            addProperty(_properties, ScanWordProperties_t::SCAN_WORD_PROPERTY_HAS_ANYTHING_BUT_TRANSITION);
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

#undef DEBUG
#undef DeLOG

//Footnotes: 
//[1] This is not so important now, but should we ever want to support accepting partially
// ranged input (construction is already possible) we may want to test to see
// if what we're doing here is acceptable. Maybe we need to check the code for the merge process as well.
//    Update: This seems to work. But I noticed we introduced an unchecked source of error in merge process.
//             if actual ranged inputs from two different keywords (up that point identical) overlap, there's serious
//             ambguity.  I actually DON'T want this to be a concession! [todo:will] figure out if we can support
//             this sort of overlapping 
//                 (1) my current thinking is that on merge we can accept ambiguity but at runtime we'll allow for the 
//                     recognition of multiple 'possible' languages at a time asynchronously? (so long as they are all 
//                     equal up to that point) diverting a hard decision to some point FURTHER down the line.
//                     As long as the two language constructs aren't entirely ambiguous this would work: just make sure
//                     not go crazy and define like 10 lengthy strands all equal 'cept for the last couple inputs.
//                     This has particular ramifications for 'alternation' and 'alternationAndJoin'.
//                 (2) the other option would be to completely rework the tentative naming system. i.e. have it internally,
//                     represent branches, and let the actual name be runtime dependant. (the more I think about it,
//                     the more I think that's what I was going for at the beginning...AND it MIGHT actually be
//                     TRIVIAL to implement....almost trivial. Well the addition to language construction would be:
//                       something like newTentativeNameKey = addBranchName(tentativeNameKey, name, dfa-id? ...), 
//                         then use newTentativeNameKey for granchildren of branch (no change here needed) etc etc. 
//                           -> This will cut down on code! ...and kindof fits into our goal schema!
//                     then on the tentativeNameManagement side, every cal to addBranchName(...)
                   
