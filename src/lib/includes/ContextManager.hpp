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

#include "utils/ConstVector.hpp"

#ifndef _CONTEXT_MANAGER_
#define _CONTEXT_MANAGER_

#include "execution_phase/lexer/construction/ScanWordNode.hpp" 

#include "execution_phase/lexer/ILexerContext.hpp"
#include "execution_phase/grammaticalForm/IGrammarContext.hpp"

#define DEBUG
//#undef DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
#else
    #define DeLOG(str)
#endif

class ContextType
{
public:
    typedef enum
    {
        NoType=1, Lexer, Parser, Grammar, Analysis, Generation
    } AllowedTypes;
};

typedef ContextType::AllowedTypes _AllowedTypes_;

class Context
{
private:
    //Lexer Specific
    ILexerDataProxy* _lexerDataProxy; //this is to access constant objects of dfaManager, scanWords, etc
    std::vector<std::string> _annotatedData;
    ScanWords* _scanWords; //put this into LexerDataProxy

    //Parser Specific
    std::vector<std::string> _parserAnnotatedData;

    //Grammar Specific

    //Analysis Specific

    //Generation Specific
    std::vector<std::string> _generatorAnnotatedData;

public:
    Context() : _lexerDataProxy(nullptr), _scanWords(nullptr) {}

    void initLexerDataProxyImpl(ContextType::AllowedTypes contextTypeAsLexer, const ILexerDataProxy* lexerDataProxy)
    {
        if (contextTypeAsLexer != ContextType::Lexer)
        {
            std::cout << "initLexerDataProxImpl:: not ContextType::Lexer" << std::endl;
            return;
        }

        if (_lexerDataProxy != nullptr)
        {
            std::cout << "Warning, trying to initialized lexerDataProxy TWICE - this is NOT allowed" << std::endl;
            return;
        }

        std::cout << "initLexerDataProxyImpl:: just about to initialize lexer data proxy with const cast" << std::endl;

        _lexerDataProxy = const_cast<ILexerDataProxy*>(lexerDataProxy);
    }

    const ILexerDataProxy* getLexerDataProxyImpl(ContextType::AllowedTypes) const
    {
        return _lexerDataProxy;
    }

    void setAnnotatedDataImpl(ContextType::AllowedTypes contextTypeAsLexer,
        const std::vector<std::string>& data)
    {
        if (contextTypeAsLexer == ContextType::Lexer)
        {
            size_t numOfEntries = data.size();
            std::cout << "Setting annotated data with vector size = " << numOfEntries << std::endl;
            for (int i = 0; i < numOfEntries; i++)
            {
               const std::string entry(data.at(i));
                _annotatedData.push_back(entry);
            }
        }
    }

    ConstVector<std::string> getAnnotatedDataImpl(ContextType::AllowedTypes contextType)
    {
        //this is a terrible way BTW. todo: create a wrapper class for 
        //the vector(const) and return it.

        if (contextType == ContextType::Grammar)
        {
           ConstVector<std::string> retVector(&_annotatedData);
           return retVector; 
        }
        else  if (contextType == ContextType::Parser)
        {
            ConstVector<std::string> retVector(&_annotatedData);
            return retVector;
        }

        return ConstVector<std::string>(nullptr);
    }

    void printAnnotatedDataImpl(ContextType::AllowedTypes contextTypeAsLexer) const
    {
        if (contextTypeAsLexer == ContextType::Lexer)
        {
            for (auto blockStr : _annotatedData)
            {
                std::cout << blockStr << std::endl;
            }
        }
    }

    //example: doMe - how and how not to
    void doMe(int something)
    { 
        std::cout << "Ooops. Way off - Context Not Being Retrieved Correctly." 
                  << std::endl; 
    }

    void doMe(ContextType::AllowedTypes something) {
        switch(something)
        {
            case ContextType::Lexer:
            std::cout << "Lexeme!" << std::endl;
            break;

            case ContextType::Parser:
            std::cout << "Parse!" << std::endl;
            break;

            case ContextType::NoType:
            std::cout << "No Type - Invalid" << std::endl;
            break;

            default:
            std::cout << "Wrong usage of API - AlloweTypes::" << std::endl;
            break;
        }
    };

