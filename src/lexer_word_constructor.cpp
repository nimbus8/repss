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

#include <unordered_set>

#include "lib/includes/execution_phase/lexer/construction/lexer_word_constructor.hpp"
#include "lib/includes/execution_phase/lexer/construction/model_representation/finite_autonoma/StateAndInput.hpp"
#include "lib/includes/execution_phase/lexer/construction/model_representation/finite_autonoma/DfaTransition.hpp"
#include "lib/includes/execution_phase/lexer/construction/AggregateDfasAndDelete.hpp"
#include "lib/includes/execution_phase/lexer/construction/AggregateDfaTransitionsAndDelete.hpp"
#include "lib/includes/execution_phase/lexer/construction/model_representation/finite_autonoma/LexerStateAndInput.hpp"
#include "lib/includes/utils/Stopwatch.hpp"

//todo: should be moved to lexer_word_constructor.hpp file
//EMPTY CHAR AKA the ANY char, doesn't move the chains just accepts everything as input
#define EMPTY_CHAR '\0'
#define ST_ACCEPT 1001

void debug_printDfa(const DfaManager& dfaManager, const lexer_word_repr*  word, char seq[], size_t seq_length);

//oct 31: optimization with scanwords failed hard. it could be just the smallness and similarity of our
//         base language constructs. May want to just wait for the language to reach 90%, and come back to this.
//         still agree that scanwords needs to be there to hide internals, but question is whether it behave
//         more like mergedWords.
bool lexer_word_constructor::_testScanWords()
{
    bool retResult = true;

    const char seq1[] = {'5', '/','/', '[','/', ']', 'H', '\0'};
    const char seq2[] = {'b','/', '[', 's', 'c','o', ']', ' ', '\0'};
    const char seq3[] = { 'f','y','i','/', '[', 'r', 'e','p', ']', ' ','a','=','2',':','8', '1', '\n', '\0' };
    const char seq4[] = { 'a', 'b', 's', 'o', 'l', 'u', 't', '/', '[', 'p', 'a', 'r', '/', '[', '(', 'a', '|', 'p', '|', 'e', ')', ']', ' ', '\0' };
    const char seq5[] = { 'k', '\n', ',', '/', '[', '}', '/', '[', '[', 'y','a', 'b', 'b', 'a', '_', 'd', 'a', 'b', ']', ']', 'H', '\0' };

    std::cout << std::endl;
    std::cout << "Starting Scan Words Test" << std::endl
              << "------------------------" << std::endl;

    StopWatch stopwatch;
    stopwatch.start();

    for (int round = 0; round < 10000; round++)
    { 
    for (int k = 0; k < 5; k++)
    {
        auto word = _scanWords;

        char seq[25];
        strcpy(seq, (k==0? seq1 : (k==1? seq2 : (k==2? seq3 : (k==3? seq4 : seq5)))));
        const size_t seq_length = (k == 0? 7 : (k==1? 8 : (k==2? 17 : (k==3? 23 : 21))));

        int count = 0;
        auto curr = word;

        const ScanWordNode* nextDfa;

        do
        {//search for a beginning
            //std::cout << "(id, input-idx, input): " << curr->getId() << ", " << count << ", " << seq[count] << std::endl;
            const auto aNextDfa = curr->getNextScanWordNode(_scanWordTransitionMap, seq[count]);
            count++;

            if (aNextDfa != nullptr)
            {
                nextDfa = aNextDfa;
                //std::cout << "break" << std::endl;
                break;
            }
        } while(count < seq_length);

        while ((nextDfa != nullptr) && (!dfaManager.isAcceptingNode(nextDfa->getId()) && count < seq_length))
        {
            curr = const_cast<ScanWordNode*>(nextDfa);
            nextDfa = curr->getNextScanWordNode(_scanWordTransitionMap, seq[count]);

            //std::cout << "(id, input-idx, input): " << curr->getId() << ", " << count << ", " << seq[count] << std::endl;

            if (nextDfa == nullptr)
            {

                curr = word; //reset 'state' to 'state' at wordbase

                do
                {//search for a beginning
                    curr = word;    //reset 'state' to 'state' at worbase | reset current dfa

                    //std::cout << "(id, input-idx, input): " << curr->getId() << ", " << count << ", " << seq[count] << std::endl;

                    const auto aNextDfa = curr->getNextScanWordNode(_scanWordTransitionMap, seq[count]);
                    if (aNextDfa != nullptr)
                    {
                        nextDfa = aNextDfa;
                        count++;
                        break;
                    }
                    else
                    {
                        count++;
                    }
                } while(count < seq_length);
            }
            else
            {
                count++;
            }
        }

        //std::cout << "done search while loop" << std::endl;

        if (nextDfa == nullptr)
        {
            //std::cout << "nothing" << std::endl;
            retResult = retResult && false;
        }
        else if (dfaManager.isAcceptingNode(nextDfa->getId()))
        {
            auto foundWord = dfaManager.getAcceptingNodeName(nextDfa->getId());
            //std::cout << "found word! word=(" << foundWord << ")" << std::endl;
            retResult = retResult && true;
        }
        else if (count >= seq_length)
        {
            auto isAcceptingBool = dfaManager.isAcceptingNode(curr->getId());

            if (isAcceptingBool)
            {
                //std::cout << "found word!!" << std::endl;
                retResult = retResult && true;
            }
            else
            {
                //std::cout << "reached end of input" << std::endl;
                retResult = retResult && false;
            }
        }
        else
        {
            retResult = retResult && false;
        }

        //std::cout << std::endl;
    }
    }

    stopwatch.getElapsedAndPrintfd("ScanWordTest: done ALL search while loops in %3.3f milliseconds\n\n");

    std::cout << "--Scan Word Test Result: " << (retResult? "Success" : "FAILED") << std::endl;

    return retResult;
}

//at the end of this function, the toSet will have all the elements in fromVector.
//the reverse isn't necessarily true
//todo:will, this may be a cool thing to put under until...maybe (maybe also support vector to vector transfer)
void transferDifference(std::unordered_set<ScanWordNode*>& toSet, const std::vector<ScanWordNode*>& fromVector)
{
    for (auto element : fromVector)
    {
        //transfer only elements in fromVector, not in toSet, to toSet
        if (toSet.find(element) == toSet.end())
        {
            toSet.emplace(element);
        }
    }
}

bool lexer_word_constructor::_constructScanWords()
{
    _scanWordTransitionMap = new ScanWordTransitionMap();

    std::cout << "\nConstructing ScanWords" << std::endl;
    std::cout << "......................" << std::endl << std::endl;

    std::unordered_set<ScanWordNode*> existingScanWordNodes;
    std::vector<ScanWordNode*> nodesToBeInitd;

    //note: todo:will aggregate and delete for scan nodes may be necessary
    ScanWordNode* startScanWordNode = new ScanWordNode(_startWordForMergedRepr);

    existingScanWordNodes.emplace(startScanWordNode);
    nodesToBeInitd.push_back(startScanWordNode);

    while (nodesToBeInitd.size() > 0)
    {
        auto wordNode = nodesToBeInitd.back();
        nodesToBeInitd.pop_back();

        std::cout << "Popped back from 'nodesToBeInitd': node{ id=(" << wordNode->getId() << ")}" << std::endl;
        wordNode->init(_scanWordTransitionMap, existingScanWordNodes, nodesToBeInitd); //we pass in transition map, to let ScanWord add to it

        std::cout << "Copying new entries in 'nodesToBeIntd' to 'existingScanWordNodes'" << std::endl; 
        transferDifference(existingScanWordNodes, nodesToBeInitd);
    }

    _scanWords = startScanWordNode; //may be deprecated in favor of pair w/ aggregateAndDelete
    AggregatePtrsAndDelete<ScanWordNode*>* scanWordPtrAggr = new AggregatePtrsAndDelete<ScanWordNode*>(existingScanWordNodes);
    _scanWordStartAndPtrAggregation = new std::pair<ScanWords*, AggregatePtrsAndDelete<ScanWordNode*>*>(startScanWordNode, scanWordPtrAggr);

    std::cout << "Beginning ScanWord Test" << std::endl;

    auto result = _testScanWords();

    std::cout << "Done Scan Word Test - result:" << result << std::endl;

    return true;
}

