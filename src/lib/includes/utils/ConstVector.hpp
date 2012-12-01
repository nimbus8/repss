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

#ifndef _CONST_VECTOR_
#define _CONST_VECTOR_

#include <vector>
#include <string>

template<class T>
class ConstVector
{
private:
    const std::vector<T>* _vector;
public:
    explicit ConstVector(const std::vector<T> *vector) : _vector(vector) {}
    ConstVector(const ConstVector& other) : _vector(other._vector) {}
    ConstVector(ConstVector&& other) : _vector(nullptr)
    {
        _vector = other._vector;
        other._vector = nullptr;
    }

    const T getAt(size_t index) const
    {
        return _vector->at(index);
    }

    size_t size()
    {
        return _vector->size();
    }
};

#endif
