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

#ifndef _GRAMMAR_CONFIG_
#define _GRAMMAR_CONFIG_

#include <stdio.h>

#include "../../Genrtd_AbstrGrammarConfig.hpp"
#include "GrammarKeywords.hpp"

#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:  %s\n", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

class GrammarConfig : public AbstrGrammarConfig
{
private:
    std::GrammarKeywords grammarKeywords;
protected:
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_Keyword_REPS_withNamedIteration(std::pair<std::string, std::string> nameAndGrammarType__Keyword_REPS_withNamedIteration__VARIABLE);
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_SquareBracketReps(std::pair<std::string, std::string> nameAndGrammarType__SquareBracketReps__VARIABLE);
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_Alteration(std::pair<std::string, std::string> nameAndGrammarType__Alteration__TERMINAL);
 virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_AlterationAndJoin(std::pair<std::string, std::string> nameAndGrammarType__AlterationAndJoin__TERMINAL);
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_Keyword_eval(std::pair<std::string, std::string> nameAndGrammarType__Keyword_eval__TERMINAL);
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_End(std::pair<std::string, std::string> nameAndGrammarType__End__TERMINAL);
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_Keyword_REPS_withNamedListIteration(std::pair<std::string, std::string> nameAndGrammarType__Keyword_REPS_withNamedListIteration__VARIABLE);
    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_RecursiveAlteration(std::pair<std::string, std::string> nameAndGrammarType__RecursiveAlteration__TERMINAL);
public:
    GrammarConfig()
    {
        _init(); 
    }

    ~GrammarConfig()
    {
        DeLOG("Successfully Deleted Grammar Configuration\n");
    }
};


#undef DEBUG
#undef DeLOG
#endif