bool lexer_word_constructor::_testMergedRepresentation()
{
    bool retResult = true;

    const char seq1[] = {'5', '/','/', '[','/', ']', 'H', '\0'};
    const char seq2[] = {'b','/', '[', 's', 'c','o', ']', ' ', '\0'};
    const char seq3[] = { 'f','y','i','/', '[', 'r', 'e','p', ']', ' ','a','=','2',':','8', '1', '\n', '\0' };
    const char seq4[] = { 'a', 'b', 's', 'o', 'l', 'u', 't', '/', '[', 'p', 'a', 'r', '/', '[', '(', 'a', '|', 'p', '|', 'e', ')', ']', ' ', '\0' };
    const char seq5[] = { 'k', '\n', ',', '/', '[', '}', '/', '[', '[', 'y','a', 'b', 'b', 'a', '_', 'd', 'a', 'b', ']', ']', 'H', '\0' };

    std::cout << "Starting Merged Representation Test" << std::endl
              << "-----------------------------------" << std::endl;

    StopWatch stopwatch;
    stopwatch.start();

    for (int round = 0; round < 10000; round++)
    {
    for (int k = 0; k < 5; k++)
    {
        auto word = _startWordForMergedRepr;

        char seq[25];
        strcpy(seq, (k==0? seq1 : (k==1? seq2 : (k==2? seq3 : (k==3? seq4 : seq5)))));
        const size_t seq_length = (k == 0? 7 : (k==1? 8 : (k==2? 17 : (k==3? 23 : 21))));

        int count = 0;
        auto curr = word;

        const lexer_word_repr* nextDfa;

        do
        {//search for a beginning
            //std::cout << "(id, input-idx, input): " << curr->getId() << ", " << count << ", " << seq[count] << std::endl;
            auto aNextDfa = curr->getNextDfaForInput(seq[count]);
            count++; 

            if (aNextDfa != nullptr)
            {
                nextDfa = aNextDfa;
                //std::cout << "break" << std::endl;
                break;
            }
        } while(count < seq_length);

        while ((nextDfa != nullptr) && (!dfaManager.isAcceptingNode(nextDfa->getId()) && count < seq_length))
        {
            curr = const_cast<lexer_dfa*>(nextDfa);
            nextDfa = curr->getNextDfaForInput(seq[count]);

            //std::cout << "(id, input-idx, input): " << curr->getId() << ", " << count << ", " << seq[count] << std::endl;

            if (nextDfa == nullptr)
            {
                curr = word; //reset 'state' to 'state' at wordbase

                do
                {//search for a beginning
                    curr = word;    //reset 'state' to 'state' at worbase | reset current dfa

                    //std::cout << "(id, input-idx, input): " << curr->getId() << ", " << count << ", " << seq[count] << std::endl;

                    auto aNextDfa = curr->getNextDfaForInput(seq[count]);
                    if (aNextDfa != nullptr)
                    {
                        nextDfa = aNextDfa;
                        count++;
                        break;
                    }
                    else
                    {
                        count++;
                    }
                } while(count < seq_length);
            }
            else
            {
                count++;
            }
        }

        //std::cout << "done search while loop in ";

        if (nextDfa == nullptr)
        {
            //std::cout << "nothing" << std::endl;
            retResult = retResult && false;
        }
        else if (dfaManager.isAcceptingNode(nextDfa->getId()))
        {
            auto foundWord = dfaManager.getAcceptingNodeName(nextDfa->getId());
            //std::cout << "found word! word=(" << foundWord << ")" << std::endl;
            retResult = retResult && true;
        }
        else if (count >= seq_length)
        {
            auto isAcceptingBool = dfaManager.isAcceptingNode(curr->getId());

            if (isAcceptingBool)
            {
                //std::cout << "found word!!" << std::endl;
                retResult = retResult && true;
            }
            else
            {
                //std::cout << "reached end of input" << std::endl;
                retResult = retResult && false;
            }
        }
        else
        {
            retResult = retResult && false;
        }

        //std::cout << std::endl;
    }
    }

    stopwatch.getElapsedAndPrintfd("Merged Word Repr Test: done ALL search while loops in %3.3f milliseconds\n\n");

    return retResult;
}

// /[rep(ss)] var=('e','l','e','m','e','n','t','s')
wordrepr_and_transition_Pair_t lexer_word_constructor::_constructKeyword_REPS_withNamedListIteration()
{
    const std::string WORD_NAME("repetition");

    //Note the following is very verbose, we could make it less so - but I think there would be some expense that comes with that. Its kindof the point *[1].
    lexer_word_repr* word_base = dfaManager.createLexerWordRepr();

    lexer_dfa* A = dfaManager.createDfa();
    auto B = dfaManager.createDfa();
    auto C = dfaManager.createDfa();
    auto D = dfaManager.createDfa();
    auto Da = dfaManager.createDfa();
    auto Db = dfaManager.createDfa();
    auto Dc = dfaManager.createDfa();
    auto Dd = dfaManager.createDfa();
    auto E = dfaManager.createDfa();
    auto E_a = dfaManager.createDfa(); //bracket(']') after E

    auto F_resPair = dfaManager.createAcceptingDfaTentativelyNamed(WORD_NAME);
    auto F = F_resPair.first;
    auto tentativeNameKey = F_resPair.second;

    const auto ST_BASE = word_base->getId();
    const auto ST_A = A->getId(), ST_B = B->getId(), ST_C = C->getId(), ST_D = D->getId();
    const auto ST_Da = Da->getId(), ST_Db = Db->getId(), ST_Dc = Dc->getId(), ST_Dd = Dd->getId(), ST_E = E->getId(), ST_E_a = E_a->getId();

    StateAndInput<int,char> stateInput1(ST_BASE,'/');
    StateAndInput<int,char> stateInput2(ST_A,'[');
    StateAndInput<int,char> stateInput3(ST_B,'r');
    StateAndInput<int,char> stateInput4(ST_C,'e');
    StateAndInput<int,char> stateInput5(ST_D,'p');

    StateAndInput<int,char> stateInput5a(ST_Da,'(');
    StateAndInput<int,char> stateInput5b(ST_Db,'s');
    StateAndInput<int,char> stateInput5c(ST_Dc,'s');
    StateAndInput<int,char> stateInput5d(ST_Dd,')');

    StateAndInput<int,char> stateInput6(ST_E,']');
    StateAndInput<int,char> stateInput6a(ST_E_a, ' ');

    auto repititionPair = __insertNamedListIterationParamsDfa(E_a,F, tentativeNameKey);
    auto E_b = repititionPair.first.first; //space after E_a

    StateAndInput<int,char> stateInput7(ST_ACCEPT, EMPTY_CHAR);


    const DfaTransition* idfa1 = dfaManager.createDfaTransition(&stateInput1,A);
    auto idfa2 = dfaManager.createDfaTransition(&stateInput2,B);
    auto idfa3 = dfaManager.createDfaTransition(&stateInput3,C);
    auto idfa4 = dfaManager.createDfaTransition(&stateInput4,D);
    auto idfa5 = dfaManager.createDfaTransition(&stateInput5,Da);

    auto idfa5a = dfaManager.createDfaTransition(&stateInput5a,Db);
    auto idfa5b = dfaManager.createDfaTransition(&stateInput5b,Dc);
    auto idfa5c = dfaManager.createDfaTransition(&stateInput5c,Dd);
    auto idfa5d = dfaManager.createDfaTransition(&stateInput5d,E);

    auto idfa6a = dfaManager.createDfaTransition(&stateInput6,E_a);
    auto idfa6b = dfaManager.createDfaTransition(&stateInput6a, E_b);


    const lexer_dfa_builder lexer_builder;

    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
    bool wasSuccess =  lexer_builder.addDfa(word_base, applyObj);

    const ApplyImmutableFunc<DfaTransition*> applyObjAtoB(1,idfa2);
    bool wasSuccess2 = lexer_builder.addDfa(A, applyObjAtoB);

    const ApplyImmutableFunc<DfaTransition*> applyObjBtoC(1,idfa3);
    bool wasSuccess3 = lexer_builder.addDfa(B, applyObjBtoC);

    const ApplyImmutableFunc<DfaTransition*> applyObjCtoD(1,idfa4);
    bool wasSuccess4 = lexer_builder.addDfa(C, applyObjCtoD);

    const ApplyImmutableFunc<DfaTransition*> applyObjDtoDa(1,idfa5);
    bool wasSuccess5 = lexer_builder.addDfa(D, applyObjDtoDa);


    const ApplyImmutableFunc<DfaTransition*> applyObjDatoDb(1,idfa5a);
    bool wasSuccess6 = lexer_builder.addDfa(Da, applyObjDatoDb);

    const ApplyImmutableFunc<DfaTransition*> applyObjDbtoDc(1,idfa5b);
    bool wasSuccess7 = lexer_builder.addDfa(Db, applyObjDbtoDc);

    const ApplyImmutableFunc<DfaTransition*> applyObjDctoDd(1,idfa5c);
    bool wasSuccess8 = lexer_builder.addDfa(Dc, applyObjDctoDd);

    const ApplyImmutableFunc<DfaTransition*> applyObjDdtoE(1,idfa5d);
    bool wasSuccess9 = lexer_builder.addDfa(Dd, applyObjDdtoE);


    const ApplyImmutableFunc<DfaTransition*> applyObjEToEa(1,idfa6a);
    bool wasSuccess10 = lexer_builder.addDfa(E, applyObjEToEa);

    const ApplyImmutableFunc<DfaTransition*> applyObjEaToEb(1,idfa6b);
    bool wasSuccess11 = lexer_builder.addDfa(E_a, applyObjEaToEb);

    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6 && wasSuccess7
        && wasSuccess8 && wasSuccess9 && wasSuccess10 && wasSuccess11)
    {
        std::cout << "Successfully constructed /[rep(ss)] var=('e','l','e','m','e','n','t','s')" << std::endl;

        char seq[] = { 'f','y','i','/', '[', 'r', 'e','p','(','s','s',')', ']', ' ', 'a', '=', '(', '\'', '8', '\'', ')', ' ' };
        const size_t seq_length = 22;
        debug_printDfa(dfaManager,word_base, seq, seq_length);
    }

    AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
        new AggregateDfasAndDelete<lexer_dfa*>(7, word_base,
                        A, B, C, D, E, E_a);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

    AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
        new AggregateDfaTransitionsAndDelete<DfaTransition*>(7,
            idfa1, idfa2, idfa3, idfa4, idfa5, idfa6a, idfa6b);
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> ret(innerRet, aggrAndDelDfaTransitions);

    return ret;
}

