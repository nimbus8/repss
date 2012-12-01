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

 If you wish to contact the author of REPSS, you may do so at
 kwillia.pub@gmail.com
 */

#include "lib/includes/execution_phase/grammaticalForm/GrammarBlockAggregator.hpp"

//this is where the main processing of annotate data occurs

void GrammarBlockAggregator::run(IGrammarContext* const context)
{
    auto annotatedData = context->getAnnotatedData();

    size_t sizeOfAnnotatedData = annotatedData.size();

    for (size_t index = 0; index < sizeOfAnnotatedData; index++)
    {
        auto dataString = annotatedData.getAt(index);

        //parse string for type
        // - it can be either a keyword or data
        //    data can be recognized as just data
        //    anything else is considered a keyword.

        //if data push it to the current Block.

        //if keyword, analyze it.
        // 'analysis' consists of, is this expected? if not ignore it.
        //   Also if it's a Variable we add a new Block as the newest child
        //   to this current line of block.
        //   If it's a variable then we have to be on the lookout for a
        //   corresponding 'closure' to end the current Block.
        //    So we must make note of this somehow.
        //   Everything else (terminals) are added to the current block
        //    or line(if not within a block). The system as a whole can be
        //    viewed as a block I guess.

        //put gramarDataBlock into lexerGrammarProxy 
    }

    auto grammarDataProxy = context->getGrammarDataProxy();
    context->setGrammarBlockAggregate(static_cast<GrammarBlockAggregate&&>(_aggregatedBlocks));
}

/*
    virtual ConstVector<std::string> getAnnotatedData() const = 0;

    virtual void initGrammarDataProxy(const IGrammarDataProxy* grammarDataProxy) = 0;
    virtual const IGrammarDataProxy* getGrammarDataProxy() const = 0;

    virtual void setGrammarBlockAggregate(const GrammarBlockAggregate* grammarBlockAggregate) = 0;
    virtual void printGrammarBlockAggregateData() const = 0;
*/
