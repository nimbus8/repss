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

    std::unordered_map<char, ScanWordNode*, InputHashFunction, InputEquals> _nextScanWordNode;
public:
    ScanWordNode() { _id = -1; _lexerDfa = nullptr; }
    explicit ScanWordNode(const lexer_dfa* lexerDfa)
    {
        _id = lexerDfa->getId();
        _lexerDfa = lexerDfa;
    }

    //this is a bit tricky, haven't thought about it too deeply. (something tells me this isn't trivial)
    //-we create a ScanWordNode with a lexerDfa reference that we hold on to.
    //-this is basically only because we want to have a list of ScanWordNode objects we
    //-create implicitly as a consequence of creating a ScanWordNode explicitly.
    //-so as it goes, outside this class, we'll maintain a list of implicitly/explicitly created
    //-ScanWordNodes, so we know not to create one (with existing id) explicitly or even 
    //-implicitly (hence vector param in init()) - if one already exists we just use the existing reference.
    //maybe we can actually, move all this init stuff into constructor make ScanWordNode const??
    void init(std::vector<ScanWordNode*>& existingScanWordNodes, std::vector<ScanWordNode*>& wordsToBeInitd);

    int getId() const
    {
        return _id;
    }

    ScanWordNode* getNextScanWordNode(const char input) const
    {
        DLOG("before call to _nextScanWordNode.find(input)\n");
        auto fetched = _nextScanWordNode.find(input);
        DLOG("after call to _nextScanWordNode.find(input)\n");

        ScanWordNode* ret;
        if (fetched == _nextScanWordNode.end())
        {
            ret = nullptr;
        }
        else
        {
            ret = fetched->second;
        }

        DLOG("ScanWordNode::getNextScanWordNode(...) before return\n");

        return ret;
    }
};

typedef ScanWordNode ScanWords;

#endif