wordrepr_and_transition_Pair_t lexer_word_constructor::_constructKeyword_eval()
{
    const std::string WORD_NAME("evaluation");

    lexer_word_repr* word_base = dfaManager.createLexerWordRepr();

    lexer_dfa* DFA_1 = dfaManager.createDfa();
    auto DFA_2 = dfaManager.createDfa();
    auto DFA_3 = dfaManager.createDfa();
    auto DFA_4 = dfaManager.createDfa();
    auto DFA_5 = dfaManager.createDfa();
    auto DFA_6 = dfaManager.createDfa();
    auto DFA_7 = dfaManager.createDfa();
    auto DFA_END = dfaManager.createAcceptingDfa(WORD_NAME);

    const auto ST_BASE = word_base->getId();
    const auto ST_1 = DFA_1->getId(), ST_2 = DFA_2->getId(), ST_3 = DFA_3->getId(), ST_4 = DFA_4->getId(); 
    const auto ST_5 = DFA_5->getId(), ST_6 = DFA_6->getId(), ST_7 = DFA_7->getId();

    StateAndInput<int,char> stateInputBase(ST_BASE,'/');
    StateAndInput<int,char> stateInput1(ST_1, '[');
    StateAndInput<int,char> stateInput2(ST_2, '[');
    StateAndInput<int,char> stateInput3a(ST_3, '_', false, false);
    StateAndInput<int,char> stateInput3b(ST_3, SI_CHARS_ANY, true, false);
        StateAndInput<int,char> stateInput4a(ST_4, SI_CHARS_ANY, true, false);
        StateAndInput<int,char> stateInput4b(ST_4, SI_NUMBERS_0to9, true, false);
            StateAndInput<int,char> stateInput5a(ST_5, SI_NUMBERS_0to9, true, false);
            StateAndInput<int,char> stateInput5b(ST_5, SI_CHARS_ANY, true, false);
            StateAndInput<int,char> stateInput5c(ST_5, ']', false, false);
            StateAndInput<int,char> stateInput5d_a(ST_5, '-', false, false);
            StateAndInput<int,char> stateInput5d_b(ST_5, '_', false, false);
        StateAndInput<int,char> stateInput4c_a(ST_4, '-', false, false);
        StateAndInput<int,char> stateInput4c_b(ST_4, '_', false, false);
            StateAndInput<int,char> stateInput6a_a(ST_6, '-', false, false);
            StateAndInput<int,char> stateInput6a_b(ST_6, '_', false, false);
            StateAndInput<int,char> stateInput6b(ST_6, SI_CHARS_ANY, true, false);
            StateAndInput<int,char> stateInput6c(ST_6, ']', false, false);
            StateAndInput<int,char> stateInput6d(ST_6, SI_NUMBERS_0to9, true, false);
        StateAndInput<int,char> stateInput4d(ST_4, ']', false, false);
            StateAndInput<int,char> stateInput7(ST_7, ']', false, false);


    const DfaTransition* idfaBase = dfaManager.createDfaTransition(&stateInputBase,DFA_1);
    auto idfa1 = dfaManager.createDfaTransition(&stateInput1, DFA_2);
    auto idfa2 = dfaManager.createDfaTransition(&stateInput2, DFA_3);
    auto idfa3a = dfaManager.createDfaTransition(&stateInput3a, DFA_6);
    auto idfa3b = dfaManager.createDfaTransition(&stateInput3b, DFA_4);
        auto idfa4a = dfaManager.createDfaTransition(&stateInput4a, DFA_4);
        auto idfa4b = dfaManager.createDfaTransition(&stateInput4b, DFA_5);
            auto idfa5a = dfaManager.createDfaTransition(&stateInput5a, DFA_5);
            auto idfa5b = dfaManager.createDfaTransition(&stateInput5b, DFA_4);
            auto idfa5c = dfaManager.createDfaTransition(&stateInput5c, DFA_7);
            auto idfa5d_a = dfaManager.createDfaTransition(&stateInput5d_a, DFA_6);
            auto idfa5d_b = dfaManager.createDfaTransition(&stateInput5d_b, DFA_6);
        auto idfa4c_a = dfaManager.createDfaTransition(&stateInput4c_a, DFA_6);
        auto idfa4c_b = dfaManager.createDfaTransition(&stateInput4c_b, DFA_6);
            auto idfa6a_a = dfaManager.createDfaTransition(&stateInput6a_a, DFA_6);
            auto idfa6a_b = dfaManager.createDfaTransition(&stateInput6a_b, DFA_6);
            auto idfa6b = dfaManager.createDfaTransition(&stateInput6b, DFA_4);
            auto idfa6c = dfaManager.createDfaTransition(&stateInput6c, DFA_7);
            auto idfa6d = dfaManager.createDfaTransition(&stateInput6d, DFA_5);
        auto idfa4d = dfaManager.createDfaTransition(&stateInput4d, DFA_7);
            auto idfa7 = dfaManager.createDfaTransition(&stateInput7, DFA_END);


    const lexer_dfa_builder lexer_builder;
    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfaBase);
    bool wasSuccess =  lexer_builder.addDfa(word_base, applyObj);

    const ApplyImmutableFunc<DfaTransition*> applyObj1to2(1,idfa1);
    bool wasSuccess2 = lexer_builder.addDfa(DFA_1, applyObj1to2);

    const ApplyImmutableFunc<DfaTransition*> applyObj2to3(1,idfa2);
    bool wasSuccess3 = lexer_builder.addDfa(DFA_2, applyObj2to3);

    const ApplyImmutableFunc<DfaTransition*> applyObj3to4n6(2,idfa3a,idfa3b);
    bool wasSuccess4 = lexer_builder.addDfa(DFA_3, applyObj3to4n6);

    const ApplyImmutableFunc<DfaTransition*> applyObj4to4n5n6n7(5, idfa4a, idfa4b, idfa4c_a, idfa4c_b, idfa4d);
    bool wasSuccess5 = lexer_builder.addDfa(DFA_4, applyObj4to4n5n6n7);

    const ApplyImmutableFunc<DfaTransition*> applyObj5to4n5n6n7(5, idfa5a, idfa5b, idfa5c, idfa5d_a, idfa5d_b);
    bool wasSuccess6 = lexer_builder.addDfa(DFA_5, applyObj5to4n5n6n7);

    const ApplyImmutableFunc<DfaTransition*> applyObj6to4n5n6n7(5, idfa6a_a, idfa6a_b, idfa6b, idfa6c, idfa6d);
    bool wasSuccess7 = lexer_builder.addDfa(DFA_6, applyObj6to4n5n6n7);

    const ApplyImmutableFunc<DfaTransition*> applyObj7toEnd(1,idfa7);
    bool wasSuccess8 =  lexer_builder.addDfa(DFA_7, applyObj7toEnd);


    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6
        && wasSuccess7 && wasSuccess8)
    {

        std::cout << std::endl << "Successfully constructed /[[variable]]" << std::endl;

        char seq[] = { 'k', '\n','/', '[', '[', 'y','a', 'b', 'b', 'a', '_', 'd', 'a', 'b', ']', ']', 'H' };
        const size_t seq_length = 17;
        debug_printDfa(dfaManager, word_base, seq, seq_length);
    }

    AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
        new AggregateDfasAndDelete<lexer_dfa*>(7, word_base,
                        DFA_1, DFA_2, DFA_3, DFA_4, DFA_5, DFA_6, DFA_7, DFA_END);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

    AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
        new AggregateDfaTransitionsAndDelete<DfaTransition*>(
            21, idfaBase, idfa1, idfa2, idfa3a, idfa3b, 
            idfa4a, idfa4b, idfa4c_a, idfa4c_b, idfa4d,
            idfa5a, idfa5b, idfa5c, idfa5d_a, idfa5d_b,
            idfa6a_a, idfa6a_b, idfa6b, idfa6c, idfa6d,
            idfa7
        );
 
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> ret(innerRet, aggrAndDelDfaTransitions);

    return ret;
}

