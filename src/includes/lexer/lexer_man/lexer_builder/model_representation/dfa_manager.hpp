
#include <vector>
#include "finite_autonoma/lexer_dfa.hpp"
#include "finite_autonoma/DfaTransition.hpp"

#ifndef _DFA_MANAGER_
#define _DFA_MANAGER_

class DfaManager
{
private:
	std::vector<lexer_dfa*> _dfas;
	std::vector<DfaTransition*> _transitions;
public:
	DfaManager() {}
	~DfaManager() {}

	lexer_dfa* createDfa(int id) 
	{
		lexer_dfa* ret = new lexer_dfa(id);
		_dfas.push_back(ret);
		return ret;
	}
	lexer_word_repr* createLexerWordRepr(int id) {return createDfa(id);}

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

		return true;
	}

	DfaTransition* createDfaTransition(StateAndInput<int,char>* stateAndInput,
		lexer_dfa* dfa_ptr)
	{
        	DfaTransition* ret = new DfaTransition(stateAndInput , dfa_ptr);
		_transitions.push_back(ret);
        	return ret;
	}

	bool destroyDfaTransition(DfaTransition* toBeDestroyed)
	{
                bool wasTransitionDestroyed = false;

                size_t removeIndex;
                DfaTransition* entryToBeDestroyed = nullptr;
		const size_t transitionsSize = _transitions.size();
                for (size_t index = 0; index < transitionsSize; index++)
                {
                        DfaTransition* entry = _transitions.at(index);
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

                return true;		
	}
};

#endif
