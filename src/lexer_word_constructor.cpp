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

#include "includes/lexer/lexer_man/lexer_builder/lexer_word_constructor.hpp"

#include "includes/lexer/lexer_man/lexer_builder/model_representation/finite_autonoma/states/StateAndInput.hpp"
#include "includes/lexer/lexer_man/lexer_builder/model_representation/finite_autonoma/DfaTransition.hpp"

lexer_word_repr* lexer_word_constructor::_constructPercentReps()
{
        lexer_word_repr* word_base = new lexer_word_repr(0);

        //we need to address sharing, if we're going to or not - if so how.
        lexer_dfa* A = new lexer_dfa(1);
        lexer_dfa* B = new lexer_dfa(2);

        //todo: we need to make objects that keep track of state for those closure expressions
        StateAndInput<int,char> stateInput1(0,'%');
        StateAndInput<int,char> stateInput2(0,'r');
        DfaTransition* idfa1 = CreateDfaTransition(&stateInput1,A);
        DfaTransition* idfa2 = CreateDfaTransition(&stateInput2,B);

        //we must replace the var list in this case with something else - like a set or vector.
        ApplyImmutableFunc<DfaTransition*> applyObj(2,idfa1, idfa2);

        bool wasSuccess =  _lexer_builder->addDfa(word_base, applyObj);
	if (wasSuccess)
	{
		std::cout << std::endl << "Successfully contructed Reps Dfa" << std::endl;
	}

        return word_base;
}

void lexer_word_constructor::_initWords()
{
        lexer_word_repr* percentReps = _constructPercentReps();
        _words.push_back(percentReps);
}

