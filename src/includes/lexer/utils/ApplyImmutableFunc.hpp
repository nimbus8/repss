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

#ifndef _APPLY_IMMUTABLE_FUNC_
#define _APPLY_IMMUTABLE_FUNC_

template<class T>
class ApplyImmutableFunc : public AggregateAndApplyFuncBase<T>
{
private:
        const std::function<bool (AggregateAndApplyFuncBase<T>&)>& _func;

        bool _noFunc;

        void _handleVargs(size_t num, va_list arglist)
        {
                for (size_t n = 0; n < num; n++)
                {
                        T argval = va_arg(arglist,T);
                        this->addAt(n, argval);
                }
        }
public:

        ApplyImmutableFunc() : AggregateAndApplyFuncBase<T>(), _noFunc(true){}
        ApplyImmutableFunc(std::function<bool (AggregateAndApplyFuncBase<T>&) > &func, const size_t num, ...) : AggregateAndApplyFuncBase<T>(num), _func(func), _noFunc(false)
        {
                va_list arguments;
                va_start(arguments, num);
                
                _handleVargs(num,arguments);

                va_end(arguments);
                std::cout << std::endl;
        }
        ApplyImmutableFunc(const size_t num, ...) : AggregateAndApplyFuncBase<T>(num), _func(nullptr), _noFunc(true)
        {
                va_list arguments;
                va_start(arguments, num);

                _handleVargs(num,arguments);

                va_end(arguments);

                _noFunc = true;

                std::cout << std::endl;
        }

        ~ApplyImmutableFunc() {}

        bool apply() { if (this->size() == 0) {return false;} return _func(*this); }
        bool apply(std::function<bool (AggregateAndApplyFuncBase<T>&)> &func) { if (this->size() == 0) {return false;} if (!_noFunc) {std::cout << "A func hs already been associated with this Apply Obj ... Exiting" << std::endl; exit(1);} return func(*this); }
};

#endif
