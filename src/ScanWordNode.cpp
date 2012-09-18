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

#include "deps/includes/lexer/lexer_man/lexer_builder/ScanWordNode.hpp"


    //todo:will note, not liking how we modify existingScanWordNodes, make const and let the calling function manage
    //transferring the difference at the end of this call to existingScanWordNodes.
    void ScanWordNode::init(std::vector<ScanWordNode*>& existingScanWordNodes, std::vector<ScanWordNode*>& wordsToBeInitd)
    {
        if (_lexerDfa == nullptr)
        {
            std::cout << "API Misuse: Either init has already been called, or nullptr was given to ScanWordNode constructor. Both bad." << std::endl;
            exit(1);
        }

        _id = _lexerDfa->getId();
        auto transitions = _lexerDfa->getTransitions();

        for (auto aTransition : transitions)
        {
            auto stateAndInput = aTransition.getStateAndInput();
            auto input = stateAndInput.getInput();

            //check if the transition points to lexer_dfa for which a ScanWordNode already exists (has same id)

            auto nextDfa = aTransition.getDfaNode();
            auto nextDfaId = nextDfa->getId();

            ScanWordNode* nextScanWordNode = nullptr;
            for (auto existingScanWord : existingScanWordNodes)
            {
                if (existingScanWord->getId() == nextDfaId)
                {
                    nextScanWordNode = existingScanWord;
                    break;
                }
            }

            //if transition already exists in vector param, use it

            if (nextScanWordNode == nullptr)
            {//if no corresponding ScanWordNode already exists in vector param, create it, place in toBeInitd
                nextScanWordNode = new ScanWordNode(nextDfa);
                wordsToBeInitd.push_back(nextScanWordNode);
                existingScanWordNodes.push_back(nextScanWordNode);
            }

            std::pair<char, ScanWordNode*> inputToScanWordNode{input, nextScanWordNode};
            _nextScanWordNode.emplace(inputToScanWordNode);
        }

        _lexerDfa = nullptr; //we don't need lexerDfa anymore. todo: after scanwords are made lexerDfas not needed at all
    }
