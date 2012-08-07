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
#include <unordered_map>

#include "../lexer_builder/lexer_word_constructor.hpp"

//-- Configuration --//

#ifndef _LEXER_CONFIGURATION_
#define _LEXER_CONFIGURATION_

class lexer_configuration
{
private:
        lexer_word_constructor* _wordConstructor;
public:
        lexer_configuration() { _wordConstructor = new lexer_word_constructor(); }
        ~lexer_configuration() {}

        void init() {}

        std::vector<lexer_word_repr*>& getLexicalWords() { return (_wordConstructor->getWords()); }
};

#endif
