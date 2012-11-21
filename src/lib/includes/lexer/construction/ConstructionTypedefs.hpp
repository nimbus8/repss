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

//This file should hold all typedefs specific to "construction"

#ifndef _CONSTRUCTION_TYPEDEFS_
#define _CONSTRUCTION_TYPEDEFS_

#include "model_representation/dfa_manager.hpp"

#include "../../utils/AggregatePtrsAndDelete.hpp"

typedef lexer_dfa* lexer_dfa_ptr_t;
typedef lexer_word_repr* lexer_word_repr_ptr_t;

typedef std::pair <lexer_word_repr_ptr_t, AggregatePtrsAndDelete<lexer_dfa_ptr_t>*> word_start_and_aggregated_nodes_Pair_t;

typedef AggregatePtrsAndDelete<DfaTransition*>* aggregated_transitions_ptr_t;


typedef std::pair<word_start_and_aggregated_nodes_Pair_t, aggregated_transitions_ptr_t> wordrepr_and_transition_Pair_t;


#endif

