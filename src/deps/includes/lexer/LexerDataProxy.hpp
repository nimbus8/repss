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

#include "../ILexerDataProxy.hpp"

//interface for set of functions allowing seperate classes sharing same context
// to 'speak' with eachother.
class LexerDataProxy : public ILexerDataProxy
{
private:
    const DfaManager* _dfaManager;
    const ScanWords* _recognizedKeywords;
public:
    LexerDataProxy() : _dfaManager(nullptr) {}
    LexerDataProxy(const DfaManager* dfaManager, const ScanWords* recognizedKeywords) : _dfaManager(dfaManager), _recognizedKeywords(recognizedKeywords) {}
    virtual ~LexerDataProxy() {}
    virtual const DfaManager* getDfaManager() const { return _dfaManager; }    
    virtual const ScanWords* getRecognizedKeywords() const { return _recognizedKeywords; }
};

#endif
