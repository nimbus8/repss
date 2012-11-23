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

#include "lib/includes/PhasedExecution.hpp"

#include "lib/includes/execution_phase/lexer/lexer_manager.hpp"
#include "lib/includes/execution_phase/lexer/Scanner.hpp"

/*#undef*/ #define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

PhasedExecution::PhasedExecution()
        : _contextManager(nullptr)
{
    DeLOG("PhasedExecution::PhasedExecution()\n");
}

std::string PhasedExecution::execute(int argc, char* argv[])
{
    if (_contextManager != nullptr)
    {
        DeLOG("Destroying Old Context For Another Run.");
        delete _contextManager;
    }

    if (argc < 4)
    {
        perror("Not enough arguments, correct usage:  repss str_cmp1 str_cmp2 input_file");

        return "";
    }

    _contextManager = new ContextManager();

    auto lexerContext = _contextManager->getContext<ContextType::AllowedTypes, ContextType::Lexer>();

    std::string inputFileName{argv[3]};
    runLexer(&lexerContext, inputFileName);  


    delete _contextManager;

    std::string generatedOutput;
    return generatedOutput;
}

void PhasedExecution::runLexer(ILexerContext* const lexerContext, std::string inputFileName)
{
    lexer_manager lexerMan(lexerContext);
    lexerMan.init(_config);

    Scanner scanner(lexerContext);
    scanner.processFile(inputFileName, std::string{"rt"});

    //todo: when lexer_man goes out of scope, it cleans of LexerDatProxy
    //       so its this routines job to return whatever is necessary for
    //       parser or grmammer phase...
}

#undef DEBUG
#undef DeLOG
