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

#include <vector>

#ifndef _DFA_MANAGER_
#define _DFA_MANAGER_

#include "finite_autonoma/lexer_dfa.hpp"
#include "finite_autonoma/DfaTransition.hpp"

class DfaManager
{
private:
    int _idCount;

    std::vector<lexer_dfa*> _dfas;
    std::vector<const DfaTransition*> _transitions;

    class DfaIdHashFunction {
    public:
        ::std::size_t operator ()(const int &id) const
        {
            return (size_t) id;
        }
    };

    class DfaIdEquals {
    public:
        bool operator ()(const int &lhs, const int &rhs) const
        {
            return lhs == rhs;
        }
    };	

    std::unordered_map<int, std::string, DfaIdHashFunction, DfaIdEquals> _endStateNameMap;
public:
    DfaManager() : _idCount(0) {}
    ~DfaManager() {}

    lexer_dfa* createDfa() 
    {
        lexer_dfa* ret = new lexer_dfa(++_idCount);
        _dfas.push_back(ret);
        return ret;
    }

    lexer_dfa* createAcceptingDfa(const std::string endStateName)
    {
        //todo: change this...1001 refers to const defined in word construction
        //	we want to change this to taking in a string to defined a named
        //	accepting state. - Done (now testing)
        int nextIdCount = _idCount + 1;

        std::cout << "CREATING end state mapping: endStateToName=(" << nextIdCount
                  << "=>" << endStateName << ")" << std::endl; 

        std::pair<int, std::string> idToEndStateName{nextIdCount, endStateName};
        _endStateNameMap.emplace(idToEndStateName);

        lexer_dfa* ret = new lexer_dfa(nextIdCount);
        _dfas.push_back(ret);
        return ret;
    }

    bool isAcceptingNode(const int id) const
    {
        std::cout << "isAcceptingNode called: id=(" << id << ")" << std::endl;
        auto fetched = _endStateNameMap.find(id);

        if (fetched == _endStateNameMap.end())
        {
            std::cout << "isAcceptingNode RESPONSE false" << std::endl;
            return false;
        }

        std::cout << "isAcceptingNode RESPONSE: true" << std::endl;
        return true;
    }

    std::string getAcceptingNodeName(const int id) const
    {
        auto fetched = _endStateNameMap.find(id);
        if (fetched == _endStateNameMap.end())
        {
            std::string blankString("");
            return blankString;
        }

        return fetched->second;
    }

    lexer_word_repr* createLexerWordRepr() {return createDfa();}

    bool destroyDfa(lexer_dfa* toBeDestroyed) 
    {
        bool wasDfaDestroyed = false;

        size_t removeIndex; 
        lexer_dfa* entryToBeDestroyed = nullptr;
        for (size_t index = 0; index < _dfas.size(); index++)
        {
            lexer_dfa* entry = _dfas.at(index);
            if (entry == toBeDestroyed)
            {
                removeIndex = index;
                entryToBeDestroyed = entry;
                toBeDestroyed = nullptr;
                break;
            }
        }	

        if (toBeDestroyed != nullptr)
        {
            wasDfaDestroyed = false;
        } 
        else 
        {
            _dfas.erase(_dfas.begin()+removeIndex);
            delete entryToBeDestroyed;
            wasDfaDestroyed = true;
        }

        return wasDfaDestroyed;
    }

    const DfaTransition* createDfaTransition(const StateAndInput<int,char>* stateAndInput,
        const lexer_dfa* dfa_ptr)
    {
        const DfaTransition* ret = new DfaTransition(stateAndInput , dfa_ptr);
        _transitions.push_back(ret);
        return ret;
    }

	bool destroyDfaTransition(DfaTransition* toBeDestroyed)
	{
                bool wasTransitionDestroyed = false;

                size_t removeIndex;
                const DfaTransition* entryToBeDestroyed = nullptr;
		const size_t transitionsSize = _transitions.size();
                for (size_t index = 0; index < transitionsSize; index++)
                {
                        const DfaTransition* const entry = _transitions.at(index);
                        if (entry == toBeDestroyed)
                        {
                                removeIndex = index;
                                entryToBeDestroyed = entry;

                                toBeDestroyed = nullptr;
                                break;
                        }
                }

                if (toBeDestroyed != nullptr)
                {
                        wasTransitionDestroyed = false;
                }
                else
                {
                        _transitions.erase(_transitions.begin()+removeIndex);
                        delete entryToBeDestroyed;
                        wasTransitionDestroyed = true;
                }

                return wasTransitionDestroyed;		
	}
};

#endif