wordrepr_and_transition_Pair_t lexer_word_constructor::_constructAlteration()
{
    const std::string WORD_NAME("alteration");

    lexer_word_repr* word_base = dfaManager.createLexerWordRepr();

    lexer_dfa* DFA_2 = dfaManager.createDfa();
    auto DFA_3 = dfaManager.createDfa();
    auto DFA_4 = dfaManager.createDfa();
    auto DFA_5 = dfaManager.createDfa();
    auto DFA_6 = dfaManager.createDfa();
    auto DFA_7 = dfaManager.createDfa();
    auto DFA_8 = dfaManager.createAcceptingDfa(WORD_NAME);

    const auto ST_1 = word_base->getId();
    const auto ST_2 = DFA_2->getId(), ST_3 = DFA_3->getId(), ST_4 = DFA_4->getId(), ST_5 = DFA_5->getId(), ST_6 = DFA_6->getId();
    const auto ST_7 = DFA_7->getId();

    //note to self, 'anything buts' are evaluated after normal and ranged - poor solution but im not sure how to handle this well

    StateAndInput<int,char> stateInput1(ST_1,'/');
    StateAndInput<int,char> stateInput2(ST_2,'[');
    StateAndInput<int,char> stateInput3(ST_3,'(');
    StateAndInput<int,char> stateInput4b(ST_4, ')');
            StateAndInput<int,char> stateInput7(ST_7, ']');
    StateAndInput<int,char> stateInput4a(ST_4,'|');
        StateAndInput<int,char> stateInput5(ST_5,'|',false,true); //goes to 6
        StateAndInput<int,char> stateInput5b(ST_5, '\n', false, false); //goes back into 5 //should go to a state that doesn't accept '\n'
    StateAndInput<int,char> stateInput4c(ST_4, '|', false, true); //non-ranged anything but -- i kno kinda horrible eh? change to enum or (combined uint repr)
        StateAndInput<int,char> stateInput6a(ST_6,'|'); //goes to 5
        StateAndInput<int,char> stateInput6b(ST_6,')'); //goes to 7
        StateAndInput<int,char> stateInput6c(ST_6,'|', false, true); //goes to 6

    const DfaTransition* idfa1 = dfaManager.createDfaTransition(&stateInput1,DFA_2);
    auto idfa2 = dfaManager.createDfaTransition(&stateInput2, DFA_3);
    auto idfa3 = dfaManager.createDfaTransition(&stateInput3, DFA_4);
    auto idfa4a = dfaManager.createDfaTransition(&stateInput4a, DFA_5);
        auto idfa5 = dfaManager.createDfaTransition(&stateInput5, DFA_6);
        auto idfa5b = dfaManager.createDfaTransition(&stateInput5b, DFA_5);
    auto idfa4b = dfaManager.createDfaTransition(&stateInput4b, DFA_7);
            auto idfa7 = dfaManager.createDfaTransition(&stateInput7, DFA_8);
    auto idfa4c = dfaManager.createDfaTransition(&stateInput4c, DFA_6);
        auto idfa6a = dfaManager.createDfaTransition(&stateInput6a, DFA_5);
        auto idfa6b = dfaManager.createDfaTransition(&stateInput6b, DFA_7);
        auto idfa6c = dfaManager.createDfaTransition(&stateInput6c, DFA_6);

    const lexer_dfa_builder lexer_builder;
    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
    bool wasSuccess =  lexer_builder.addDfa(word_base, applyObj);

    const ApplyImmutableFunc<DfaTransition*> applyObj2to3(1,idfa2);
    bool wasSuccess2 = lexer_builder.addDfa(DFA_2, applyObj2to3);

    const ApplyImmutableFunc<DfaTransition*> applyObj3to4(1,idfa3);
    bool wasSuccess3 = lexer_builder.addDfa(DFA_3, applyObj3to4);

    const ApplyImmutableFunc<DfaTransition*> applyObj4to5n7n6(3,idfa4a,idfa4b,idfa4c);
    bool wasSuccess4 = lexer_builder.addDfa(DFA_4, applyObj4to5n7n6);

    const ApplyImmutableFunc<DfaTransition*> applyObj5to6(2,idfa5, idfa5b);
    bool wasSuccess5 = lexer_builder.addDfa(DFA_5, applyObj5to6);

    const ApplyImmutableFunc<DfaTransition*> applyObj6to5n7n6(3,idfa6a,idfa6b,idfa6c);
    bool wasSuccess6 = lexer_builder.addDfa(DFA_6, applyObj6to5n7n6);

    const ApplyImmutableFunc<DfaTransition*> applyObj7to8(1,idfa7);
    bool wasSuccess7 = lexer_builder.addDfa(DFA_7, applyObj7to8);

    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6
        && wasSuccess7)
    {

        std::cout << std::endl << "Successfully constructed /[(a|l|t|e|r|n|a|t|i|o|n)]" << std::endl;

        char seq[] = { 'k', '\n','/', '[', '(', 'a','|', 'c', '|', 'd', '|', 'e', ')', ']', ' ', 'H' };
        const size_t seq_length = 16;
        debug_printDfa(dfaManager, word_base, seq, seq_length);
    }

    AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
        new AggregateDfasAndDelete<lexer_dfa*>(8, word_base,
            DFA_2, DFA_3, DFA_4, DFA_5, DFA_6, DFA_7, DFA_8);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

    AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
        new AggregateDfaTransitionsAndDelete<DfaTransition*>(12,
            idfa1, idfa2, idfa3, idfa4a, idfa4b, idfa4c, idfa5, idfa5b, idfa6a, idfa6b, idfa6c, idfa7);
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> ret(innerRet, aggrAndDelDfaTransitions);

    return ret;
}

wordrepr_and_transition_Pair_t lexer_word_constructor::_constructAlterationAndJoin()
{
    const std::string WORD_NAME("alteration");

    lexer_word_repr* word_base = dfaManager.createLexerWordRepr();

    lexer_dfa* DFA_2 = dfaManager.createDfa();
    auto DFA_3 = dfaManager.createDfa();
    auto DFA_4 = dfaManager.createDfa();
    auto DFA_5 = dfaManager.createDfa();
    auto DFA_6 = dfaManager.createDfa();
    auto DFA_7_a = dfaManager.createDfa(); //right after (')') from 6

    auto DFA_8_resPair = dfaManager.createAcceptingDfaTentativelyNamed(WORD_NAME);
    auto DFA_8 = DFA_8_resPair.first;
    auto tentativeNameKey = DFA_8_resPair.second;



    const auto ST_1 = word_base->getId();
    const auto ST_2 = DFA_2->getId(), ST_3 = DFA_3->getId(), ST_4 = DFA_4->getId(), ST_5 = DFA_5->getId(), ST_6 = DFA_6->getId();

    //note to self, 'anything buts' are evaluated after normal and ranged - poor solution but im not sure how to handle this well

    StateAndInput<int,char> stateInput1(ST_1,'/');
    StateAndInput<int,char> stateInput2(ST_2,'[');
    StateAndInput<int,char> stateInput3(ST_3,'(');

    StateAndInput<int,char> stateInput4b(ST_4, ')'); //goes to 7a

        auto andJoinPair = __insertAndJoinParamsDfa(DFA_7_a,DFA_8, tentativeNameKey);
        DFA_7_a = andJoinPair.first.first; // ']'
    StateAndInput<int,char> stateInput4a(ST_4,'|');
        StateAndInput<int,char> stateInput5(ST_5,'|',false,true); //goes to 6
        StateAndInput<int,char> stateInput5b(ST_5, '\n', false, false); //goes back into 5 //should go to a state that doesn't accept '\n'
    StateAndInput<int,char> stateInput4c(ST_4, '|', false, true); //non-ranged anything but -- i kno kinda horrible eh? change to enum or (combined uint repr)
/*    StateAndInput<int,char> stateInput4b(ST_4, ')'); //goes to 7a

        auto andJoinPair = __insertAndJoinParamsDfa(DFA_7_a,DFA_8, tentativeNameKey);
        DFA_7_a = andJoinPair.first.first; // ']'
*/

        StateAndInput<int,char> stateInput6a(ST_6,'|'); //goes to 5
        StateAndInput<int,char> stateInput6b(ST_6,')'); //goes to 7_a
        StateAndInput<int,char> stateInput6c(ST_6,'|', false, true); //goes to 6

    const DfaTransition* idfa1 = dfaManager.createDfaTransition(&stateInput1,DFA_2);
    auto idfa2 = dfaManager.createDfaTransition(&stateInput2, DFA_3);
    auto idfa3 = dfaManager.createDfaTransition(&stateInput3, DFA_4);
    auto idfa4a = dfaManager.createDfaTransition(&stateInput4a, DFA_5);
        auto idfa5 = dfaManager.createDfaTransition(&stateInput5, DFA_6);
        auto idfa5b = dfaManager.createDfaTransition(&stateInput5b, DFA_5);
    auto idfa4b = dfaManager.createDfaTransition(&stateInput4b, DFA_7_a);
    auto idfa4c = dfaManager.createDfaTransition(&stateInput4c, DFA_6);
        auto idfa6a = dfaManager.createDfaTransition(&stateInput6a, DFA_5);
        auto idfa6b = dfaManager.createDfaTransition(&stateInput6b, DFA_7_a);
        auto idfa6c = dfaManager.createDfaTransition(&stateInput6c, DFA_6);

    const lexer_dfa_builder lexer_builder;
    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
    bool wasSuccess =  lexer_builder.addDfa(word_base, applyObj);

    const ApplyImmutableFunc<DfaTransition*> applyObj2to3(1,idfa2);
    bool wasSuccess2 = lexer_builder.addDfa(DFA_2, applyObj2to3);

    const ApplyImmutableFunc<DfaTransition*> applyObj3to4(1,idfa3);
    bool wasSuccess3 = lexer_builder.addDfa(DFA_3, applyObj3to4);

    const ApplyImmutableFunc<DfaTransition*> applyObj4to5n7n6(3,idfa4a,idfa4b,idfa4c);
    bool wasSuccess4 = lexer_builder.addDfa(DFA_4, applyObj4to5n7n6);

    const ApplyImmutableFunc<DfaTransition*> applyObj5to6(2,idfa5, idfa5b);
    bool wasSuccess5 = lexer_builder.addDfa(DFA_5, applyObj5to6);

    const ApplyImmutableFunc<DfaTransition*> applyObj6to5n7n6(3,idfa6a,idfa6b,idfa6c);
    bool wasSuccess6 = lexer_builder.addDfa(DFA_6, applyObj6to5n7n6);

    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6)
    {

        std::cout << std::endl << "Successfully constructed /[(a|l|t|e|r|n|a|t|i|o|n)]>and join<" << std::endl;

        char seq[] = { 'k', '\n','/', '[', '(', 'a','|', 'c', '|', 'd', '|', 'e', ')', '>', '<', ']', ' ', 'H' };
        const size_t seq_length = 18;
        debug_printDfa(dfaManager, word_base, seq, seq_length);
    }

    AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
        new AggregateDfasAndDelete<lexer_dfa*>(9, word_base,
            DFA_2, DFA_3, DFA_4, DFA_5, DFA_6, DFA_7_a, DFA_8);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

    AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
        new AggregateDfaTransitionsAndDelete<DfaTransition*>(11,
            idfa1, idfa2, idfa3, idfa4a, idfa4b, idfa4c, idfa5, idfa5b, idfa6a, idfa6b, idfa6c);
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> ret(innerRet, aggrAndDelDfaTransitions);

    return ret;
}

