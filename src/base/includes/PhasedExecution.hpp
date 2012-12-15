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

#ifndef _PHASED_EXECUTION_
#define _PHASED_EXECUTION_

#include <string>

#include "ContextManager.hpp"
#include "Genrtd_Keywords.hpp"

#include "execution_phase/lexer/ILexerContext.hpp"
#include "execution_phase/lexer/lexer_configuration.hpp"
#include "execution_phase/grammaticalForm/IGrammarContext.hpp"
#include "execution_phase/grammaticalForm/GrammarConfig.hpp"

#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:  %s\n", __FILE__, __LINE__, str);
    #define DeNEWLINE() printf("\n");
#else
    #define DeLOG(str)
    #define DeNEWLINE()
#endif

class PhasedExecution
{
private:
    GrammarConfig _grammarConfig;
    lexer_configuration _lexerConfig;
    ContextManager* _contextManager;
public:
    PhasedExecution();
    ~PhasedExecution()
    {
        if (_contextManager != nullptr) 
        {
            delete _contextManager;
        }

        DeLOG("Successfully Deleted PhasedExecution\n");
    }

    //todo:will return tuple of status and string -- so handle exceptions inside here too
    std::string execute(int argc, char* argv[]);

    //lexing
    void runLexer(ILexerContext* const lexerContext, std::string inputFileName);

    //parsing -- note: we're skipping this for now (hard n fast, throwing caution to the wind, etc.)
    void runParser();

    //grammar aggregation
    void runGrammarAggregation(IGrammarContext* const grammarContext);

    //analysis & tree/level construction
    void runAnalysis();

    //generation -- summary: keyword elimination
    std::string runGeneration();
};

#undef DEBUG
#undef DeLOG

#endif
