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

#include "AggregateAndApplyFuncBase.hpp"
#include "../lexer/lexer_man/lexer_builder/model_representation/dfa_manager.hpp"

#ifndef _APPLY_MUTABLE_FUNC_
#define _APPLY_MUTABLE_FUNC_

template<class T>
class AggregatePtrsAndDelete : public AggregateAndApplyFuncBase<T>
{
private:
	bool _initEnabled;

        void _handleVargs(size_t num, va_list arglist)
        {
                for (size_t n = 0; n < num; n++)
                {
                        T argval = va_arg(arglist,T);
                        this->addAt(n, argval);
                }
        }

protected:
        void init(const size_t num, va_list arglist)
        {
		if (_initEnabled)
		{
                	_handleVargs(num, arglist);
			_initEnabled = false;
		}
		//throw exception or warning
        }
public:

        AggregatePtrsAndDelete() : AggregateAndApplyFuncBase<T>() {}
	/*
        AggregatePtrsAndDelete(const size_t num, ...) 
		: AggregateAndApplyFuncBase<T>(num), _initEnabled(false)
        {
                va_list arguments;
                va_start(arguments, num);
                
                _handleVargs(num,arguments);

                va_end(arguments);
                std::cout << std::endl;
        }*/

	explicit AggregatePtrsAndDelete(const size_t num)
		: AggregateAndApplyFuncBase<T>(num), _initEnabled(true)
	{
		//init needs to be called by subclass
	}

        virtual ~AggregatePtrsAndDelete() 
	{
		std::cout << "Destructor for AggregatePtrsAndDelete called" << std::endl;
	}

        bool applyDelete() {
		if (this->size() == 0) 
		{
			return false;
		}

		for (int index = 0; index < this->size(); index++)
		{
			T obj = this->getAt(index);
			std::cout << obj << std::endl;
			delete obj;
		}

		return true; 
	}

	virtual bool applyDelete(DfaManager&) = 0;
};

#endif