wordrepr_and_transition_Pair_t lexer_word_constructor::__insertAndJoinParamsDfa(lexer_dfa_ptr_t fromDfa, lexer_dfa_ptr_t toDfa, unsigned int tentativeNameKey)
{
    const std::string WORD_NAME("andJoin");
    dfaManager.appendToTentativeName(tentativeNameKey, WORD_NAME);

    auto word_base = fromDfa;

    auto DFA_2 = dfaManager.createDfa();
    auto DFA_3 = dfaManager.createDfa();
    auto DFA_4 = dfaManager.createDfa();
    auto DFA_5 = dfaManager.createDfa();
    auto DFA_6 = dfaManager.createDfa();
    auto DFA_7 = dfaManager.createDfa();
    auto DFA_8 = dfaManager.createDfa();

    const auto ST_BASE = word_base->getId();
    const auto ST_2 = DFA_2->getId(), ST_3 = DFA_3->getId(), ST_4 = DFA_4->getId();
    const auto ST_5 = DFA_5->getId(), ST_7 = DFA_7->getId(), ST_8 = DFA_8->getId();

    StateAndInput<int,char> stateInput_ST_BASE_rightBracket(ST_BASE, '>'); //goes 2
        StateAndInput<int,char> stateInput_ST_2_leftBracket(ST_2, '<'); //goes to 3
            StateAndInput<int,char> stateInput_ST_3_leftBracket(ST_3, '<'); //goes to 8
                StateAndInput<int,char> stateInput_ST_8_rightSquareBracket(ST_8, ']'); //goes to 9
            StateAndInput<int,char> stateInput_ST_3_rightSquareBracket(ST_3, ']'); //goes to 9
        StateAndInput<int,char> stateInput_ST_2_anythingButRightBracket(ST_2, '>', false, true); //goes to 4
            StateAndInput<int,char> stateInput_ST_4_anythingButLeftBracket(ST_4, '<', false, true); //goes back in on itself (to 4)
            StateAndInput<int,char> stateInput_ST_4_leftBracket(ST_4, '<'); //goes to 3
        StateAndInput<int,char> stateInput_ST_2_rightBracket(ST_2, '>'); //goes to 5
            StateAndInput<int,char> stateInput_ST_5_anythingButLeftBracket(ST_5, '<', false, true); //goes to 4
            StateAndInput<int,char> stateInput_ST_5_leftBracket(ST_5, '<'); //goes 7
                StateAndInput<int,char> stateInput_ST_7_rightSquareBracket(ST_7, ']'); //goes to 9
            StateAndInput<int,char> stateInput_ST_5_rightBracket(ST_5, '>'); //goes to 6, dead end


    auto idfaBase2 = dfaManager.createDfaTransition(&stateInput_ST_BASE_rightBracket, DFA_2);
        auto idfa2a = dfaManager.createDfaTransition(&stateInput_ST_2_leftBracket, DFA_3);
            auto idfa3a = dfaManager.createDfaTransition(&stateInput_ST_3_leftBracket, DFA_8);
                auto idfa8 = dfaManager.createDfaTransition(&stateInput_ST_8_rightSquareBracket, toDfa);
            auto idfa3b = dfaManager.createDfaTransition(&stateInput_ST_3_rightSquareBracket, toDfa);
        auto idfa2b = dfaManager.createDfaTransition(&stateInput_ST_2_anythingButRightBracket, DFA_4);
            auto idfa4a = dfaManager.createDfaTransition(&stateInput_ST_4_anythingButLeftBracket, DFA_4);
            auto idfa4b = dfaManager.createDfaTransition(&stateInput_ST_4_leftBracket, DFA_3);
        auto idfa2c = dfaManager.createDfaTransition(&stateInput_ST_2_rightBracket, DFA_5);
            auto idfa5a = dfaManager.createDfaTransition(&stateInput_ST_5_anythingButLeftBracket, DFA_4);
            auto idfa5b = dfaManager.createDfaTransition(&stateInput_ST_5_leftBracket, DFA_7);
                auto idfa7 = dfaManager.createDfaTransition(&stateInput_ST_7_rightSquareBracket, toDfa);
            auto idfa5c = dfaManager.createDfaTransition(&stateInput_ST_5_rightBracket, DFA_6);


    const lexer_dfa_builder lexer_builder;

    const ApplyImmutableFunc<DfaTransition*> applyObjToBase(1,idfaBase2);
    bool wasSuccess =  lexer_builder.addDfa(word_base, applyObjToBase);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo2(3,idfa2a,idfa2b,idfa2c);
    bool wasSuccess2 = lexer_builder.addDfa(DFA_2, applyObjTo2);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo3(2,idfa3a,idfa3b);
    bool wasSuccess3 = lexer_builder.addDfa(DFA_3, applyObjTo3);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo8(1,idfa8);
    bool wasSuccess8 = lexer_builder.addDfa(DFA_8, applyObjTo8);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo4(2,idfa4a,idfa4b);
    bool wasSuccess4 = lexer_builder.addDfa(DFA_4, applyObjTo4);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo5(3,idfa5a,idfa5b,idfa5c);
    bool wasSuccess5 = lexer_builder.addDfa(DFA_5, applyObjTo5);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo7(1,idfa7);
    bool wasSuccess7 = lexer_builder.addDfa(DFA_7, applyObjTo7);


    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess7 && wasSuccess8)
    {
        std::cout << std::endl << "Successfully constructed andJoin" << std::endl;
        char seq[] = { 'k', '\n', '>', '>','<',']', 'f', 'o', 'o', 'b','a', 'r', '1', ':', '8', '\'', ',', '\'', 'a', 'b', 'c', '\'', ')', '\n' };
        const size_t seq_length = 24;
        debug_printDfa(dfaManager, word_base, seq, seq_length);
    }


    //todo:will the upper levels will have to keep a list of AggregatePtrsAndDelete, or something like that. Currently it does not.
    AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
                new AggregateDfasAndDelete<lexer_dfa*>(8, word_base,
                       DFA_2, DFA_3, DFA_4, DFA_5, DFA_6, DFA_7, DFA_8);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

    AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
                new AggregateDfaTransitionsAndDelete<DfaTransition*>(13,
                        idfaBase2, idfa2a, idfa2b, idfa2c, idfa3a, idfa3b, idfa8,
                        idfa4a, idfa4b, idfa5a, idfa5b, idfa5c, idfa7);
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> ret(innerRet, aggrAndDelDfaTransitions);

    return ret;
}

