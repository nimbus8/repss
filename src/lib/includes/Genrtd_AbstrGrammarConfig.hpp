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
#ifndef _ABSTR_GRAMMAR_CONFIG_
#define _ABSTR_GRAMMAR_CONFIG

#include <string>
#include <vector>
class AbstrGrammarConfig
{
private:
    Keywords _keywords;
protected:
    ReadOnlyData<std::tuple<std::string, std::string, std::vector<GrammarRules::Term>>> _grammarRules;

    _defineGrammarKeyword_Keyword_REPS_withNamedIteration(std::pair<std::string, std::string> nameAndGrammarType__Keyword_REPS_withNamedIteration__VARIABLE) == 0;
    _defineGrammarKeyword_SquareBracketReps(std::pair<std::string, std::string> nameAndGrammarType__SquareBracketReps__VARIABLE) == 0;
    _defineGrammarKeyword_Alteration(std::pair<std::string, std::string> nameAndGrammarType__Alteration__TERMINAL) == 0;
    _defineGrammarKeyword_Keyword_eval(std::pair<std::string, std::string> nameAndGrammarType__Keyword_eval__TERMINAL) == 0;
    _defineGrammarKeyword_End(std::pair<std::string, std::string> nameAndGrammarType__End__TERMINAL) == 0;
    _defineGrammarKeyword_Keyword_REPS_withNamedListIteration(std::pair<std::string, std::string> nameAndGrammarType__Keyword_REPS_withNamedListIteration__VARIABLE) == 0;

    void _init()
    {
        auto grammarRules__Keyword_REPS_withNamedIteration = _defineGrammarKeyword_Keyword_REPS_withNamedIteration(std::make_pair("reps.named_iteration", _keywords.getTypeForKeywordName("reps.named_iteration"));
        _grammarRules.addRule("reps.named_iteration", "VARIABLE", grammarRules__Keyword_REPS_withNamedIteration);

        auto grammarRules__SquareBracketReps = _defineGrammarKeyword_SquareBracketReps(std::make_pair("scope", _keywords.getTypeForKeywordName("scope"));
        _grammarRules.addRule("scope", "VARIABLE", grammarRules__SquareBracketReps);

        auto grammarRules__Alteration = _defineGrammarKeyword_Alteration(std::make_pair("alternation", _keywords.getTypeForKeywordName("alternation"));
        _grammarRules.addRule("alternation", "TERMINAL", grammarRules__Alteration);

        auto grammarRules__Keyword_eval = _defineGrammarKeyword_Keyword_eval(std::make_pair("evaluation", _keywords.getTypeForKeywordName("evaluation"));
        _grammarRules.addRule("evaluation", "TERMINAL", grammarRules__Keyword_eval);

        auto grammarRules__End = _defineGrammarKeyword_End(std::make_pair("general_end", _keywords.getTypeForKeywordName("general_end"));
        _grammarRules.addRule("general_end", "TERMINAL", grammarRules__End);

        auto grammarRules__Keyword_REPS_withNamedListIteration = _defineGrammarKeyword_Keyword_REPS_withNamedListIteration(std::make_pair("reps.named_list_iteration", _keywords.getTypeForKeywordName("reps.named_list_iteration"));
        _grammarRules.addRule("reps.named_list_iteration", "VARIABLE", grammarRules__Keyword_REPS_withNamedListIteration);

    virtual ~AbstrGrammarConfig
    {
    }
};

#endif