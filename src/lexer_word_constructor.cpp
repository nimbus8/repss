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

#include "deps/includes/lexer/lexer_man/lexer_builder/lexer_word_constructor.hpp"
#include "deps/includes/lexer/lexer_man/lexer_builder/model_representation/finite_autonoma/states/StateAndInput.hpp"
#include "deps/includes/lexer/lexer_man/lexer_builder/model_representation/finite_autonoma/DfaTransition.hpp"
#include "deps/includes/lexer/lexer_man/lexer_builder/AggregateDfasAndDelete.hpp"
#include "deps/includes/lexer/lexer_man/lexer_builder/AggregateDfaTransitionsAndDelete.hpp"

//todo: should be moved to lexer_word_constructor.hpp file
//EMPTY CHAR AKA the ANY char, doesn't move the chains just accepts everything as input
#define EMPTY_CHAR '\0'
#define ST_ACCEPT 1001

void debug_printDfa(const lexer_word_repr*  word, char seq[], size_t seq_length)
{
    int count = 0;
	int state = 0;

    auto curr = word;
    
    const lexer_word_repr* nextDfa;
    
    //search for a beginning
    do
    {
	    StateAndInput<int,char> si0(state, seq[count]);
    	std::cout << "(seq,c): " << count << " " << seq[count] << std::endl;
    	count++;
    	auto aNextDfa = curr->getNextDfa(si0);
    	if (aNextDfa != nullptr) 
    	{
    		state++;
    		nextDfa = aNextDfa;
    		break;
    	}
    } while(count < seq_length);
    
    //todo: Check this out. Very strange, without this cout below I get segmentation fault 11! wtf?
    std::cout << "pass the point" << std::endl;
    
    //if a beginning is found, try and match next however many characters with rest of dfa
    while ((nextDfa != nullptr) && (nextDfa->getId() != ST_ACCEPT && count < seq_length))
    {
        std::cout << seq[count-1] << " - DfaNode(" << curr->getId() << ")" << std::endl;

        curr = nextDfa;
        const StateAndInput<int,char> si(state,seq[count]);
        nextDfa = curr->getNextDfa(si);

    	state++;
    	count++;

        if (nextDfa == nullptr) 
        {	
            //reset dfa to keep on looking
            nextDfa = word;
            state = 0;

            //search for a beginning
            	
            do
            {
                StateAndInput<int,char> si0(state, seq[count++]);
                auto aNextDfa = curr->getNextDfa(si0);
                if (aNextDfa != nullptr) 
                {
                    state++;
                    nextDfa = aNextDfa;
                    break;
                }
             } while(count < seq_length);
        }
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

std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> lexer_word_constructor::_constructPercentReps()
{
	//const int ST_ERROR = -1;
	const int ST_BASE = 0, ST_A = 1, ST_B = 2, ST_C = 3, ST_D = 4, ST_E = 5;
	//const int ST_F = 6;

    lexer_word_repr* word_base = dfaManager.createLexerWordRepr(ST_BASE);

    //we need to address sharing, if we're going to or not - if so how.
    lexer_dfa* A = dfaManager.createDfa(ST_A);
    auto B = dfaManager.createDfa(ST_B);
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

    const DfaTransition* idfa1 = dfaManager.createDfaTransition(&stateInput1,A);
    auto idfa2 = dfaManager.createDfaTransition(&stateInput2,B);
	auto idfa3 = dfaManager.createDfaTransition(&stateInput3,C);
    auto idfa4 = dfaManager.createDfaTransition(&stateInput4,D);
    auto idfa5 = dfaManager.createDfaTransition(&stateInput5,E);
    auto idfa6 = dfaManager.createDfaTransition(&stateInput6,F);	

    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
	bool wasSuccess =  _lexer_builder->addDfa(word_base, applyObj);

	const ApplyImmutableFunc<DfaTransition*> applyObjAtoB(1,idfa2);
	bool wasSuccess2 = _lexer_builder->addDfa(A, applyObjAtoB);

    const ApplyImmutableFunc<DfaTransition*> applyObjBtoC(1,idfa3);
    bool wasSuccess3 = _lexer_builder->addDfa(B, applyObjBtoC);

	const ApplyImmutableFunc<DfaTransition*> applyObjCtoD(1,idfa4);
    bool wasSuccess4 = _lexer_builder->addDfa(C, applyObjCtoD);

    const ApplyImmutableFunc<DfaTransition*> applyObjDtoE(1,idfa5);
    bool wasSuccess5 = _lexer_builder->addDfa(D, applyObjDtoE);

    const ApplyImmutableFunc<DfaTransition*> applyObjEtoF(1,idfa6);
	bool wasSuccess6 = _lexer_builder->addDfa(E, applyObjEtoF);
 
	if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6)
	{
		std::cout << std::endl << "Successfully contructed Reps Dfa: Base To A";
        std::cout << std::endl << "Successfully contructed Reps Dfa: A To B";
        std::cout << std::endl << "Successfully contructed Reps Dfa: B To C";
        std::cout << std::endl << "Successfully contructed Reps Dfa: C To D";
    	std::cout << std::endl << "Successfully contructed Reps Dfa: D To E";
        std::cout << std::endl << "Successfully contructed Reps Dfa: E To F";

		std::cout << std::endl;

		char seq[] = { '%', 'r', 'e', 'p','s', ' ', 'H' };
    	const size_t seq_length = 7;
		debug_printDfa(word_base, seq, seq_length);
	}

	AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
		new AggregateDfasAndDelete<lexer_dfa*>(7, word_base,
			A, B, C, D, E, F);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

	AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
		new AggregateDfaTransitionsAndDelete<DfaTransition*>(6, 
			idfa1, idfa2, idfa3, idfa4, idfa5, idfa6);
	std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> ret(innerRet, aggrAndDelDfaTransitions);

	return ret;
}

std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> lexer_word_constructor::_constructSquareBracketReps()
{
	//const int ST_ERROR = -1;
	const int ST_BASE = 0, ST_A = 1, ST_B = 2, ST_C = 3, ST_D = 4, ST_E = 5;
	const int ST_F = 6;

    lexer_word_repr* word_base = dfaManager.createLexerWordRepr(ST_BASE);

    //we need to address sharing, if we're going to or not - if so how.
    lexer_dfa* A = dfaManager.createDfa(ST_A);
    auto B = dfaManager.createDfa(ST_B);
	auto C = dfaManager.createDfa(ST_C);
	auto D = dfaManager.createDfa(ST_D);
	auto E = dfaManager.createDfa(ST_E);
	auto F = dfaManager.createDfa(ST_F);	
	auto G = dfaManager.createDfa(ST_ACCEPT);
	
    //todo: we need to add capability to lets us make objects that represent StateAndInput for closure expressions. Lazy example: (1-9)*
    StateAndInput<int,char> stateInput1(ST_BASE,'[');
    StateAndInput<int,char> stateInput2(ST_A,'r');
	StateAndInput<int,char> stateInput3(ST_B,'e');
	StateAndInput<int,char> stateInput4(ST_C,'p');
	StateAndInput<int,char> stateInput5(ST_D,'s');
	StateAndInput<int,char> stateInput6(ST_E,']');
	StateAndInput<int,char> stateInput7(ST_F,' ');
	StateAndInput<int,char> stateInput8(ST_ACCEPT, EMPTY_CHAR);

    const DfaTransition* idfa1 = dfaManager.createDfaTransition(&stateInput1,A);
    auto idfa2 = dfaManager.createDfaTransition(&stateInput2,B);
	auto idfa3 = dfaManager.createDfaTransition(&stateInput3,C);
    auto idfa4 = dfaManager.createDfaTransition(&stateInput4,D);
    auto idfa5 = dfaManager.createDfaTransition(&stateInput5,E);
    auto idfa6 = dfaManager.createDfaTransition(&stateInput6,F);	
    auto idfa7 = dfaManager.createDfaTransition(&stateInput7,G);	


    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
	bool wasSuccess =  _lexer_builder->addDfa(word_base, applyObj);

	const ApplyImmutableFunc<DfaTransition*> applyObjAtoB(1,idfa2);
	bool wasSuccess2 = _lexer_builder->addDfa(A, applyObjAtoB);

    const ApplyImmutableFunc<DfaTransition*> applyObjBtoC(1,idfa3);
    bool wasSuccess3 = _lexer_builder->addDfa(B, applyObjBtoC);

	const ApplyImmutableFunc<DfaTransition*> applyObjCtoD(1,idfa4);
    bool wasSuccess4 = _lexer_builder->addDfa(C, applyObjCtoD);

    const ApplyImmutableFunc<DfaTransition*> applyObjDtoE(1,idfa5);
    bool wasSuccess5 = _lexer_builder->addDfa(D, applyObjDtoE);

    const ApplyImmutableFunc<DfaTransition*> applyObjEtoF(1,idfa6);
	bool wasSuccess6 = _lexer_builder->addDfa(E, applyObjEtoF);
 
    const ApplyImmutableFunc<DfaTransition*> applyObjFtoG(1,idfa7);
	bool wasSuccess7 = _lexer_builder->addDfa(F, applyObjFtoG);
 
	if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6
			&& wasSuccess7)
	{
		std::cout << std::endl << "Successfully constructed [reps] Dfa: Base To A";
        std::cout << std::endl << "Successfully constructed [reps] Dfa: A To B";
        std::cout << std::endl << "Successfully constructed [reps] Dfa: B To C";
        std::cout << std::endl << "Successfully constructed [reps] Dfa: C To D";
    	std::cout << std::endl << "Successfully constructed [reps] Dfa: D To E";
        std::cout << std::endl << "Successfully constructed [reps] Dfa: E To F";
        std::cout << std::endl << "Successfully constructed [reps] Dfa: F To G";

		std::cout << std::endl;

		char seq[] = { 'k',	'\n','[', 'r', 'e', 'p','s', ']', ' ', 'H' };
    	const size_t seq_length = 10;
		debug_printDfa(word_base, seq, seq_length);
	}

	__insertNamedRepitionParamsDfa(A,B);	


	AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
		new AggregateDfasAndDelete<lexer_dfa*>(7, word_base,
			A, B, C, D, E, F, G);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

	AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
		new AggregateDfaTransitionsAndDelete<DfaTransition*>(6, 
			idfa1, idfa2, idfa3, idfa4, idfa5, idfa6, idfa7);
	std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> ret(innerRet, aggrAndDelDfaTransitions);

	return ret;
}