wordrepr_and_transition_Pair_t lexer_word_constructor::__insertNamedListIterationParamsDfa(lexer_dfa* fromDfa, lexer_dfa* toDfa, const unsigned int tentativeNameKey)
{
    const std::string WORD_NAME("named_list");
    dfaManager.appendToTentativeName(tentativeNameKey, WORD_NAME);

    lexer_word_repr* word_base = dfaManager.createDfa();

    lexer_dfa* DFA_1 = dfaManager.createDfa(); //we DONT use createStartingDfa here, because its not meant to be a starting dfa, its always intermediate from this point of view.
    auto DFA_2 = dfaManager.createDfa();
    auto DFA_3 = dfaManager.createDfa();
    auto DFA_4 = dfaManager.createDfa();
    auto DFA_5 = dfaManager.createDfa();
    auto DFA_6 = dfaManager.createDfa();

    const auto ST_BASE = word_base->getId(); //word_base->getId();
    const auto ST_1 = DFA_1->getId(), ST_2 = DFA_2->getId(), ST_3 = DFA_3->getId(), ST_4 = DFA_4->getId();
    const auto ST_5 = DFA_5->getId(), ST_6 = DFA_6->getId(); //ST_7 = DFA_7->getId();

    StateAndInput<int,char> stateInput_ST_BASE_SI_CHARS_ANY(ST_BASE, SI_CHARS_ANY, true);
    StateAndInput<int,char> stateInput_ST_BASE_SI_NUMBERS_0to9(ST_BASE, SI_NUMBERS_0to9, true);
        StateAndInput<int,char> stateInput_ST_1_SI_CHARS_ANY(ST_1, SI_CHARS_ANY, true);
        StateAndInput<int,char> stateInput_ST_1_SI_NUMBERS_0to9(ST_1, SI_NUMBERS_0to9, true);
        StateAndInput<int,char> stateInput_ST_1_equals(ST_1, '=');
            StateAndInput<int,char> stateInput_ST_2_space(ST_2, ' ');  //to go back in on itself!
            StateAndInput<int,char> stateInput_ST_2_leftRoundBracket(ST_2, '(');
                StateAndInput<int,char> stateInput_ST_3_singleQuote(ST_3, '\'');
                    StateAndInput<int,char> stateInput_ST_4_anythingButSingleQuote(ST_4, '\'', false, true); //goes back in on itself
                    StateAndInput<int,char> stateInput_ST_4_singleQuote(ST_4, '\'');
                        StateAndInput<int,char> stateInput_ST_5_comma(ST_5, ',');
                            StateAndInput<int,char> stateInput_ST_6_singleQuote(ST_6, '\''); //goes back to DFA_4
                        StateAndInput<int,char> stateInput_ST_5_singleQuote(ST_5, '\''); //goes back into DFA_4
                        StateAndInput<int,char> stateInput_ST_5_rightRoundBracket(ST_5, ')');   


    const DfaTransition* idfaBase1 = dfaManager.createDfaTransition(&stateInput_ST_BASE_SI_CHARS_ANY, DFA_1);  //done
    auto idfaBase2 = dfaManager.createDfaTransition(&stateInput_ST_BASE_SI_NUMBERS_0to9, DFA_1); 
        auto idfa1a = dfaManager.createDfaTransition(&stateInput_ST_1_SI_CHARS_ANY, DFA_1); 
        auto idfa1b = dfaManager.createDfaTransition(&stateInput_ST_1_SI_NUMBERS_0to9, DFA_1);
        auto idfa1c = dfaManager.createDfaTransition(&stateInput_ST_1_equals, DFA_2);
                auto idfa2a = dfaManager.createDfaTransition(&stateInput_ST_2_space, DFA_2);
                auto idfa2b = dfaManager.createDfaTransition(&stateInput_ST_2_leftRoundBracket, DFA_3);
                        auto idfa3 = dfaManager.createDfaTransition(&stateInput_ST_3_singleQuote, DFA_4);
                            auto idfa4a = dfaManager.createDfaTransition(&stateInput_ST_4_anythingButSingleQuote, DFA_4);
                            auto idfa4b = dfaManager.createDfaTransition(&stateInput_ST_4_singleQuote, DFA_5);
                                auto idfa5a = dfaManager.createDfaTransition(&stateInput_ST_5_comma, DFA_6);
                                    auto idfa6 = dfaManager.createDfaTransition(&stateInput_ST_6_singleQuote, DFA_4);
                                auto idfa5b = dfaManager.createDfaTransition(&stateInput_ST_5_singleQuote, DFA_4);
                                auto idfa5c = dfaManager.createDfaTransition(&stateInput_ST_5_rightRoundBracket, toDfa);
                                    

    const lexer_dfa_builder lexer_builder;

    const ApplyImmutableFunc<DfaTransition*> applyObjToBase(2,idfaBase1, idfaBase2);
    bool wasSuccess =  lexer_builder.addDfa(word_base, applyObjToBase);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo1(3,idfa1a,idfa1b,idfa1c);
    bool wasSuccess2 = lexer_builder.addDfa(DFA_1, applyObjTo1);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo2(2,idfa2a,idfa2b);
    bool wasSuccess3 = lexer_builder.addDfa(DFA_2, applyObjTo2);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo3(1,idfa3);
    bool wasSuccess4 = lexer_builder.addDfa(DFA_3, applyObjTo3);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo4(2,idfa4a,idfa4b);
    bool wasSuccess5 = lexer_builder.addDfa(DFA_4, applyObjTo4);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo5(3,idfa5a,idfa5b,idfa5c);
    bool wasSuccess6 = lexer_builder.addDfa(DFA_5, applyObjTo5);

    const ApplyImmutableFunc<DfaTransition*> applyObjTo6(1,idfa6);
    bool wasSuccess7 = lexer_builder.addDfa(DFA_6, applyObjTo6);

    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6
                   && wasSuccess7)
    {
        std::cout << std::endl << "Successfully constructed named=('l','i','s','t')" << std::endl;
        char seq[] = { 'k',     '\n','f', 'o', 'o', 'b','a', 'r', '=', '(', '\'','a',':', '1', ':', '8', '\'', ',', '\'', 'a', 'b', 'c', '\'', ')', '\n' };
        const size_t seq_length = 25;
        debug_printDfa(dfaManager, word_base, seq, seq_length);
    }


    //todo:will the upper levels will have to keep a list of AggregatePtrsAndDelete, or something like that. Currently it does not.
    AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
                new AggregateDfasAndDelete<lexer_dfa*>(9, word_base,
                        DFA_1, DFA_2, DFA_3, DFA_4, DFA_5, DFA_6);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

    AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
                new AggregateDfaTransitionsAndDelete<DfaTransition*>(14,
                        idfaBase1, idfaBase2, idfa1a, idfa1b, idfa1c, idfa2a, idfa2b, idfa3, idfa4a, idfa4b,
                        idfa5a, idfa6, idfa5b, idfa5c);
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> ret(innerRet, aggrAndDelDfaTransitions);

    return ret;
}

wordrepr_and_transition_Pair_t lexer_word_constructor::_constructEnd()
{
    const std::string WORD_NAME("general_end");

    lexer_word_repr* word_base = dfaManager.createLexerWordRepr();

    lexer_dfa* A = dfaManager.createDfa();
    auto B = dfaManager.createDfa();
    auto C = dfaManager.createDfa();
    auto D = dfaManager.createAcceptingDfa(WORD_NAME);

    const auto ST_BASE = word_base->getId();
    const auto ST_A = A->getId(), ST_B = B->getId(), ST_C = C->getId();

    StateAndInput<int,char> stateInput1(ST_BASE,'/');
    StateAndInput<int,char> stateInput2(ST_A,'[');
    StateAndInput<int,char> stateInput3(ST_B,'/');
    StateAndInput<int,char> stateInput4(ST_C,']');
    StateAndInput<int,char> stateInput5(ST_ACCEPT, EMPTY_CHAR);

    const DfaTransition* idfa1 = dfaManager.createDfaTransition(&stateInput1,A);
    auto idfa2 = dfaManager.createDfaTransition(&stateInput2,B);
    auto idfa3 = dfaManager.createDfaTransition(&stateInput3,C);
    auto idfa4 = dfaManager.createDfaTransition(&stateInput4,D);


    const lexer_dfa_builder lexer_builder;
    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
    bool wasSuccess =  lexer_builder.addDfa(word_base, applyObj);

    const ApplyImmutableFunc<DfaTransition*> applyObjAtoB(1,idfa2);
    bool wasSuccess2 = lexer_builder.addDfa(A, applyObjAtoB);

    const ApplyImmutableFunc<DfaTransition*> applyObjBtoC(1,idfa3);
    bool wasSuccess3 = lexer_builder.addDfa(B, applyObjBtoC);

    const ApplyImmutableFunc<DfaTransition*> applyObjCtoD(1,idfa4);
    bool wasSuccess4 = lexer_builder.addDfa(C, applyObjCtoD);

    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4)
    {
        std::cout << "Successfully constructed /[/] " << std::endl;

        char seq[] = { 'f','y','i','/', '[', 'r', 'e','p', ']', 'H','H','/','[','/',']' };
        const size_t seq_length = 15;
        debug_printDfa(dfaManager,word_base, seq, seq_length);
    }

    AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
        new AggregateDfasAndDelete<lexer_dfa*>(5, word_base,
                        A, B, C, D);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

    AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
        new AggregateDfaTransitionsAndDelete<DfaTransition*>(4,
            idfa1, idfa2, idfa3, idfa4);
    wordrepr_and_transition_Pair_t ret(innerRet, aggrAndDelDfaTransitions);

    return ret;
}

//__insertNamedRepititionParamsDfa(A,B);
wordrepr_and_transition_Pair_t lexer_word_constructor::_constructKeyword_REPS_withNamedIteration()
{
    const std::string WORD_NAME("repetition");

    //Note the following is very verbose, we could make it less so - but I think there would be some expense that comes with that. Its kindof the point *[1].
    lexer_word_repr* word_base = dfaManager.createLexerWordRepr();

    lexer_dfa* A = dfaManager.createDfa();
    auto B = dfaManager.createDfa();
    auto C = dfaManager.createDfa();
    auto D = dfaManager.createDfa();
    auto E = dfaManager.createDfa();
    auto E_a = dfaManager.createDfa(); //bracket(']') after E

    auto F_resPair = dfaManager.createAcceptingDfaTentativelyNamed(WORD_NAME);
    auto F = F_resPair.first;
    auto tentativeNameKey = F_resPair.second;

    const auto ST_BASE = word_base->getId();
    const auto ST_A = A->getId(), ST_B = B->getId(), ST_C = C->getId(), ST_D = D->getId(), ST_E = E->getId();
    const auto ST_E_a = E_a->getId();

    StateAndInput<int,char> stateInput1(ST_BASE,'/');
    StateAndInput<int,char> stateInput2(ST_A,'[');
    StateAndInput<int,char> stateInput3(ST_B,'r');
    StateAndInput<int,char> stateInput4(ST_C,'e');
    StateAndInput<int,char> stateInput5(ST_D,'p');
    StateAndInput<int,char> stateInput6(ST_E,']');
    StateAndInput<int,char> stateInput6a(ST_E_a, ' ');

    auto repititionPair = __insertNamedIterationParamsDfa(E_a,F, tentativeNameKey);
    auto E_b = repititionPair.first.first; //space after E_a

    StateAndInput<int,char> stateInput7(ST_ACCEPT, EMPTY_CHAR);

    const DfaTransition* idfa1 = dfaManager.createDfaTransition(&stateInput1,A);
    auto idfa2 = dfaManager.createDfaTransition(&stateInput2,B);
    auto idfa3 = dfaManager.createDfaTransition(&stateInput3,C);
    auto idfa4 = dfaManager.createDfaTransition(&stateInput4,D);
    auto idfa5 = dfaManager.createDfaTransition(&stateInput5,E);
    auto idfa6a = dfaManager.createDfaTransition(&stateInput6,E_a); 
    auto idfa6b = dfaManager.createDfaTransition(&stateInput6a, E_b);

    const lexer_dfa_builder lexer_builder;
    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
    bool wasSuccess =  lexer_builder.addDfa(word_base, applyObj);

    const ApplyImmutableFunc<DfaTransition*> applyObjAtoB(1,idfa2);
    bool wasSuccess2 = lexer_builder.addDfa(A, applyObjAtoB);

    const ApplyImmutableFunc<DfaTransition*> applyObjBtoC(1,idfa3);
    bool wasSuccess3 = lexer_builder.addDfa(B, applyObjBtoC);

    const ApplyImmutableFunc<DfaTransition*> applyObjCtoD(1,idfa4);
    bool wasSuccess4 = lexer_builder.addDfa(C, applyObjCtoD);

   const ApplyImmutableFunc<DfaTransition*> applyObjDtoE(1,idfa5);
    bool wasSuccess5 = lexer_builder.addDfa(D, applyObjDtoE);

    const ApplyImmutableFunc<DfaTransition*> applyObjEToEa(1,idfa6a);
    bool wasSuccess6 = lexer_builder.addDfa(E, applyObjEToEa);

    const ApplyImmutableFunc<DfaTransition*> applyObjEaToEb(1,idfa6b);
    bool wasSuccess7 = lexer_builder.addDfa(E_a, applyObjEaToEb);

    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6 && wasSuccess7)
    {
        std::cout << "Successfully constructed /[rep] var=x:y" << std::endl;

        char seq[] = { 'f','y','i','/', '[', 'r', 'e','p', ']', ' ', 'a', '=', '2', ':', '8', '1', ' ' };
        const size_t seq_length = 17;
        debug_printDfa(dfaManager,word_base, seq, seq_length);
    }

    //we have to figure what to do with repitionPair.first.second -- the cleanup of the allocated memory...
    //plus try and see if adding E_b to the list below (AggregateAndDelete) does any harm

    AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
        new AggregateDfasAndDelete<lexer_dfa*>(7, word_base,
                        A, B, C, D, E, E_a);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

    AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
        new AggregateDfaTransitionsAndDelete<DfaTransition*>(7,
            idfa1, idfa2, idfa3, idfa4, idfa5, idfa6a, idfa6b);
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> ret(innerRet, aggrAndDelDfaTransitions);

    return ret;
}

