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

#include "../../../utils/AggregatePtrsAndDelete.hpp"

#ifndef _LEXER_WORD_CONSTRUCTOR_
#define _LEXER_WORD_CONSTRUCTOR_

//lexer_word constructor
class lexer_word_constructor
{
private:
    DfaManager dfaManager;      //dfa manager handles references, create/destroy fn pairs
    std::vector<std::pair<lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>> _words;
    std::vector<AggregatePtrsAndDelete<DfaTransition*>*> _dfaTransitions;
    lexer_dfa_builder* _lexer_builder;
    lexer_word_repr* _startWordForMergedRepr;
  
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> _constructPercentReps();
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> _constructSquareBracketReps();
    std::pair<std::pair <lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>, AggregatePtrsAndDelete<DfaTransition*>*> __insertNamedRepitionParamsDfa(lexer_dfa* fromDfa, lexer_dfa* toDfa);
    void _destructDfasAndTransitions();
    void _initWords();
    bool _testMergedRepresentation()
{
    const int ST_ACCEPT = 1001;
    auto word = _startWordForMergedRepr;
    //const char seq[] = {'/', '[', 'r', 'e','p', ']', 'H'};
    const char seq[] = {'/', '[', 's', 'c','o', ']', ' '};
    const size_t seq_length = 7;

    int count = 0;
    auto curr = word;

    const lexer_word_repr* nextDfa;

    //search for a beginning
    do
    {
        std::cout << "(id, input-idx, input): " << curr->getId() << ", " << count << ", " << seq[count] << std::endl;
        //change the origanization of this to just use input and not state
        auto aNextDfa = curr->getNextDfaForInput(seq[count]);
        count++; 

       if (aNextDfa != nullptr)
        {
                nextDfa = aNextDfa;

                std::cout << "break" << std::endl;

                break;
        }
    } while(count < seq_length);

    while ((nextDfa != nullptr) && (nextDfa->getId() != ST_ACCEPT && count < seq_length))
    {
        curr = const_cast<lexer_dfa*>(nextDfa);
        nextDfa = curr->getNextDfaForInput(seq[count]);

        std::cout << "(id, input-idx, input): " << curr->getId() << ", " << count << ", " << seq[count] << std::endl;

        if (nextDfa == nullptr)
        {
            //reset state to state at word_base
            curr = word;

            //search for a beginning
            do
            {
                //reset current dfa
                curr = word;

                std::cout << "(id, input-idx, input): " << curr->getId() << ", " << count << ", " << seq[count] << std::endl;

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

    if (nextDfa == nullptr)
    {
        std::cout << "nothing" << std::endl;
        return false;
    }
    else if (nextDfa->getId() == ST_ACCEPT)
    {
        std::cout << "found word!" << std::endl;
        return true;
    }
    else if (count >= seq_length)
    {
        std::cout << "reached end of input" << std::endl;
        return false;
    }
    else
    {
        return false;
    }
}
     
    bool _mergeWords()
    {
        std::vector<lexer_dfa*>* dfaWords = new std::vector<lexer_dfa*>();

        for (auto aWordTuple : _words)
        {
            std::cout << "Pushing back dfa to mergeDfas parameter: dfa(" << aWordTuple.first->getId() << ")" << std::endl;
            dfaWords->push_back(aWordTuple.first);
        }

        _startWordForMergedRepr = _lexer_builder->mergeDfas(dfaWords, dfaManager);
        bool resultOfMergedDataTest = _testMergedRepresentation();
        
        return resultOfMergedDataTest;
    }
public:
    lexer_word_constructor()
    {
        _initWords();
        _mergeWords();
    }

    ~lexer_word_constructor()
    {
        std::cout << "Destructor for Lexer Word Constructor called" << std::endl; 
        _destructDfasAndTransitions();
        std::cout << "Destructor for Lexer Word Constructor was successfull" << std::endl;

        delete _lexer_builder;
    }

    const std::vector<std::pair<lexer_word_repr*, AggregatePtrsAndDelete<lexer_dfa*>*>>& getWords() 
    { 
        return _words;
    }
};

#endif