std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> lexer_word_constructor::__insertNamedRepitionParamsDfa(lexer_dfa* fromDfa, lexer_dfa* toDfa)
{
    //const int ST_ERROR = -1;
    const int ST_BASE = 0, ST_1 = 1, ST_2 = 2, ST_3 = 3, ST_4 = 4, ST_5 = 5;
    const int ST_6 = 6, ST_7 = 7, ST_8 = 8;

    lexer_word_repr* word_base = dfaManager.createLexerWordRepr(ST_BASE);

    //we need to address sharing, if we're going to or not - if so how.
    lexer_dfa* DFA_1 = dfaManager.createDfa(ST_1);
    auto DFA_2 = dfaManager.createDfa(ST_2);
    auto DFA_3 = dfaManager.createDfa(ST_3);
    auto DFA_4 = dfaManager.createDfa(ST_4);
    auto DFA_5 = dfaManager.createDfa(ST_5);
    auto DFA_6 = dfaManager.createDfa(ST_6);
    auto DFA_7 = dfaManager.createDfa(ST_7);
    auto DFA_8 = dfaManager.createDfa(ST_8);
    
    auto word_end = dfaManager.createDfa(ST_ACCEPT);	

    StateAndInput<int,char> stateInput_ST_BASE_SI_CHARS_ANY(ST_BASE, SI_CHARS_ANY);
    	StateAndInput<int,char> stateInput_ST_1_SI_CHARS_ANY(ST_1, SI_CHARS_ANY);
    		StateAndInput<int,char> stateInput_ST_1_equals(ST_1, '=');
    			StateAndInput<int,char> stateInput_ST_2_SI_NUMBERS_1to9(ST_2, SI_NUMBERS_1to9);
    				StateAndInput<int,char> stateInput_ST_3_SI_NUMBERS_1to9(ST_3, SI_NUMBERS_1to9);
    				StateAndInput<int,char> stateInput_ST_3_colon(ST_3, ':');
    			StateAndInput<int,char> stateInput_ST_2_SI_NUMBERS_0(ST_2, SI_NUMBERS_0);
    				StateAndInput<int,char> stateInput_ST_4_colon(ST_4, ':');
    					StateAndInput<int,char> stateInput_ST_5_SI_NUMBERS_1to9(ST_5, SI_NUMBERS_1to9);
    						StateAndInput<int,char> stateInput_ST_6_SI_NUMBERS_0to9(ST_6, SI_NUMBERS_0to9);
    						StateAndInput<int,char> stateInput_ST_6_space(ST_6, ' ');
    						StateAndInput<int,char> stateInput_ST_6_newline(ST_6, '\n');
    					StateAndInput<int,char> stateInput_ST_5_SI_NUMBERS_0(ST_5, SI_NUMBERS_0);
    						StateAndInput<int,char> stateInput_ST_7_space(ST_7, ' ');
    						StateAndInput<int,char> stateInput_ST_7_newline(ST_7, '\n');
    
    StateAndInput<int,char> stateInput_ST_ACCEPT_EMPTY_CHAR(ST_ACCEPT, EMPTY_CHAR);

	//todo: think about changing function signature so that the caller does some of the hardwork,
	//			basically at the end we'll return word_base, and leave to them to connect their last
	//			dfa node to word_base.

    const DfaTransition* idfa1 = dfaManager.createDfaTransition(&stateInput_ST_BASE_SI_CHARS_ANY, DFA_1);
    
    	auto idfa2 = dfaManager.createDfaTransition(&stateInput_ST_1_SI_CHARS_ANY, word_base);
    	auto idfa3 = dfaManager.createDfaTransition(&stateInput_ST_1_equals, DFA_2);
    		auto idfa4 = dfaManager.createDfaTransition(&stateInput_ST_2_SI_NUMBERS_1to9, DFA_3);
    			auto idfa5 = dfaManager.createDfaTransition(&stateInput_ST_3_SI_NUMBERS_1to9, DFA_3);
    			auto idfa6 = dfaManager.createDfaTransition(&stateInput_ST_3_colon, DFA_5);	
    		auto idfa7 = dfaManager.createDfaTransition(&stateInput_ST_2_SI_NUMBERS_0, DFA_4);
    			auto idfa8 = dfaManager.createDfaTransition(&stateInput_ST_4_colon, DFA_5);
	    			auto idfa9 = dfaManager.createDfaTransition(&stateInput_ST_5_SI_NUMBERS_1to9, DFA_6);
	    				auto idfa10 = dfaManager.createDfaTransition(&stateInput_ST_6_SI_NUMBERS_0to9, DFA_6);
	    				auto idfa11 = dfaManager.createDfaTransition(&stateInput_ST_6_space, DFA_8);
	    				auto idfa12 = dfaManager.createDfaTransition(&stateInput_ST_6_newline, DFA_8);
	    			auto idfa13 = dfaManager.createDfaTransition(&stateInput_ST_5_SI_NUMBERS_0, DFA_7);
	    				auto idfa14 = dfaManager.createDfaTransition(&stateInput_ST_7_space, DFA_8);
	    				auto idfa15 = dfaManager.createDfaTransition(&stateInput_ST_7_newline, DFA_8);

	//todo: make transition to toDfa ptr with stateInput_ST_ACCEPT_EMPTY_CHAR 
	
    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
    bool wasSuccess =  _lexer_builder->addDfa(word_base, applyObj);

    const ApplyImmutableFunc<DfaTransition*> applyObj1to23(2,idfa2,idfa3);
    bool wasSuccess2 = _lexer_builder->addDfa(DFA_1, applyObj1to23);

    const ApplyImmutableFunc<DfaTransition*> applyObj2to34(2,idfa4,idfa7);
    bool wasSuccess3 = _lexer_builder->addDfa(DFA_2, applyObj2to34);

    const ApplyImmutableFunc<DfaTransition*> applyObj3to35(2,idfa5,idfa6);
    bool wasSuccess4 = _lexer_builder->addDfa(DFA_3, applyObj3to35);

    const ApplyImmutableFunc<DfaTransition*> applyObj4to5(1,idfa8);
    bool wasSuccess5 = _lexer_builder->addDfa(DFA_4, applyObj4to5);

    const ApplyImmutableFunc<DfaTransition*> applyObj5to67(2,idfa9,idfa13);
    bool wasSuccess6 = _lexer_builder->addDfa(DFA_5, applyObj5to67);
 
    const ApplyImmutableFunc<DfaTransition*> applyObj6to688(1,idfa10,idfa11,idfa12);
    bool wasSuccess7 = _lexer_builder->addDfa(DFA_6, applyObj6to688);
 
    const ApplyImmutableFunc<DfaTransition*> applyObj7to88(1,idfa14,idfa15);
    bool wasSuccess8 = _lexer_builder->addDfa(DFA_7, applyObj7to88);
    
    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6
                   && wasSuccess7 && wasSuccess8)
    {
        std::cout << std::endl << "Successfully constructed [reps] Dfas idfa[1-15]" << std::endl;

		char seq[] = { 'k',	'\n','a', '=', '1', ':','1', '0', ' ', 'H', '\n' };
    	const size_t seq_length = 11;
		debug_printDfa(word_base, seq, seq_length);
	}

	AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
		new AggregateDfasAndDelete<lexer_dfa*>(9, word_base,
			DFA_1, DFA_2, DFA_3, DFA_4, DFA_5, DFA_6, DFA_7, DFA_8);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

	AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
		new AggregateDfaTransitionsAndDelete<DfaTransition*>(15, 
			idfa1, idfa2, idfa3, idfa4, idfa5, idfa6, idfa7, idfa8, idfa9, idfa10,
			idfa11, idfa12, idfa13, idfa14, idfa15);
	std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> ret(innerRet, aggrAndDelDfaTransitions);

	return ret;
}

