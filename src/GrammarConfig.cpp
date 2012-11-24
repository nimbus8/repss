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

#include "lib/includes/execution_phase/grammaticalForm/GrammarConfig.hpp"
#include "lib/includes/execution_phase/grammaticalForm/GrammarKeywordDefn.hpp"

#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

    std::vector<GrammarRules::Term*> GrammarConfig::_defineGrammarKeyword_Keyword_REPS_withNamedIteration(std::pair<std::string, std::string> nameAndGrammarType__Keyword_REPS_withNamedIteration__VARIABLE)
    {
        std::vector<GrammarRules::Term*> topLevelTerms;
        return topLevelTerms;
    }

    std::vector<GrammarRules::Term*> GrammarConfig::_defineGrammarKeyword_SquareBracketReps(std::pair<std::string, std::string> nameAndGrammarType__SquareBracketReps__VARIABLE)
    {
        std::vector<GrammarRules::Term*> topLevelTerms;
        return topLevelTerms;
    }

    std::vector<GrammarRules::Term*> GrammarConfig::_defineGrammarKeyword_Alteration(std::pair<std::string, std::string> nameAndGrammarType__Alteration__TERMINAL)
    {
        std::vector<GrammarRules::Term*> topLevelTerms;
        return topLevelTerms;
    }

    std::vector<GrammarRules::Term*> GrammarConfig::_defineGrammarKeyword_Keyword_eval(std::pair<std::string, std::string> nameAndGrammarType__Keyword_eval__TERMINAL)
    {
        std::vector<GrammarRules::Term*> topLevelTerms;
        return topLevelTerms;
    }

    std::vector<GrammarRules::Term*> GrammarConfig::_defineGrammarKeyword_End(std::pair<std::string, std::string> nameAndGrammarType__End__TERMINAL)
    {
        std::vector<GrammarRules::Term*> topLevelTerms;
        return topLevelTerms;
    }

    std::vector<GrammarRules::Term*> GrammarConfig::_defineGrammarKeyword_Keyword_REPS_withNamedListIteration(std::pair<std::string, std::string> nameAndGrammarType__Keyword_REPS_withNamedListIteration__VARIABLE)
    {
        std::vector<GrammarRules::Term*> topLevelTerms;
        return topLevelTerms;
    }

#undef DEBUG
#undef DeLOG
