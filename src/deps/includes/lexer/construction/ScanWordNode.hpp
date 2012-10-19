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

    int _id; //ok so you may ask why have id, when its the same in _lexerDfa? well, we may want to delete lexer_dfa, and use this class as preferred representation (thats the plan). if we move the init into constructor, we wouldn't even need to hold ont to reference to lexer_dfa.
    const lexer_dfa* _lexerDfa;

    //maps *unranged* input to a ScanWordNode
    std::unordered_map<char, ScanWordNode*, InputHashFunction, InputEquals> _nextScanWordNode;

    //same as _nextScanWordNode but for ranged input 
    ScanWordNode* _rangedTransitionsByCategory[NUMBER_OF_RANGED_SI_CATEGORIES];    
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
                mappedNode = _rangedTransitionsByCategory[rangedCategoryIndex];

                if (mappedNode != nullptr)
                {
                    break;
                }
            }
        }        

        return mappedNode;
    }
public:
    ScanWordNode() { _id = -1; _lexerDfa = nullptr; }
    explicit ScanWordNode(const lexer_dfa* lexerDfa)
    {
        _id = lexerDfa->getId();
        _lexerDfa = lexerDfa;

        for (size_t i = 0; i < NUMBER_OF_RANGED_SI_CATEGORIES; i++)
        {
            _rangedTransitionsByCategory[i] = nullptr;
        }

        __fnIsInputWithinRange = lexerDfa->getFnIsInputWithinRange();
    }

    void init(const std::unordered_set<ScanWordNode*>& existingScanWordNodes, std::vector<ScanWordNode*>& wordsToBeInitd);

    void _printTransitions() const
    {
        //start with unranged
        InputHashFunction hashFunc;
        for (auto iter : _nextScanWordNode)
        {
            //auto inputKey = iter.first;
            //auto scanWordPtr = iter.second;
            //std::cout << "\t(<" << inputKey << ", ranged?(no)>, "
                //<< "id(" << scanWordPtr->getId() << "))"
                //<< "\t- hash(" << hashFunc(inputKey)  << ")" << std::endl; //commented in order to benchmark diff between ScanWords
        }

        //print ranged
        for (size_t i = 0; i < NUMBER_OF_RANGED_SI_CATEGORIES; i++)
        {
            const auto scanWordPtr = _rangedTransitionsByCategory[i];

            if (scanWordPtr != nullptr)
            {
                //const auto inputKey = i;
                //std::cout << "\t(<" << inputKey << ", ranged?(yes)>, "
                    //<< "id(" << scanWordPtr->getId() << "))" << std::endl; //commented in order to benchmark diff between ScanWords
            }
        }
    }

    int getId() const
    {
        return _id;
    }

    ScanWordNode* getNextScanWordNode(const char input) const
    {
        //_printTransitions();

        ScanWordNode* ret;

        //1) check for ranged

        //Problem CONFIRMED! oct 13 2012 early morning
        //oct 13: it seems like any speed bump I was hoping for is negated by linear search through ranged states,
        //  and it looks like its now consistently (slightly) slower than the merged dfa impl...which defeats the purpose(or point w.e)
        //..starting to think that a a global table(unordered map with composite keys*) is best/easiest soln. 
        //Beter than 'heavy' unordered_maps for EVERY scanword - scanword was supposed to be lightweight alternative to lexer_dfa (why even bother? we can realy prdict how large and differentiated language will be...still thinking about it)
        //[Details:*composite key would compose of : { id, input } and {id, rangedCategory} for unranged and ranged respectively - with an appropriate hash function (no collisions)).
        //.....actually this change wouldn't be so difficult to implement, mostly architecture considerations 
        //todo:will create a ScanWordProgressionDataMan, give all created scanword(s) a reference to it (maybe take advntage of friend relationships or something...figure it out) or make it a singleton]

        ret = _getNextScanWordNodeOrNullForRangedInput(input);

        if (ret == nullptr)
        {//2) check for unranged
            auto fetched = _nextScanWordNode.find(input);

            if (fetched == _nextScanWordNode.end())
            {
                ret = nullptr;
            }
            else
            {
                ret = fetched->second;
            }
        }

        return ret;
    }
};

typedef ScanWordNode ScanWords;

#endif