void lexer_word_constructor::_destructDfasAndTransitions()
{
	if (_words.size() != _dfaTransitions.size())
	{
		std::cout << "Error, Unintended Implementation" << std::endl
			<< "the number of lexical words should be equal to number of DfaTransition groups.";

		exit(1);
	}

	while (_words.size() > 0 && _dfaTransitions.size() > 0)
	{
		auto entryPair = _words.back();
		auto aggregateAndDeleteObj = entryPair.second;
	
		//this takes care of all the dfas, including entryPair.first
		aggregateAndDeleteObj->applyDelete(dfaManager);
		delete aggregateAndDeleteObj;
		aggregateAndDeleteObj = nullptr;

		_words.pop_back();
		std::cout << "Deleted one lexer_word successfully" << std::endl;

		auto aggregateAndDeleteTransObj = _dfaTransitions.back();
        aggregateAndDeleteTransObj->applyDelete(dfaManager);
        delete aggregateAndDeleteTransObj;
        aggregateAndDeleteTransObj = nullptr;

        _dfaTransitions.pop_back();
        std::cout << "Deleted a group of DfaTransitions successfully" << std::endl;
	}
}

void lexer_word_constructor::_initWords()
{
	//the first in container is a pair with lexer_dfa accessible
	//the second in container is a reminder for us to delete DfaTransitions when we're done

	auto squareReps = _constructSquareBracketReps();
	_words.push_back(squareReps.first);
	_dfaTransitions.push_back(squareReps.second);
	
    auto percentReps = _constructPercentReps();
    _words.push_back(percentReps.first);
	_dfaTransitions.push_back(percentReps.second);
}