// /[rep] -- this is currently here as a sort of template. It's not actually used specifically (the function/method -- the call with below signature to be precise.)
std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> lexer_word_constructor::_constructPercentReps()
{
    const std::string WORD_NAME("repetition");

    //Note the following is very verbose, we could make it less so - but I think there would be some expense that comes with that. Its kindof the point *[1].
    lexer_word_repr* word_base = dfaManager.createLexerWordRepr();

    lexer_dfa* A = dfaManager.createDfa();
    auto B = dfaManager.createDfa();
    auto C = dfaManager.createDfa();
    auto D = dfaManager.createDfa();
    auto E = dfaManager.createDfa();
    auto F = dfaManager.createDfa();
    auto G = dfaManager.createDfa();
    auto H = dfaManager.createAcceptingDfa(WORD_NAME);	

    const auto ST_BASE = word_base->getId();
    const auto ST_A = A->getId(), ST_B = B->getId(), ST_C = C->getId(), ST_D = D->getId(), ST_E = E->getId();
    const auto ST_F = F->getId();//, ST_G = G->getId();

    StateAndInput<int,char> stateInput1(ST_BASE,'/');
    StateAndInput<int,char> stateInput2(ST_A,'[');
    StateAndInput<int,char> stateInput3(ST_B,'r');
    StateAndInput<int,char> stateInput4(ST_C,'e');
    StateAndInput<int,char> stateInput5(ST_D,'p');
    StateAndInput<int,char> stateInput6(ST_E,']');
        StateAndInput<int,char> stateInput7(ST_F,'\n');
  
    const DfaTransition* idfa1 = dfaManager.createDfaTransition(&stateInput1,A);
    auto idfa2 = dfaManager.createDfaTransition(&stateInput2,B);
    auto idfa3 = dfaManager.createDfaTransition(&stateInput3,C);
    auto idfa4 = dfaManager.createDfaTransition(&stateInput4,D);
    auto idfa5 = dfaManager.createDfaTransition(&stateInput5,E);
    auto idfa6 = dfaManager.createDfaTransition(&stateInput6,F);	
        auto idfa7 = dfaManager.createDfaTransition(&stateInput7,H);

    const lexer_dfa_builder lexer_builder;
    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
    bool wasSuccess =  lexer_builder.addDfa(word_base, applyObj);

    const ApplyImmutableFunc<DfaTransition*> applyObjAtoB(1,idfa2);
    bool wasSuccess2 = lexer_builder.addDfa(A, applyObjAtoB);

    const ApplyImmutableFunc<DfaTransition*> applyObjBtoC(1,idfa3);
    bool wasSuccess3 = lexer_builder.addDfa(B, applyObjBtoC);

    const ApplyImmutableFunc<DfaTransition*> applyObjCtoD(1,idfa4);
    bool wasSuccess4 = lexer_builder.addDfa(C, applyObjCtoD);

    const ApplyImmutableFunc<DfaTransition*> applyObjDtoE(1,idfa5);
    bool wasSuccess5 = lexer_builder.addDfa(D, applyObjDtoE);

    const ApplyImmutableFunc<DfaTransition*> applyObjEtoF(1,idfa6);
    bool wasSuccess6 = lexer_builder.addDfa(E, applyObjEtoF);

    const ApplyImmutableFunc<DfaTransition*> applyObjFToH(1,idfa7);
    bool wasSuccess7 = lexer_builder.addDfa(F, applyObjFToH);

    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6
     && wasSuccess7)
    {
        std::cout << "Constructed /[rep] " << std::endl;

        char seq[] = { 'f','y','i','/', '[', 'r', 'e','p', ']', 'H' };
        const size_t seq_length = 10;
       	debug_printDfa(dfaManager,word_base, seq, seq_length);
    }

    AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
        new AggregateDfasAndDelete<lexer_dfa*>(9, word_base,
			A, B, C, D, E, F, G, H);
    std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*> innerRet (word_base, aggrAndDel);

    AggregatePtrsAndDelete<DfaTransition*>* aggrAndDelDfaTransitions =
        new AggregateDfaTransitionsAndDelete<DfaTransition*>(7, 
            idfa1, idfa2, idfa3, idfa4, idfa5, idfa6, idfa7);
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> ret(innerRet, aggrAndDelDfaTransitions);

    return ret;
}

std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> lexer_word_constructor::_constructSquareBracketReps()
{
    const std::string WORD_NAME("scope");

    lexer_word_repr* word_base = dfaManager.createLexerWordRepr();

    lexer_dfa* A = dfaManager.createDfa();
    auto B = dfaManager.createDfa();
    auto C = dfaManager.createDfa();
    auto D = dfaManager.createDfa();
    auto E = dfaManager.createDfa();
    auto F = dfaManager.createDfa();	
    auto G = dfaManager.createAcceptingDfa(WORD_NAME);

    const auto ST_BASE = word_base->getId();
    const auto ST_A = A->getId(), ST_B = B->getId(), ST_C = C->getId(), ST_D = D->getId(), ST_E = E->getId();
    const auto ST_F = F->getId();
	
    StateAndInput<int,char> stateInput1(ST_BASE,'/');
    StateAndInput<int,char> stateInput2(ST_A,'[');
    StateAndInput<int,char> stateInput3(ST_B,'s');
    StateAndInput<int,char> stateInput4(ST_C,'c');
    StateAndInput<int,char> stateInput5(ST_D,'o');
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

    const lexer_dfa_builder lexer_builder;
    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
    bool wasSuccess =  lexer_builder.addDfa(word_base, applyObj);

    const ApplyImmutableFunc<DfaTransition*> applyObjAtoB(1,idfa2);
    bool wasSuccess2 = lexer_builder.addDfa(A, applyObjAtoB);

    const ApplyImmutableFunc<DfaTransition*> applyObjBtoC(1,idfa3);
    bool wasSuccess3 = lexer_builder.addDfa(B, applyObjBtoC);

    const ApplyImmutableFunc<DfaTransition*> applyObjCtoD(1,idfa4);
    bool wasSuccess4 = lexer_builder.addDfa(C, applyObjCtoD);

    const ApplyImmutableFunc<DfaTransition*> applyObjDtoE(1,idfa5);
    bool wasSuccess5 = lexer_builder.addDfa(D, applyObjDtoE);

    const ApplyImmutableFunc<DfaTransition*> applyObjEtoF(1,idfa6);
    bool wasSuccess6 = lexer_builder.addDfa(E, applyObjEtoF);
 
    const ApplyImmutableFunc<DfaTransition*> applyObjFtoG(1,idfa7);
    bool wasSuccess7 = lexer_builder.addDfa(F, applyObjFtoG);
 
    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6
        && wasSuccess7)
    {

        std::cout << std::endl << "Successfully constructed /[sco]" << std::endl;

        char seq[] = { 'k', '\n','/', '[', 's', 'c','o', ']', ' ', 'H' };
        const size_t seq_length = 10;
        debug_printDfa(dfaManager, word_base, seq, seq_length);
    }

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

