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

#include "../../../utils/AggregateAndApplyFuncBase.hpp"
#include "../../../utils/ApplyImmutableFunc.hpp"

#include "model_representation/dfa_manager.hpp"
#include "model_representation/finite_autonoma/lexer_dfa.hpp"
#include "model_representation/finite_autonoma/DfaTransition.hpp"

#ifndef _LEXER_DFA_BUILDER_
#define _LEXER_DFA_BUILDER_

//Helper class for lexer_word_constructor

class lexer_dfa_builder
{
public:
        lexer_dfa_builder() {}
        ~lexer_dfa_builder() {}

        bool addDfa(lexer_word_repr* word_base, const ApplyImmutableFunc<DfaTransition*>& applyObj) const
        {
                std::function<bool (const AggregateAndApplyFuncBase<DfaTransition*>*)> applyFunc =
                        [&word_base,applyObj](const AggregateAndApplyFuncBase<DfaTransition*>* aggr)->bool
                        {
                                //adds to the word base, all InputAndDfaNode objects defined in args list
                                for (int index = 0; index < applyObj.size(); index++)
                                {
                                        DfaTransition* stateInputAndDfa = applyObj.getAt(index);
                                        word_base->add_next_dfa(*(stateInputAndDfa->getStateAndInput()), stateInputAndDfa->getDfaNode());
                                }

                                return true;
                        };

                const bool wasSuccess = applyObj.apply(applyFunc);

                return wasSuccess;
        }

        lexer_word_repr* mergeDfas(std::vector<lexer_word_repr*>& words, DfaManager& dfaManager);
};

#endif
