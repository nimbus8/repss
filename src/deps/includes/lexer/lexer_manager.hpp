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

#include <iostream>
#include <vector>
#include <string>

#include <stdarg.h>
#include <utility>
#include <functional>

#ifndef _LEX_MANAGER_
#define _LEX_MANAGER_

#define DEBUG
//#undef DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
    #define DLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str)
#else
    #define DeLOG(str)
    #define DLOG(str)
#endif

#include "lexer_configuration.hpp"
#include "LexerDataProxy.hpp"

#include "ILexerContext.hpp"

//todo:will put context manager in here...
class lexer_manager
{
private:
        ILexerContext* _context;
        const lexer_configuration* _config;
        const LexerDataProxy* _lexerDataProxy;
        void mergeDfas();
public:
        lexer_manager(ILexerContext* context, const lexer_configuration* config)
        {
            _context = context;
            _config = config;

            auto scanWordTransitionMap = config->getScanWordTransitionMap();
            auto scanWords = config->getScanWords();
            std::cout << "LexerManager:: First ScanWordNode (id: " << scanWords->getId() << ")" << std::endl;    
            
            auto dfaManager = config->getDfaManager();

            _lexerDataProxy = new LexerDataProxy(dfaManager, scanWordTransitionMap, scanWords);
            DLOG("Initializing lexer data proxy\n");

            context->initLexerDataProxy((const ILexerDataProxy*)_lexerDataProxy);
            DLOG("initialized lexer data proxy successfully\n");

            DLOG(" finished intializing lexer_manager\n");
	}

        ~lexer_manager() {}
};

#undef DEBUG
#undef DLOG
#undef DeLOG

#endif
