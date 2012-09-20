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

#include "lexer_config/lexer_configuration.hpp"

//todo:will change this to LexerContext - find where it defined
#include "../ContextManager.hpp"

//todo:will put context manager in here...
class lexer_manager
{
private:
        ContextManager::TypedContext<ContextType::AllowedTypes, ContextType::Lexer>* _context; //todo: typedef this
        const lexer_configuration* _config;

        void mergeDfas();
public:
        lexer_manager(ContextManager::TypedContext<ContextType::AllowedTypes, ContextType::Lexer>* context, const lexer_configuration* config)
        {
            _context = context;
            _config = config;

            auto scanWords = config->getScanWords();
            std::cout << "LexerManager:: First ScanWordNode (id: " << scanWords->getId() << ")" << std::endl;    
            //start scanning here
            //...
	}
        ~lexer_manager() {}
};

#endif
