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

#include "model_representation/finite_autonoma/lexer_dfa.hpp"
#include "lexer_dfa_builder.hpp"

#ifndef _LEXER_WORD_CONSTRUCTOR_
#define _LEXER_WORD_CONSTRUCTOR_

//lexer_word constructor
class lexer_word_constructor
{
private:
        std::vector<lexer_word_repr*> _words;
        lexer_dfa_builder* _lexer_builder;

        lexer_word_repr* _constructPercentReps();

        void _initWords();
public:
        lexer_word_constructor()
        {
                lexer_word_repr* initDfa1 = new lexer_word_repr(13);
                _words.push_back(initDfa1);

                _initWords();
        }
        ~lexer_word_constructor() { }

        std::vector<lexer_word_repr*>& getWords() { return _words; }
};

#endif
