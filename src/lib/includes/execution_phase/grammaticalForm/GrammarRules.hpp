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

#ifndef _GRAMMAR_RULES_
#define _GRAMMAR_RULES_

#define DEBUG
//#undef DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

#include <string>
#include <vector>

#include "../../utils/AggregatePtrsAndDelete.hpp"
#include "../../utils/ConstantArrayAccess.hpp"
class GrammarRules
{
public:
    class Term
    {
    private:
        std::string _name;
    protected:
        virtual ~Term() {}
    public:
        Term() : _name(""){};
        explicit Term(const std::string& name) : _name(name) {}
        Term(const Term& other) : _name(other._name) {}

        const std::string& getName() const { return _name; }
    };

    class Terminal : public Term
    {
    public:
        explicit Terminal(std::string& name) : Term(name) {}
        Terminal(const Terminal& other) : Term(other.getName()) {}
        Terminal(Terminal&& other) : Term(other.getName()) {}
        ~Terminal() {}
    };

    class Variable : public Term
    {
    private:
        const AggregatePtrsAndDelete<Term*>* _directlyDerivedTerms;
    public:
        Variable(std::string& name, AggregatePtrsAndDelete<Term*>* directlyDerivedTerms)
            : Term(name), _directlyDerivedTerms(directlyDerivedTerms) {}
        Variable(const Variable& other)
            : Term(other.getName()), _directlyDerivedTerms(other._directlyDerivedTerms) {}
        Variable(Variable&& other)
            : Term(other.getName()), _directlyDerivedTerms(nullptr)
        {
            _directlyDerivedTerms = other._directlyDerivedTerms;
            other._directlyDerivedTerms = nullptr;
        }
        ~Variable()
        {
           delete _directlyDerivedTerms;
        }

        //todo:will change this to use ConstVector
        ConstantPtrArrayAccess<Term*> getDerivations() const
        {
            return ConstantPtrArrayAccess<Term*>(_directlyDerivedTerms);
        }
    };

    class Closure : public Term
    {
    public:
        Closure(std::string& name) : Term(name) {}
        ~Closure() {}
    };


    //for variables in particular we have to give them references
    //because if were to define a term that could produce a variable
    //that variable is technically undefined as each term is defined
    //in isolation during construction. Even at runtime, a variable
    //isn't guaranteed to have already been defined and it is a choice that we
    //not make moves to define that variable before finishing up the defn of 
    //the term that makes reference to it. Still wanting to treat Terms (variables in particular)
    //as contants, we require that there be only one Variable object constructed for a particular
    //variable, the and any mention of that variable in another term is done with 
    //a VariableReference object. Which like Very other term(excluding Variable) contains only the name -- .
    //the entire class acts as a key for some data store map to retrieve
    //actual full definition of Variable.
    class VariableRef : public Term
    {
    public:
        explicit VariableRef(const std::string& name) : Term(name) {}
        VariableRef(const VariableRef& other)
            : Term(other.getName()) {}
        VariableRef(VariableRef&& other) : Term(other.getName()) {}
    };
private:
    std::vector<Term*> _rules;
};

#undef DEBUG
#undef DeLOG
#endif

