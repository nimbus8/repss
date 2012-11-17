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

#define DEBUG
//#undef DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

class PhasedExecution
{
private:
    lexer_configuration* _config;

    //lexing
    void executePhase1(ILexerContext* lexerContext);

    //parsing -- note: we're skipping this for now (hard n fast, throwing caution to the wind, etc.)

    //grammar aggregation
    void executePhase2(IGrammarContext* grammarContext);

    //analysis & tree/level construction
    void executePhase3();

    //generation -- summary: keyword elimination
    void executePhase3();
public:
    PhasedExecution()
    {
        DeLog("PhasedExecution::PhasedExecution()\n");

        //...this entire function needs rework

        ContextManager contextManager;
        const lexer_configuration config;

        auto lexerContext = contextManager.getContext<ContextType::AllowedTypes, ContextType::Lexer>();
        const lexer_manager lexMan(&lexerContext,&config);

        DeLOG("past lexer manager creation.\n");

        if (argc < 4)
        {
            std::cerr << "Error, correct usage:  repss str_cmp1 str_cmp2 input_file" << std::endl;
        }

        const string filename{argv[3]};
        const string permissions{"rt"};

        Scanner *scanner = new Scanner(&lexerContext);

        scanner->processFile(filename, permissions);

    }
    void execute();
};

#undef DEBUG
#undef DeLOG

#endif
