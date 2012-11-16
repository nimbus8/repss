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

#define DEBUG
//#undef DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

#include <string>
#include <vector>

class GrammerRules::Term;

class GrammarRules
{
private:
    std::vector<GrammerRules*> _rules;
public:
    class Term
    {
    private:
        std::string _name;
    public:
        Term() _name(""){};
        Term(Term& other) : _name(other._name) {}
        virtual ~Term() = 0;

        const std::string& getName() const { return _name; }
    };

    class Terminal : public Term
    {
    public:
        Terminal(std::string& name) : Term(name) {}
        ~Terminal() {}
    };

    class Variable : public Term
    {
    private:
        const AggregatePtrsAndDelete<Term*>* _directlyDerivedTerms;
    public:
        explicit Variable(std::string& name, AggregatePtrsAndDelete<Term*>* directlyDerivedTerms)
            : Term(name), _directlyDerivedTerms(directlyDerivedTerms) {}

        //move constructor? try it
        std::vector<Terms>& getDerivations();

        ~Variable() 
        {
           delete _directlyDerivedTerms;
        }
    };
};

class ILanguageAndGrammar
{
public:
    getLanguageKeywords();
    getGrammarRules();
};
