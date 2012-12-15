#include "../../../utils/AggregatePtrsAndDelete.hpp"
#include "model_representation/dfa_manager.hpp"

#ifndef _AGGREGATE_DFA_TRANSITIONS_AND_DELETE_
#define _AGGREGATE_DFA_TRANSITIONS_AND_DELETE_

template <class T>
class AggregateDfaTransitionsAndDelete : public AggregatePtrsAndDelete<T>
{
public:
    AggregateDfaTransitionsAndDelete() : AggregatePtrsAndDelete<T>(0) {}
    AggregateDfaTransitionsAndDelete(const size_t num, ...) : AggregatePtrsAndDelete<T>(num) 
    {
        va_list arguments;
        va_start(arguments, num);

        this->init(num, arguments);

        va_end(arguments);
        std::cout << std::endl;
    }
    ~AggregateDfaTransitionsAndDelete() 
    {
        std::cout << "Destructor for AggregateDfaTransitionsAndDelete called"
                  << std::endl;
    }

    virtual bool applyDelete(DfaManager& dfaManager)
    {
        if (this->size() == 0)
        {
            return false;
        }

        for (int index = 0; index < this->size(); index++)
        {
            T obj = this->getAt(index);
            std::cout << "Destroying Dfa Transition id(" << obj 
                      << ") the RIGHT way." << std::endl;
            dfaManager.destroyDfaTransition(obj);
        }

        return true;
    }
};

#endif
