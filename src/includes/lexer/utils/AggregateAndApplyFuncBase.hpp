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

#include <stdarg.h>
#include <utility>
#include <functional>
#include <unordered_map>

#ifndef _AGGREGATE_AND_APPLY_FUNC_BASE_
#define _AGGREGATE_AND_APPLY_FUNC_BASE_

//AggregateAndApplyFuncBase - meant to be used as abstract class
template<class T>
class AggregateAndApplyFuncBase
{
private:
        T *elements;
        const size_t _size;
protected:
        void addAt(size_t index, T element) { elements[index] = element;}
public:
        AggregateAndApplyFuncBase() : _size(0) {elements = nullptr;}
        AggregateAndApplyFuncBase(size_t num, ...) : _size(num)
        {
                if (num == 0)
                {
                        elements = nullptr;
                }
                else
                {
                        elements = new T[num];
                }
        };
        ~AggregateAndApplyFuncBase() { for (size_t i = 0; i < size(); i++) { std::cout << "setting member (" << i << ") to null" << std::endl;} std::cout << "deleting array with delete[]" << std::endl;  delete elements; }

        T getAt(size_t index) const { if (elements == nullptr) {std::cout << "error - ::getAt in AggregateAndApplyFuncBase - Invalid use"; exit(1);} return elements[index]; }
        size_t size() const { return _size; };
};

#endif
