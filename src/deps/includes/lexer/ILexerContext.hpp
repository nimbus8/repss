/*
 REPSS
 Copyright (C) 2012  Khalique Williams

 This file is part of REPSS.

 REPSS is free software: you can redistribute it and/or modify
 it under the terms of the   GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 REPSS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with REPSS.  If not, see <http://www.gnu.org/licenses/>. */


#ifndef _ILEXER_CONTEXT_
#define _ILEXER_CONTEXT_

#include "../ILexerDataProxy.hpp"
#include "construction/ScanWordNode.hpp"

class ILexerContext
{
protected:
    virtual ~ILexerContext() {}

public:
    virtual void initLexerDataProxy(const ILexerDataProxy* lexerDataProxy) = 0;
    virtual const ILexerDataProxy* getLexerDataProxy() const = 0;

    virtual void initScanWords(const ScanWords* scanWords) = 0; //I think this may be deprecated in favour of ILexerDataProxy
    virtual const ScanWords* getScanWords() const = 0;  //I think this is deprecated in favour of ILexerDataProxy

    virtual void setAnnotatedData(const std::vector<std::string>& data) = 0;

    virtual void printAnnotatedData() const = 0;
};

#endif
