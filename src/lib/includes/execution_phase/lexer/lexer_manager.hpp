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

/*#undef*/#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
    #define DLOG(str)
#endif

#include "lexer_configuration.hpp"
#include "LexerDataProxy.hpp"

#include "ILexerContext.hpp"

class lexer_manager
{
private:
    ILexerContext* _context;
    LexerDataProxy* _lexerDataProxy;
public:
    lexer_manager(ILexerContext* context)
            : _context(context), _lexerDataProxy(nullptr) {}

    void init(const lexer_configuration& config)
    {
        if (_lexerDataProxy != nullptr)
        {
            perror("LexerManager/LexerDataProxy have already been initialized. Doing nothing...");
            return;
        }

        auto scanWordTransitionMap = config.getScanWordTransitionMap();
        auto scanWords = config.getScanWords();
        auto dfaManager = config.getDfaManager();

        _lexerDataProxy = new LexerDataProxy(dfaManager, scanWordTransitionMap, scanWords);
        _context->initLexerDataProxy((const ILexerDataProxy*)_lexerDataProxy);

        DeLOG("Initialized Lexer Data Proxy Successfully.\nSuccessfully Intialized LexerManager\n");
    }

    const LexerDataProxy* getLexerDataProxy()
    {
        return _lexerDataProxy;
    }

    ~lexer_manager()
    {
        delete _lexerDataProxy;
    }    
};

/**
 *    LexerManager: 
 *        Main purpose of this class is to (1) initialize LexerDataProxy. This may or may not include grammar rules, to be passed on...
 *
 *        Also since it's the platform upon which we've created LexerDataProxy, it has the responsibility of destroying it.
 *           Important note: This is NOT to destroy ptr's to instance data inside of LexerDataProxy.
 *                            Reason being, we would like to leave the door open to being able to process a number of files
 *                            at a time (and leave yet another door open to processing them each more than once).
 */

#undef DEBUG
#undef DLOG
#undef DeLOG

#endif