    Context(Context* ctx)
    {
        _lexerDataProxy = ctx->_lexerDataProxy;
        _annotatedData = ctx->_annotatedData;
        _scanWords = ctx->_scanWords;      
    }
    ~Context() { DeLOG("Context was successfully destroyed\n"); }
};	

typedef ContextType::AllowedTypes _AllowedTypes_;


class ContextManager
{
private:
	Context _globalContext;
public:
        typedef ContextType::AllowedTypes _AllowedTypes_;

	template<class T, T V>
	class TypedContext
	{
        private:
            Context* _refContext;
	public:
            TypedContext() : _refContext(nullptr) {}   
            explicit TypedContext(Context* ctx) : _refContext(ctx) {}
            ~TypedContext() {}
            //void doMe() { Context::doMe(ContextType::NoType); }
	};

	//default template for getContext - rest in implementation file
	template<class T, T V>
	TypedContext<T, V> getContext()
	{
		TypedContext<T, V> ret(&_globalContext);
		return ret;
	}

	//Context& requestContext(ContextType type);
};

template<>
class ContextManager::TypedContext <_AllowedTypes_, ContextType::Lexer>
    : virtual public ILexerContext
{
private:
    Context* _refContext;
public:
    ContextManager::TypedContext() {}
    explicit TypedContext(Context* ctx) : _refContext(ctx) {}

    ~TypedContext()
    {
    }

    virtual void initLexerDataProxy(const ILexerDataProxy* lexerDataProxy)
    {
        _refContext->initLexerDataProxyImpl(ContextType::Lexer, lexerDataProxy);
    }

    virtual const ILexerDataProxy* getLexerDataProxy() const
    {
        return _refContext->getLexerDataProxyImpl(ContextType::Lexer);
    }

    virtual void setAnnotatedData(const std::vector<std::string>& data)
    {
        std::cout << "CtxMan TypedContext <> setAnnotatedData" << std::endl;
        _refContext->setAnnotatedDataImpl(ContextType::Lexer, data);
    }

    virtual void printAnnotatedData() const 
    {
        std::cout << "CtxMan TypedContext <> printAnnotatedData" << std::endl;
        _refContext->printAnnotatedDataImpl(ContextType::Lexer);
    }

    void doMe() { _refContext->doMe(ContextType::Lexer); }
};

template<>
class ContextManager::TypedContext <ContextType::AllowedTypes, ContextType::Parser> : public Context
{
private:
    Context* _refContext;
public:
    TypedContext() {}
    explicit TypedContext(Context* ctx) : _refContext(ctx) {}
    void doMe() { _refContext->doMe(11); }
};

template<>
class ContextManager::TypedContext <_AllowedTypes_, ContextType::Grammar> : public IGrammarContext
{
private:
    Context* _refContext;
public:
    ContextManager::TypedContext() {}
    explicit TypedContext(Context* ctx) : _refContext(ctx) {}

    ~TypedContext()
    {
    }

    void doMe() { _refContext->doMe(ContextType::Grammar); }
};

//'Analysis' Context-Type {comprehension, conception, reorganization}
template<>
class ContextManager::TypedContext <_AllowedTypes_, ContextType::Analysis> : public Context
{
private:
    Context* _refContext;
public:
    TypedContext() {}
    explicit TypedContext(Context* ctx) : _refContext(ctx) {}

    ~TypedContext()
    {
    }

    void doMe() { _refContext->doMe(ContextType::Analysis); }
};

//'Generator' Context-Type {generation, [optional:feed_back]}
template<>
class ContextManager::TypedContext <_AllowedTypes_, ContextType::Generation> : public Context
{
private:
    Context* _refContext;
public:
    TypedContext() {}
    explicit TypedContext(Context* ctx) : _refContext(ctx) {}

    ~TypedContext()
    {
    }

    void doMe() { _refContext->doMe(ContextType::Generation); }
};


template<>
class ContextManager::TypedContext <ContextType::AllowedTypes, ContextType::NoType> : public Context
{
private:
    Context* _refContext;
public:
    TypedContext() {}
    explicit TypedContext(Context* ctx) : _refContext(ctx) {}

    ~TypedContext()
    {
    }

    void doMe() { _refContext->doMe(ContextType::NoType); }
};

int TestContextMan();

#endif
