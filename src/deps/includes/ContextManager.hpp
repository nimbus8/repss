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

#ifndef _CONTEXT_MANAGER_
#define _CONTEXT_MANAGER_

#include "lexer/construction/ScanWordNode.hpp" 
#include "lexer/lexer_context/LexerContext.hpp"

class ContextType
{
public:
	typedef enum
	{
		NoType=1, Lexer, Parser
	} AllowedTypes;
};

class Context
{
private:
    std::vector<std::string> _annotatedData;
    ScanWords* _scanWords;

protected:
    void initScanWordsImpl(ContextType::AllowedTypes contextTypeAsLexer, const ScanWords* const scanWords)
    {
        if (contextTypeAsLexer != ContextType::Lexer)
        {
            return;
        }

        if (_scanWords != nullptr)
        {
            std::cout << "Warning, trying to initialize context TWICE - this is NOT allowed" << std::endl;
            return;
        }

        _scanWords = const_cast<ScanWords*>(scanWords);
    }

    const ScanWords* const getScanWordsImpl(ContextType::AllowedTypes contextTypeAsLexer) const
    {
        if (contextTypeAsLexer != ContextType::Lexer)
        {
            return nullptr;
        }

        return _scanWords;
    }

    void appendToAnnotatedData(ContextType::AllowedTypes contextTypeAsLexer,
        const std::string& data)
    {
        if (contextTypeAsLexer == ContextType::Lexer)
        {
            _annotatedData.push_back(data);
        }
    }

    void printAnnotatedData(ContextType::AllowedTypes contextTypeAsLexer) const
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

public:
	Context() {}
	~Context() {}
};	

class ContextManager
{
private:
	Context _globalContext;
public:
        typedef ContextType::AllowedTypes _AllowedTypes_;

	template<class T, T V>
	class TypedContext : public Context
	{
	public:
//                virtual void initScanWords(const ScanWords* const scanWords);
//                virtual const ScanWords* const getScanWords() const;
		void appendToAnnotatedData(const std::string& data) {}
		void printAnnotatedData() const {}

                void doMe() { Context::doMe(ContextType::NoType); }
	};

	//default template for getContext - rest in implementation file
	template<class T, T V>
	TypedContext<T, V> getContext()
	{
		TypedContext<T, V> ret;
		return ret;
	}

	Context& requestContext(ContextType type);
};

template<>
class ContextManager::TypedContext <ContextType::AllowedTypes, ContextType::Lexer>
    : public Context, public ILexerContext
{
public:
        virtual void initScanWords(const ScanWords* const scanWords)
        {
            Context::initScanWordsImpl(ContextType::Lexer, scanWords);
        }

        virtual const ScanWords* const getScanWords() const
        {
            std::cout << "CtxMan TypedContext <> getScanWords" << std::endl;
            return Context::getScanWordsImpl(ContextType::Lexer);
        }

        void appendToAnnotatedData(const std::string& data)
        {
                std::cout << "CtxMan TypedContext <> addToAnnotatedData" << std::endl;
                Context::appendToAnnotatedData(ContextType::Lexer, data);
        }
        void printAnnotatedData() const
        {
                std::cout << "CtxMan TypedContext <> printAnnotatedData" << std::endl;
                Context::printAnnotatedData(ContextType::Lexer);
        }

        void doMe() { Context::doMe(ContextType::Lexer); }
};

template<>
class ContextManager::TypedContext <ContextType::AllowedTypes, ContextType::Parser> : public Context
{
public:
        void doMe() { Context::doMe(ContextType::Parser); }
};

template<>
class ContextManager::TypedContext <ContextType::AllowedTypes, ContextType::NoType> : public Context
{
public:
        void doMe() { Context::doMe(ContextType::NoType); }
};

int TestContextMan();

#endif
