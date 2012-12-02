/*
 REPSS
 Copyright (C) 2012  Khalique Williams

 This file is part of REPSS.

 REPSS is free software: you can redistribute it and/or modify
 it under the terms of the   GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 REPSS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with REPSS.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _GRAMMAR_MANAGER_
#define _GRAMMAR_MANAGER_

/*#undef*/ #define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

#include <stdio.h>

class GrammarManager
{
private:
    IGrammarDataProxy *_grammarDataProxy; 
public:
    GrammarManager(IGrammarContext* context)
    {
        DeLOG("Constructor for GrammarManager\n");

        //...from configuration get grammar keywords and rules etc

        _grammarDataProxy = nullptr;
        //create grammarDataProxy and transfer config fata to it through constructor
        //set the grammarDataProxy in Context
    }

    void init(const GrammarConfig& config)
    {
        if (_grammarDataProxy != nullptr)
        {

            perror("GrammarManager/GrammarDataProxy have already been initialiazed. Doing nothing");
            return;
        }

        //auto grammarKeywords = ...

        DeLOG("Initialized Lexer Data Proxy Successfully.\nSucessfully Initialized GrammarManager\n");
    }

    const IGrammarDataProxy* getGrammarDataProxy() const
    {
        return _grammarDataProxy;
    }

    ~GrammarManager()
    {
        DeLOG("Destroying GrammarManager\n");

        if (_grammarDataProxy != nullptr)
            delete _grammarDataProxy;
    }
};

#endif
