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

enum class ScanWordProperties_t : unsigned int
{
    SCAN_WORD_PROPERTY_NOTHING				= 0x00,
    SCAN_WORD_PROPERTY_HAS_RANGED_TRANSITION 		= 0x02,
    SCAN_WORD_PROPERTY_HAS_ANYTHING_BUT_TRANSITION	= 0x04
   // = 0x8, 0x10, 0x20, 0x80, 0x100, 0x200, 0x400, 0x800 ...
};

inline void addProperty(unsigned int &propertyContainer, ScanWordProperties_t propertyToAdd)
{
    unsigned int propertyToAddUINT = static_cast<unsigned int>(propertyToAdd);
    if (!(propertyContainer & propertyToAddUINT))
    {
        propertyContainer ^= propertyToAddUINT;

        if (!(propertyContainer & propertyToAddUINT))
        {
            std::cout << "add property failed!!! oops!" << std::endl;
            exit(1);
        }
    }
} 

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
    unsigned int _id;
    const lexer_dfa* _lexerDfa;

    unsigned int _properties;

    std::pair<TransitionInputKey, ScanWordNode*>* _anythingButTransition; //lets new this the moment we know how much we need
public:
    ScanWordNode()
    {
        _id = 0xFFFFFFFF;
        _lexerDfa = nullptr;

        _properties = static_cast<unsigned int>(ScanWordProperties_t::SCAN_WORD_PROPERTY_NOTHING);

        _anythingButTransition = nullptr;
    }

    explicit ScanWordNode(const lexer_dfa* lexerDfa)
    {
        _id = lexerDfa->getId();
        _lexerDfa = lexerDfa;

        _properties = static_cast<unsigned int>(ScanWordProperties_t::SCAN_WORD_PROPERTY_NOTHING);

        _anythingButTransition = nullptr;
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
//      SCAN_WORD_PROPERTY_NOTHING                          = 0x00,
//      SCAN_WORD_PROPERTY_HAS_RANGED_TRANSITION            = 0x02,
//      SCAN_WORD_PROPERTY_HAS_ANYTHING_BUT_TRANSITION      = 0x04

        //_printTransitions();

        const ScanWordNode* ret = nullptr;

        const auto theIdInKey = getId();

        //case 1: are there NO ranged OR anythignBut transitions? If so we needn't test for them
        if ( _properties == static_cast<unsigned int>(ScanWordProperties_t::SCAN_WORD_PROPERTY_NOTHING))
        {
            std::cout << "\t::Trying normal way" <<std::endl;
            //finally, if all else fails - try normal case
            const TransitionInputKey transitionMapKey(theIdInKey, input, false, false, false);
            ret = transitionMap->getNextScanWordNode(transitionMapKey);

            return ret;
        }
        else 
        {
            //case 2: we know we have ranged transitions and NO anythingButTransitions, just check for the former
            //first and then for the unranged equivalent after that
            if ((_properties & (static_cast<unsigned int>(ScanWordProperties_t::SCAN_WORD_PROPERTY_HAS_RANGED_TRANSITION))) 
              && !(_properties & static_cast<unsigned int>(ScanWordProperties_t::SCAN_WORD_PROPERTY_HAS_ANYTHING_BUT_TRANSITION)))  
            {
                std::cout << "\t::Trying Ranged" << std::endl;
                const TransitionInputKey transitionMapKeyRanged(theIdInKey, input, true, false, false);
                ret = transitionMap->getNextScanWordNode(transitionMapKeyRanged);

                //trying normal - the rule here is that normal transitions & ranged take precedence over anything but
                //and we should probably do some checking at lower levels so transitions don't condlict.
                if (ret == nullptr)
                {
                    const TransitionInputKey transitionMapKeyNormal(theIdInKey, input, false, false, false);
                    ret = transitionMap->getNextScanWordNode(transitionMapKeyNormal);
                }
            }
            else if ((_properties & (static_cast<unsigned int>(ScanWordProperties_t::SCAN_WORD_PROPERTY_HAS_RANGED_TRANSITION)))
                   && (_properties & static_cast<unsigned int>(ScanWordProperties_t::SCAN_WORD_PROPERTY_HAS_ANYTHING_BUT_TRANSITION)))
            {
                //case 3: we know there are ranged transtions AND anything but transitions
                //         we must test for both individual and/or in combination.
                const TransitionInputKey transitionMapKeyRanged(theIdInKey, input, true, false, false);
                ret = transitionMap->getNextScanWordNode(transitionMapKeyRanged);

                //if no hit for ranged try normal case, before anythingBut
                if (ret == nullptr)
                {
                    const TransitionInputKey transitionMapKey(theIdInKey, input, false, false, false);
                    ret = transitionMap->getNextScanWordNode(transitionMapKey);
                }

                if (ret == nullptr)
                {
                    const auto inputAndType = _anythingButTransition->first;

                    if (inputAndType.getIsRanged())
                    {
                        const auto anyThingButCharacterRange = inputAndType.getInputCharacter();
                        
                        //todo:will a switch on the ranges possible
                    }
                    else
                    {
                        if (input != inputAndType.getInputCharacter())
                        {
                            ret = _anythingButTransition->second;
                        }
                    }
                }
            }
            else if(_properties & static_cast<unsigned int>(ScanWordProperties_t::SCAN_WORD_PROPERTY_HAS_ANYTHING_BUT_TRANSITION))
            {
                //case 4: we know that there ARE'NT any ranged transitions, but there are anythingBut transitions
                //         we need test for the normal(unranged) case first, THEN the anythingBut case (if norm fail)
                const TransitionInputKey transitionMapKey(theIdInKey, input, false, false, false);
                ret = transitionMap->getNextScanWordNode(transitionMapKey);

                if (ret == nullptr)
                {
                    const auto inputAndType = _anythingButTransition->first;

                    if (input != inputAndType.getInputCharacter())
                    {
                        ret = _anythingButTransition->second;
                    }
                }
            }
        }
            
        return ret;
    }
};

typedef ScanWordNode ScanWords;

#endif
