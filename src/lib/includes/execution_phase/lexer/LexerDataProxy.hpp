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

#ifndef _LEXER_DATA_PROXY_
#define _LEXER_DATA_PROXY_

#include "../../ILexerDataProxy.hpp"

//interface for set of functions allowing seperate classes sharing same context
// to 'speak' with eachother.
class LexerDataProxy : public ILexerDataProxy
{
private:
    const DfaManager* _dfaManager;
    const ScanWordTransitionMap* _scanWordTransitionMap;
    const ScanWords* _recognizedKeywords;
public:
    LexerDataProxy() : _dfaManager(nullptr), _scanWordTransitionMap(nullptr), _recognizedKeywords(nullptr) {}
    LexerDataProxy(const DfaManager* dfaManager, const ScanWordTransitionMap* scanWordTransitionMap, const ScanWords* recognizedKeywords) : _dfaManager(dfaManager), _scanWordTransitionMap(scanWordTransitionMap), _recognizedKeywords(recognizedKeywords) {}

    virtual ~LexerDataProxy();

    virtual const DfaManager* getDfaManager() const;
    virtual const ScanWordTransitionMap* getScanWordTransitionMap() const; 
    virtual const ScanWords* getRecognizedKeywords() const;
};

LexerDataProxy::~LexerDataProxy()
{
}

const DfaManager* LexerDataProxy::getDfaManager() const
{
    std::cout << "getDfaManager()" << std::endl;
    return _dfaManager;
}

const ScanWordTransitionMap* LexerDataProxy::getScanWordTransitionMap() const
{
    std::cout << "getScanWordTransitionMap()" << std::endl;
    return _scanWordTransitionMap;
}

const ScanWords* LexerDataProxy::getRecognizedKeywords() const
{
    std::cout << "getRecognizedKeywords()" << std::endl;
    return _recognizedKeywords;
}

#endif
