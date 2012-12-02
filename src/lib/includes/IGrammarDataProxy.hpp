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

#include "execution_phase/grammaticalForm/GrammarKeywords.hpp"
#include "execution_phase/grammaticalForm/GrammarBlockAggregate.hpp"

#ifndef _IGRAMMAR_DATA_PROXY_
#define _IGRAMMAR_DATA_PROXY_

class IGrammarDataProxy
{
protected:
    virtual ~IGrammarDataProxy() {};
    virtual const std::GrammarKeywords* const getGrammarKeywords() const = 0;
};

#endif
