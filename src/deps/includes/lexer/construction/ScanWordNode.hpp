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

#ifndef _SCAN_WORD_NODE_
#define _SCAN_WORD_NODE_

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "model_representation/finite_autonoma/lexer_dfa.hpp"
#include "ScanWordTransitionMap.hpp"


#ifdef DEBUG
     #ifndef DLOG(str)
         #define DLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str)
     #endif
#else
    #define DLOG(str)
#endif

//Scan Word is simply meant to be a simpler interface of lexer_dfa. 
//most importantly, its a bit of a runtime optimization: 
//  the nextScanWordNode(...) call runs in ~ O(1), whereas nextDfa(...) runs in ~ O(n)
class ScanWordNode
{
private:
    class InputHashFunction {
    public:
        ::std::size_t operator ()(const char &input) const
        {
            return (size_t) input;
        }
    };

    class InputEquals {
    public:
        bool operator ()(const char &lhs, const char &rhs) const
        {
            return lhs == rhs;
        }
    };

    unsigned int _id; //ok so you may ask why have id, when its the same in _lexerDfa? well, we may want to delete lexer_dfa, and use this class as preferred representation (thats the plan). if we move the init into constructor, we wouldn't even need to hold ont to reference to lexer_dfa.
    const lexer_dfa* _lexerDfa;

    bool _hasAnythingButTransition;
    bool _hasRangedTransition;

    std::function<bool (unsigned int, char)> __fnIsInputWithinRange; //helper func from lexerDfa
    ScanWordNode* _getNextScanWordNodeOrNullForRangedInput(const char rangedInput) const //composite helper
    {
        //todo:will this seems almost silly, revisit -- i think its just so we can iterate...maybe we can put this somewhere else...
        //revisited, oct 13 - this could be a good(easy...) way to order evaluation - i.e. order more specigic before more general - which is pretty important as our algorithm is somewhat 'greedy'(probably horrible wording)
        char rangedSI_categories[] {SI_CHARS_LOWER, SI_CHARS_UPPER, SI_CHARS_ANY, SI_NUMBERS_0, SI_NUMBERS_1to9, SI_NUMBERS_0to9,SI_EMPTY};

        ScanWordNode* mappedNode = nullptr;

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

        for (size_t i = 0; i < NUMBER_OF_RANGED_SI_CATEGORIES; i++)
        {
            //simulating 'mapping'
            const auto rangedCategory = rangedSI_categories[i];
            //std::cout << "\tcalling _fnIsInputWithinRange("
            //          << rangedCategory << ", " << rangedInput << ")"
            //          << std::endl;

            if (__fnIsInputWithinRange(rangedCategory, rangedInput))
            {
                const auto rangedCategoryIndex = rangedSIToIndex(rangedCategory); 
                //mappedNode = _rangedTransitionsByCategory[rangedCategoryIndex];

                if (mappedNode != nullptr)
                {
                    break;
                }
            }
        }        

        return mappedNode;
    }
public:
    ScanWordNode() { _id = 2^(32)-1; _lexerDfa = nullptr; _hasAnythingButTransition = false; _hasRangedTransition = false; }
    explicit ScanWordNode(const lexer_dfa* lexerDfa)
    {
        _id = lexerDfa->getId();
        _lexerDfa = lexerDfa;

        _hasAnythingButTransition = false;
        _hasRangedTransition = false;
    }

    void init(ScanWordTransitionMap* const transitionMap, const std::unordered_set<ScanWordNode*>& existingScanWordNodes, std::vector<ScanWordNode*>& wordsToBeInitd);

    void _printTransitions() const
    {
        //to be implemented
    }

    unsigned int getId() const
    {
        return _id;
    }

    const ScanWordNode* getNextScanWordNode(const ScanWordTransitionMap* const transitionMap, const char input) const
    {
        //_printTransitions();

        const ScanWordNode* ret = nullptr;

        //1) check for ranged

        const auto theIdInKey = getId();

        if (!_hasRangedTransition && !_hasAnythingButTransition)
        {
            //std::cout << "\t::Trying normal way" <<std::endl;
            //finally, if all else fails - try normal case
            const TransitionInputKey transitionMapKey(theIdInKey, input, false, false, false);
            ret = transitionMap->getNextScanWordNode(transitionMapKey);

            return ret;
        }
        else
        {
            //std::cout << "\t::Trying Ranged" << std::endl;
            const TransitionInputKey transitionMapKey(theIdInKey, input, true, false, false);
            ret = transitionMap->getNextScanWordNode(transitionMapKey);

            if (ret != nullptr) return ret; //is this anything?

            if (_hasAnythingButTransition)
            {
                //std::cout << "\t::Trying Ranged and HasAnythingBut" << std::endl;
                const TransitionInputKey transitionMapKey2(theIdInKey, input, true, true, false);
                ret = transitionMap->getNextScanWordNode(transitionMapKey2);

                if (ret != nullptr) return ret;
            }

            //std::cout << "\t::Trying normal way" <<std::endl;
            //finally, if all else fails - try normal case
            const TransitionInputKey transitionMapKey3(theIdInKey, input, false, false, false);
            ret = transitionMap->getNextScanWordNode(transitionMapKey3);
        }
            
/*
        if (ret == nullptr && _hasRangedTransition && _hasAnythingButTransition)
        {
            //std::cout << "\t::Trying Ranged and HasAnythingBut" << std::endl;
            const TransitionInputKey transitionMapKey(theIdInKey, input, true, true, false);
            ret = transitionMap->getNextScanWordNode(transitionMapKey);
        }

        if (ret == nullptr && _hasAnythingButTransition)
        {
            //std::cout << "\t::Trying hasAnythingbut" << std::endl;
           //iunno this is to say it has an anything but, but this wasn't covered w/ the rnge
            const TransitionInputKey transitionMapKey(theIdInKey, input, false, true, false);
            ret = transitionMap->getNextScanWordNode(transitionMapKey); 
        }

        if (ret == nullptr)
        {
            //std::cout << "\t::Trying normal way" <<std::endl;
            //finally, if all else fails - try normal case
            const TransitionInputKey transitionMapKey(theIdInKey, input, false, false, false);
            ret = transitionMap->getNextScanWordNode(transitionMapKey);
        }
*/

        return ret;
    }
};

typedef ScanWordNode ScanWords;

#endif
