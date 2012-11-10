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

#ifndef _GRAMMAR_BLOCK_AGGREGATOR_
#define _GRAMMAR_BLOCK_AGGREGATOR_

//In this file we assume that everything is in proper order (i.e. parsed) and go data through methodically

class GrammarBlockAggregate : virtual public IGrammarBlockAggregate
{
public:
    GrammarBlockAggregate() {}
    virtual ~GrammarBlockAggregate() {}

    //this should only be called once: in the 'analysis' context
    virtual const void exportToVectorAsReadOnlyElements(std::vector<ReadOnlyElement*> readOnlyElements) const = 0;
};

class GrammarBlockAggregator
{
private:
    GrammarBlockAggregate _aggregatedBlocks;
public:
    GrammarBlockAggregator() {}
    ~GrammarBlockAggregator() {}
};

#endf
