
#include <vector>
#include "finite_autonoma/lexer_dfa.hpp"
#include "finite_autonoma/DfaTransition.hpp"

#ifndef _DFA_MANAGER_
#define _DFA_MANAGER_

class DfaManager
{
private:
	int _idCount;

	std::vector<lexer_dfa*> _dfas;
	std::vector<const DfaTransition*> _transitions;

	std::unordered_map<int, std::string> _endStateNameMap;

public:
	DfaManager() : _idCount(0) {}
	~DfaManager() {}

	lexer_dfa* createDfa() 
	{
		lexer_dfa* ret = new lexer_dfa(_idCount++);
		_dfas.push_back(ret);
		return ret;
	}

	lexer_dfa* createAcceptingDfa()
	{
		//todo: change this...1001 refers to const defined in word construction
		//	we want to change this to taking in a string to defined a named
		//	accepting state.
                lexer_dfa* ret = new lexer_dfa(1001);
                _dfas.push_back(ret);
                return ret;
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
