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
#include "model_representation/dfa_manager.hpp"
#include "lexer_dfa_builder.hpp"
#include "../../utils/AggregatePtrsAndDelete.hpp"

#ifndef _LEXER_WORD_CONSTRUCTOR_
#define _LEXER_WORD_CONSTRUCTOR_

//lexer_word constructor
class lexer_word_constructor
{
private:
        std::vector<std::pair<lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>> _words;
	DfaManager dfaManager;	//dfa manager handles references, create/destroy fn pairs
        lexer_dfa_builder* _lexer_builder;

        std::pair<lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> _constructPercentReps();
	void _destructPercentReps();

        void _initWords();
public:
        lexer_word_constructor()
        {
                _initWords();
        }
        ~lexer_word_constructor() { }

        std::vector<std::pair<lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>>& getWords() { return _words; }
};

#endif
