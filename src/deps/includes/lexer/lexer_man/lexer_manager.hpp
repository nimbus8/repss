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

#include "lexer_config/lexer_configuration.hpp"

#ifndef _LEX_MANAGER_
#define _LEX_MANAGER_

class lexer_manager
{
private:
        const lexer_configuration* _config;

        const lexer_word_repr* _current_dfa;
        const std::vector<std::pair<lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>>* _lexer_words;

        void mergeDfas();
public:
        lexer_manager(const lexer_configuration* config)
	{
		_config = config;
		_lexer_words = &(config->getLexicalWords());
		std::cout << "Lexer_Word_Count " << _lexer_words->size() << std::endl;  
	}
        ~lexer_manager() {}
};

#endif