wordrepr_and_transition_Pair_t lexer_word_constructor::__insertNamedIterationParamsDfa(lexer_dfa* fromDfa, lexer_dfa* toDfa, unsigned int tentativeNameKey)
{
    const std::string WORD_NAME("named_iteration");
    dfaManager.appendToTentativeName(tentativeNameKey, WORD_NAME);

    lexer_word_repr* word_base = dfaManager.createDfa();

    lexer_dfa* DFA_1 = dfaManager.createDfa(); //we DONT use createStartingDfa here, because its not meant to be a starting dfa, its always intermediate from this point of view.
    auto DFA_2 = dfaManager.createDfa();
    auto DFA_3 = dfaManager.createDfa();
    auto DFA_4 = dfaManager.createDfa();
    auto DFA_5 = dfaManager.createDfa();
    auto DFA_6 = dfaManager.createDfa();
    auto DFA_7 = dfaManager.createDfa();
   
    //there is not accepting dfa here
 
    const auto ST_BASE = word_base->getId();
    const auto ST_1 = DFA_1->getId(), ST_2 = DFA_2->getId(), ST_3 = DFA_3->getId(), ST_4 = DFA_4->getId();
    const auto ST_5 = DFA_5->getId(), ST_6 = DFA_6->getId(), ST_7 = DFA_7->getId();

    StateAndInput<int,char> stateInput_ST_BASE_SI_CHARS_ANY(ST_BASE, SI_CHARS_ANY, true);
    	StateAndInput<int,char> stateInput_ST_1_SI_CHARS_ANY(ST_1, SI_CHARS_ANY, true);
    		StateAndInput<int,char> stateInput_ST_1_equals(ST_1, '=');
    			StateAndInput<int,char> stateInput_ST_2_SI_NUMBERS_1to9(ST_2, SI_NUMBERS_1to9, true);
    				StateAndInput<int,char> stateInput_ST_3_SI_NUMBERS_1to9(ST_3, SI_NUMBERS_1to9, true);
    				StateAndInput<int,char> stateInput_ST_3_colon(ST_3, ':');
    			StateAndInput<int,char> stateInput_ST_2_SI_NUMBERS_0(ST_2, SI_NUMBERS_0, true);
    				StateAndInput<int,char> stateInput_ST_4_colon(ST_4, ':');
    					StateAndInput<int,char> stateInput_ST_5_SI_NUMBERS_1to9(ST_5, SI_NUMBERS_1to9, true);
    						StateAndInput<int,char> stateInput_ST_6_SI_NUMBERS_0to9(ST_6, SI_NUMBERS_0to9, true);
    						StateAndInput<int,char> stateInput_ST_6_space(ST_6, ' ');
    						StateAndInput<int,char> stateInput_ST_6_newline(ST_6, '\n');
    					StateAndInput<int,char> stateInput_ST_5_SI_NUMBERS_0(ST_5, SI_NUMBERS_0, true);
    						StateAndInput<int,char> stateInput_ST_7_space(ST_7, ' ');
    						StateAndInput<int,char> stateInput_ST_7_newline(ST_7, '\n');
    

    const DfaTransition* idfa1 = dfaManager.createDfaTransition(&stateInput_ST_BASE_SI_CHARS_ANY, DFA_1);
    
    	auto idfa2 = dfaManager.createDfaTransition(&stateInput_ST_1_SI_CHARS_ANY, DFA_1);
    	auto idfa3 = dfaManager.createDfaTransition(&stateInput_ST_1_equals, DFA_2);
    		auto idfa4 = dfaManager.createDfaTransition(&stateInput_ST_2_SI_NUMBERS_1to9, DFA_3);
    			auto idfa5 = dfaManager.createDfaTransition(&stateInput_ST_3_SI_NUMBERS_1to9, DFA_3);
    			auto idfa6 = dfaManager.createDfaTransition(&stateInput_ST_3_colon, DFA_5);	
    		auto idfa7 = dfaManager.createDfaTransition(&stateInput_ST_2_SI_NUMBERS_0, DFA_4);
    			auto idfa8 = dfaManager.createDfaTransition(&stateInput_ST_4_colon, DFA_5);
	    			auto idfa9 = dfaManager.createDfaTransition(&stateInput_ST_5_SI_NUMBERS_1to9, DFA_6);
	    				auto idfa10 = dfaManager.createDfaTransition(&stateInput_ST_6_SI_NUMBERS_0to9, DFA_6);
	    				auto idfa11 = dfaManager.createDfaTransition(&stateInput_ST_6_space, toDfa); //to outside dfa
	    				auto idfa12 = dfaManager.createDfaTransition(&stateInput_ST_6_newline, toDfa);//to outside dfa
	    			auto idfa13 = dfaManager.createDfaTransition(&stateInput_ST_5_SI_NUMBERS_0, DFA_7);
	    				auto idfa14 = dfaManager.createDfaTransition(&stateInput_ST_7_space, toDfa); //to outside dfa
	    				auto idfa15 = dfaManager.createDfaTransition(&stateInput_ST_7_newline, toDfa);//to outside dfa
                                          

    const lexer_dfa_builder lexer_builder;

    const ApplyImmutableFunc<DfaTransition*> applyObj(1,idfa1);
    bool wasSuccess =  lexer_builder.addDfa(word_base, applyObj);

    const ApplyImmutableFunc<DfaTransition*> applyObj1to23(2,idfa2,idfa3);
    bool wasSuccess2 = lexer_builder.addDfa(DFA_1, applyObj1to23);

    const ApplyImmutableFunc<DfaTransition*> applyObj2to34(2,idfa4,idfa7);
    bool wasSuccess3 = lexer_builder.addDfa(DFA_2, applyObj2to34);

    const ApplyImmutableFunc<DfaTransition*> applyObj3to35(2,idfa5,idfa6);
    bool wasSuccess4 = lexer_builder.addDfa(DFA_3, applyObj3to35);

    const ApplyImmutableFunc<DfaTransition*> applyObj4to5(1,idfa8);
    bool wasSuccess5 = lexer_builder.addDfa(DFA_4, applyObj4to5);

    const ApplyImmutableFunc<DfaTransition*> applyObj5to67(2,idfa9,idfa13);
    bool wasSuccess6 = lexer_builder.addDfa(DFA_5, applyObj5to67);
 
    const ApplyImmutableFunc<DfaTransition*> applyObj6to688(3,idfa10,idfa11,idfa12);
    bool wasSuccess7 = lexer_builder.addDfa(DFA_6, applyObj6to688);
 
    const ApplyImmutableFunc<DfaTransition*> applyObj7to88(2,idfa14,idfa15);
    bool wasSuccess8 = lexer_builder.addDfa(DFA_7, applyObj7to88);

    if (wasSuccess && wasSuccess2 && wasSuccess3 && wasSuccess4 && wasSuccess5 && wasSuccess6
                   && wasSuccess7 && wasSuccess8)
    {
        std::cout << std::endl << "Successfully constructed name=x:y" << std::endl;
        char seq[] = { 'k',	'\n','f', 'o', 'o', 'b','a', 'r', '=', '1', ':','1', '0', ' ', 'H', '\n' };
        const size_t seq_length = 16;
        debug_printDfa(dfaManager, word_base, seq, seq_length);
    }


    //todo:will the upper levels will have to keep a list of AggregatePtrsAndDelete, or something like that. Currently it does not.
    AggregatePtrsAndDelete<lexer_dfa*>* aggrAndDel =
		new AggregateDfasAndDelete<lexer_dfa*>(9, word_base,
			DFA_1, DFA_2, DFA_3, DFA_4, DFA_5, DFA_6, DFA_7);
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

void debug_printDfa(const DfaManager& dfaManager, const lexer_word_repr*  word, char seq[], size_t seq_length)
{
    std::cout << "Construction Test: (," << seq << ", " << seq_length << ")" << std::endl;

    int count = 0;
    int state = word->getId(); //start state (for a specific test) is word_base's identifier
    auto curr = word;

    std::cout << "Starting state: " << state << ", startingDfaId: " << curr->getId() << std::endl;

    const lexer_word_repr* nextDfa;

    //search for a beginning
    do
    {
        LexerStateAndInput si0(state, seq[count]);
        std::cout << "search for a beginning: (state, seq,c): " << state << ", " << count << ", " << seq[count] << std::endl;
        count++;
        auto aNextDfa = curr->getNextDfa(si0);
        if (aNextDfa != nullptr)
        {
                state = aNextDfa->getId(); //state++;
                nextDfa = aNextDfa;
                break;
        }
    } while(count < seq_length);

    while ((nextDfa != nullptr) && (!dfaManager.isAcceptingNode(state) && count < seq_length))
    {
        std::cout << "'" << seq[count-1] << "' - DfaNode(" << curr->getId() << ")"
                << " ~ stateAndInput=(state, seq,c): (" << state << ", "
                << count << ", " << seq[count] << ")" << std::endl;

        curr = nextDfa;
        const LexerStateAndInput si(state,seq[count]);
        nextDfa = curr->getNextDfa(si);

        count++;

        if (nextDfa == nullptr)
        {
            state = word->getId(); //reset state to state at word_base

            do
            {//search for a beginning
                curr = word; //reset current dfa
                
                std::cout << "'" << seq[count-1] << "' - DfaNode(" << curr->getId() << ")"
                        << " ~ stateAndInput=(state, seq,c): (" << state << ", "
                        << count << ", " << seq[count] << ")" << std::endl;

                LexerStateAndInput si0(state, seq[count++]);
                auto aNextDfa = curr->getNextDfa(si0);
                if (aNextDfa != nullptr)
                {
                    state = aNextDfa->getId(); //state++;
                    nextDfa = aNextDfa;
                    break;
                }
             } while(count < seq_length);
        }
        else
        {
            state = nextDfa->getId(); //state++;
        }
    }

    if (nextDfa == nullptr)
    {
        std::cout << "nothing" << std::endl << std::endl;
    }
    else if (dfaManager.isAcceptingNode(state))
    {
        auto foundWord = dfaManager.getAcceptingNodeName(nextDfa->getId());
        std::cout << "found word!" << "word=(" << foundWord << ")" << std::endl << std::endl;
    }
    else if (count >= seq_length)
    {
        std::cout << "reached end of input" << std::endl << std::endl;
    }
}

//Footnotes:
//[1] It's why we're building our own lexer instead of using the 'lex' tool (a fine tool btw).
