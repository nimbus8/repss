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

//Should you modify this file? NO

#ifndef _ABSTR_GRAMMAR_CONFIG_
#define _ABSTR_GRAMMAR_CONFIG

#include <string>
#include <vector>

#include "../../../src/lib/includes/Genrtd_Keywords.hpp"
#include "ILanguageAndGrammar.hpp"

#include "execution_phase/grammaticalForm/GrammarTypedefs.hpp"
#include "execution_phase/grammaticalForm/GrammarConstruct.hpp"
#include "utils/ReadOnlyData.hpp"

#define stringify( name ) # name

class AbstrGrammarConfig
{
private:
    Keywords _keywords;
protected:
    ReadOnlyData<std::vector<std::tuple<std::string, std::string, std::vector<GrammarRules::Term*>>>>* _grammarRules;

    virtual ~AbstrGrammarConfig()
    {
        delete _grammarRules;
    }
    AbstrGrammarConfig()
    {
    }

    void _init()
    {
        std::vector<std::tuple<std::string, std::string, std::vector<GrammarRules::Term*>>> _grammarRulesData;

        auto grammarRules__Keyword_REPS_withNamedIteration = _defineGrammarKeyword_Keyword_REPS_withNamedIteration(std::make_pair("reps.named_iteration", _keywords.getGrammarTypeForName("reps.named_iteration")));
        _grammarRulesData.push_back(std::make_tuple("reps.named_iteration", "VARIABLE", grammarRules__Keyword_REPS_withNamedIteration));

        auto grammarRules__SquareBracketReps = _defineGrammarKeyword_SquareBracketReps(std::make_pair("scope", _keywords.getGrammarTypeForName("scope")));
        _grammarRulesData.push_back(std::make_tuple("scope", "VARIABLE", grammarRules__SquareBracketReps));

        auto grammarRules__AlterationAndJoin = _defineGrammarKeyword_AlterationAndJoin(std::make_pair("alternation.andJoin", _keywords.getGrammarTypeForName("alternation.andJoin")));
        _grammarRulesData.push_back(std::make_tuple("alternation.andJoin", "TERMINAL", grammarRules__AlterationAndJoin));

        auto grammarRules__Alteration = _defineGrammarKeyword_Alteration(std::make_pair("alternation", _keywords.getGrammarTypeForName("alternation")));
        _grammarRulesData.push_back(std::make_tuple("alternation", "TERMINAL", grammarRules__Alteration));

        auto grammarRules__Keyword_eval = _defineGrammarKeyword_Keyword_eval(std::make_pair("evaluation", _keywords.getGrammarTypeForName("evaluation")));
        _grammarRulesData.push_back(std::make_tuple("evaluation", "TERMINAL", grammarRules__Keyword_eval));

        auto grammarRules__End = _defineGrammarKeyword_End(std::make_pair("general_end", _keywords.getGrammarTypeForName("general_end")));
        _grammarRulesData.push_back(std::make_tuple("general_end", "CLOSURE", grammarRules__End));

        auto grammarRules__Keyword_REPS_withNamedListIteration = _defineGrammarKeyword_Keyword_REPS_withNamedListIteration(std::make_pair("reps.named_list_iteration", _keywords.getGrammarTypeForName("reps.named_list_iteration")));
        _grammarRulesData.push_back(std::make_tuple("reps.named_list_iteration", "VARIABLE", grammarRules__Keyword_REPS_withNamedListIteration));

        _grammarRules = new ReadOnlyData<std::vector<std::tuple<std::string, std::string, std::vector<GrammarRules::Term*>>>>(static_cast<std::vector<std::tuple<std::string, std::string, std::vector<GrammarRules::Term*>>>&&>(_grammarRulesData));
    }

    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_Keyword_REPS_withNamedIteration(std::pair<std::string, std::string> nameAndGrammarType__Keyword_REPS_withNamedIteration__VARIABLE) = 0;
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_SquareBracketReps(std::pair<std::string, std::string> nameAndGrammarType__SquareBracketReps__VARIABLE) = 0;
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_AlterationAndJoin(std::pair<std::string, std::string> nameAndGrammarType__AlterationAndJoin__TERMINAL) = 0;
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_Alteration(std::pair<std::string, std::string> nameAndGrammarType__Alteration__TERMINAL) = 0;
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_Keyword_eval(std::pair<std::string, std::string> nameAndGrammarType__Keyword_eval__TERMINAL) = 0;
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_End(std::pair<std::string, std::string> nameAndGrammarType__End__CLOSURE) = 0;
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_Keyword_REPS_withNamedListIteration(std::pair<std::string, std::string> nameAndGrammarType__Keyword_REPS_withNamedListIteration__VARIABLE) = 0;
    //here we have helper functions for the virtual functions to be defined in subclass
    class VARIABLE
    {
    public:
        std::function<GrammarConstruct()> GET_VARIABLE =
          []()
          {
              return GrammarConstruct(
                  "var", "VARIABLE",
                  std::vector<GrammarRules::Term*>{
                      new GrammarRules::VariableRef("reps.named_iteration"),
                      new GrammarRules::VariableRef("scope"),
                      new GrammarRules::VariableRef("reps.named_list_iteration"),
                  }
              );
          };
    };
};

#endif
