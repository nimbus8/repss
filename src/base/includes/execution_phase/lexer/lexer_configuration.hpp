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

#include <iostream>
#include <vector>
#include <string>

#include <stdarg.h>
#include <utility>
#include <functional>
#include <unordered_map>

#include "construction/lexer_word_constructor.hpp"
#include "construction/ScanWordNode.hpp"

//-- Configuration --//

#ifndef _LEXER_CONFIGURATION_
#define _LEXER_CONFIGURATION_

#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:  %s\n", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

class lexer_configuration
{
private:
    lexer_word_constructor* _wordConstructor;
public:
    lexer_configuration() { _wordConstructor = new lexer_word_constructor(); }
    ~lexer_configuration() 
    {
        delete _wordConstructor;
        DeLOG("Successfully Deleted Lexer Configuration\n");   
    }

    void init() {}

    const ScanWordTransitionMap* getScanWordTransitionMap() const
    {
        return _wordConstructor->getScanWordTransitionMap();
    }

    const ScanWords* getScanWords() const
    {
       return _wordConstructor->getScanWords();
    }

    const DfaManager* const getDfaManager() const
    {
        return _wordConstructor->getDfaManager();
    }

    const std::vector<std::pair<lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>>& getLexicalWords() const
    { 
        return _wordConstructor->getWords();
    }
};

#undef DEBUG
#undef DeLOG
#endif
