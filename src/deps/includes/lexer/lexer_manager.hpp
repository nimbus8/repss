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

#include "lexer_configuration.hpp"
#include "LexerDataProxy.hpp"

//todo:will change this to LexerContext - find where it defined
#include "ILexerContext.hpp"

#define DEBUG YES

#ifdef DEBUG
    #define DLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str)
#else
    #define DLOG(str)
#endif

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
            //start scanning here
            //...
            //actually with dataproxy construct, scanning doesn't necessarily need to be done here.
            // it could actually just be done in a phased execution engine of some sort - just
            // so that most of important functionality is in the same place.
            // so this 'manager' would jsut be to initialize the dataproxy (I'm gonna admit my ignorance here
            // I have no idea whether we need to (here) keep track lexerdataproxy to allow it to be deleted
            // properly from memory (its liek another part of it is living somewhere else, but can we ensure
            // that the 'substance' is being deleted if we don't delete the derived class(LexerContext)
            // in a context that we KNOW the derived class (here)? - I think this may be where virtual
            // constructors come in...I hope so. (check it out).
            auto dfaManager = config->getDfaManager();

            _lexerDataProxy = new LexerDataProxy(dfaManager, scanWordTransitionMap, scanWords);

            DLOG("Initializing lexer data proxy\n");

            context->initLexerDataProxy((const ILexerDataProxy*)_lexerDataProxy);
//            context->initScanWords(scanWords);

            DLOG("initialized lexer data proxy successfully\n");

            auto dataProxy = context->getLexerDataProxy();

            DLOG("got lexer data proxy from context\n");

            auto recKeys = dataProxy->getRecognizedKeywords();

            auto scanTransMap = dataProxy->getScanWordTransitionMap();

            DLOG(" finished intializing lexer_manager\n");
           
	}

        ~lexer_manager() {}
};

#endif
