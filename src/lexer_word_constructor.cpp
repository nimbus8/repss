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

//todo: should be moved to lexer_word_constructor.hpp file
//EMPTY CHAR AKA the ANY char, doesn't move the chains just accepts everything as input
#define EMPTY_CHAR '\0'
#define ST_ACCEPT 1001

void debug_printDfa(lexer_word_repr* word)
{
	int count = 0;
	int state = 0;
	char seq[] = { '%', 'r', 'q', 'p','s', ' ', 'H' };
	const size_t seq_length = 7;

	StateAndInput<int,char> si0(state++, seq[count++]);
	lexer_dfa* curr = word;
	lexer_dfa* nextDfa = curr->getNextDfa(si0);
	while (nextDfa->getId() != ST_ACCEPT && count < seq_length)
	{
		std::cout << seq[count-1] << " - DfaNode(" << curr->getId() << ")" << std::endl;
		curr = nextDfa;
		StateAndInput<int,char> si(state,seq[count]);
		nextDfa = curr->getNextDfa(si);

		//todo: in lexer_dfa the empty string or 'ANY CHAR' will almost always* be fetched as nullptr
		//since the value is conceptual will never exist as input, this is handled so that it returns a valid dfa here (with
		//the appropriate id) - BUT right now there is no way to know if this particular DfaTranstion (state + the empty string)
		//was ever encountered. IF we ever decide to use this concept we'd need to know not to increase the pos in text.

		//somewhat related to the above: when we encounter a nullptr, this signals error
		//do not increase the pos in text. At this point we need to search for something like a space(' ').
		if (nextDfa == nullptr) break;

		state++; count++;
	}


	if (nextDfa == nullptr)
	{
		std::cout << "nothing" << std::endl;
	}
	else if (nextDfa->getId() == ST_ACCEPT)
	{
		std::cout << "found word!" << std::endl;
	}
	else if (count >= seq_length)
	{
		std::cout << "reached end of input" << std::endl;
	}
}

std::pair<lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> lexer_word_constructor::_constructPercentReps()
{
	const int ST_ERROR = -1;
	const int ST_BASE = 0, ST_A = 1, ST_B = 2, ST_C = 3, ST_D = 4, ST_E = 5;
	const int ST_F = 6;

        lexer_word_repr* word_base = dfaManager.createLexerWordRepr(ST_BASE);

        //we need to address sharing, if we're going to or not - if so how.
        lexer_dfa* A = dfaManager.createDfa(ST_A);		//new lexer_dfa(1);
        auto B = dfaManager.createDfa(ST_B);			//new lexer_dfa(2);
	auto C = dfaManager.createDfa(ST_C);
	auto D = dfaManager.createDfa(ST_D);
	auto E = dfaManager.createDfa(ST_E);
	auto F = dfaManager.createDfa(ST_ACCEPT);	

        //todo: we need to make objects that keep track of state for those closure expressions
        StateAndInput<int,char> stateInput1(ST_BASE,'%');
        StateAndInput<int,char> stateInput2(ST_A,'r');
	StateAndInput<int,char> stateInput3(ST_B,'e');
	StateAndInput<int,char> stateInput4(ST_C,'p');
	StateAndInput<int,char> stateInput5(ST_D,'s');
	StateAndInput<int,char> stateInput6(ST_E,' ');
	StateAndInput<int,char> stateInput7(ST_ACCEPT, EMPTY_CHAR);

	std::cout << "Checkpoint 1" << std::endl;

        DfaTransition* idfa1 = dfaManager.createDfaTransition(&stateInput1,A);
        DfaTransition* idfa2 = dfaManager.createDfaTransition(&stateInput2,B);
	DfaTransition* idfa3 = dfaManager.createDfaTransition(&stateInput3,C);
        DfaTransition* idfa4 = dfaManager.createDfaTransition(&stateInput4,D);
        DfaTransition* idfa5 = dfaManager.createDfaTransition(&stateInput5,E);
        DfaTransition* idfa6 = dfaManager.createDfaTransition(&stateInput6,F);
	

	//what was before	
        //ApplyImmutableFunc<DfaTransition*> applyObj(2,idfa1, idfa2);
        ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
        
        std::cout << "Checkpoint 2A" << std::endl;

	bool wasSuccess =  _lexer_builder->addDfa(word_base, applyObj);

	std::cout << "Checkpoint 2" << std::endl;

	ApplyImmutableFunc<DfaTransition*> applyObjAtoB(1,idfa2);
	bool wasSuccess2 = _lexer_builder->addDfa(A, applyObjAtoB);
	
        ApplyImmutableFunc<DfaTransition*> applyObjBtoC(1,idfa3);
        bool wasSuccess3 = _lexer_builder->addDfa(B, applyObjBtoC);

	ApplyImmutableFunc<DfaTransition*> applyObjCtoD(1,idfa4);
        bool wasSuccess4 = _lexer_builder->addDfa(C, applyObjCtoD);

        ApplyImmutableFunc<DfaTransition*> applyObjDtoE(1,idfa5);
        bool wasSuccess5 = _lexer_builder->addDfa(D, applyObjDtoE);

        ApplyImmutableFunc<DfaTransition*> applyObjEtoF(1,idfa6);
	bool wasSuccess6 = _lexer_builder->addDfa(E, applyObjEtoF);
 

	if (wasSuccess)
	{
		std::cout << std::endl << "Successfully contructed Reps Dfa: Base To A";
	}

        if (wasSuccess2)
        {
                std::cout << std::endl << "Successfully contructed Reps Dfa: A To B";
        }

        if (wasSuccess3)
        {
                std::cout << std::endl << "Successfully contructed Reps Dfa: B To C" << std::endl;
        }

        if (wasSuccess4)
        {
                std::cout << std::endl << "Successfully contructed Reps Dfa: C To D" << std::endl;
        }

        if (wasSuccess5)
        {
                std::cout << std::endl << "Successfully contructed Reps Dfa: D To E" << std::endl;
        }

        if (wasSuccess6)
        {
                std::cout << std::endl << "Successfully contructed Reps Dfa: E To F" << std::endl;
        }
	debug_printDfa(word_base);

        //todo: Aug 12, 2012 - deletion should go through dfaManager instead of using 'delete' in 
	//AggregatePtrsAndDelete. Think of passing func or dfaManager to constructor.

	AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
		new AggregatePtrsAndDelete<lexer_dfa*>(7, word_base,
			A, B, C, D, E, F);

        std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> ret (word_base, aggrAndDel);

        return ret;
}

void lexer_word_constructor::_initWords()
{
        auto percentReps = _constructPercentReps();
        _words.push_back(percentReps);
}

void lexer_word_constructor::_destructPercentReps()
{
	while (_words.size() > 0)
	{
		auto entryPair = _words.back();
		auto aggregateAndDeleteObj = entryPair.second;
	
		//this takes care of all the dfas, including entryPair.first
		aggregateAndDeleteObj->applyDelete();
		delete aggregateAndDeleteObj;
		aggregateAndDeleteObj = nullptr;	
	}
}

