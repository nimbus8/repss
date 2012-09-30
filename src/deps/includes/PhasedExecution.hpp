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

class PhasedExecution
{
private:
    lexer_configuration* _config;

    //lexing
    void executePhase1()
    {

    }

    void executePhase2()
    {

    }
public:
    PhasedExecution()
    {
        ContextManager contextManager;
        const lexer_configuration config;

        auto lexerContext = contextManager.getContext<ContextType::AllowedTypes, ContextType::Lexer>();
        const lexer_manager lexMan(&lexerContext,&config);

        DLOG("past lexer manager creation.\n");

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
