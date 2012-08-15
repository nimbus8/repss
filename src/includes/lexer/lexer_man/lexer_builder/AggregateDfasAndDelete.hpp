#include "../../utils/AggregatePtrsAndDelete.hpp"
#include "model_representation/dfa_manager.hpp"

#ifndef _AGGREGATE_DFAS_AND_DELETE_
#define _AGGREGATE_DFAS_AND_DELETE_

template <class T>
class AggregateDfasAndDelete : public AggregatePtrsAndDelete<T>
{
public:
	AggregateDfasAndDelete() : AggregatePtrsAndDelete<T>(0) {}
	AggregateDfasAndDelete(const size_t num, ...) : AggregatePtrsAndDelete<T>(num) 
	{
                va_list arguments;
                va_start(arguments, num);

                this->init(num, arguments);

                va_end(arguments);
                std::cout << std::endl;
	}
	~AggregateDfasAndDelete() 
	{
		std::cout << "Destructor for AggregateDfasAndDelete called"
			<< std::endl;
	}

	bool applyDelete(DfaManager& dfaManager)
	{
                if (this->size() == 0)
                {
                        return false;
                }

                for (int index = 0; index < this->size(); index++)
                {
                        T obj = this->getAt(index);
                        std::cout << "Destroying Dfa id(" << obj 
				<< ") the RIGHT way." << std::endl;
                        dfaManager.destroyDfa(obj);
                }

                return true;
	}
};

#endif
