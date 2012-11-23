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

/*#unded*/ #define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

#include <string>

#include "ContextManager.hpp"
#include "Genrtd_Keywords.hpp"

#include "execution_phase/lexer/ILexerContext.hpp"
#include "execution_phase/lexer/lexer_configuration.hpp"

class PhasedExecution
{
private:
    lexer_configuration _config;

    ContextManager* _contextManager;

    //lexing
    void executePhase1(ILexerContext* lexerContext);

    //parsing -- note: we're skipping this for now (hard n fast, throwing caution to the wind, etc.)

    //grammar aggregation
    //void executePhase2(IGrammarContext* grammarContext);

    //analysis & tree/level construction
    void executePhase3();

    //generation -- summary: keyword elimination
    void executePhase4();
public:
    PhasedExecution();
 /*   {
        DeLog("PhasedExecution::PhasedExecution()\n");

        //...this entire function needs rework

        ContextManager contextManager;

        auto lexerContext = contextManager.getContext<ContextType::AllowedTypes, ContextType::Lexer>();
        const lexer_configuration config;
        const lexer_manager lexMan(&lexerContext, config);

        DeLOG("Past lexer manager creation.\n");

        if (argc < 4)
        {
            std::cerr << "Error, correct usage:  repss str_cmp1 str_cmp2 input_file" << std::endl;
        }

        const string filename{argv[3]};
        const string permissions{"rt"};

        Scanner *scanner = new Scanner(&lexerContext);

        scanner->processFile(filename, permissions);

    }*/

    //todo:will return tuple of status and string -- so handle exceptions inside here too
    std::string execute(int argc, char* argv[]);

    void runLexer(ILexerContext* const lexerContext, std::string inputFileName);

};

#undef DEBUG
#undef DeLOG

#endif
