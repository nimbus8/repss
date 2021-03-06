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

#ifndef _ILEXER_DATA_PROXY_
#define _ILEXER_DATA_PROXY_

//interface for set of functions allowing seperate classes sharing same context
// to 'speak' with eachother.

#include "execution_phase/lexer/construction/model_representation/dfa_manager.hpp"
#include "execution_phase/lexer/construction/ScanWordNode.hpp"

class ILexerDataProxy
{
public:
    virtual ~ILexerDataProxy() {};
    virtual const DfaManager* getDfaManager() const = 0;    
    virtual const ScanWordTransitionMap* getScanWordTransitionMap() const = 0;
    virtual const ScanWords* getRecognizedKeywords() const = 0;
};

#endif
