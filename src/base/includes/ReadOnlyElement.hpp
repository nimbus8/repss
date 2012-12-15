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

#ifndef _READ_ONLY_ELEMENT_
#define _READ_ONLY_ELEMENT_

//used to define complex object structures - as a nonspecific base level representation
// and maybe even a transfer object. Type will usually be some stl container with possibly
// more stl containers as template parameters.

template<class T>
class ReadOnlyElement
{
private:
    const T* data;
public:
    explicit ReadOnlyElement(T* data) {}
    ReadOnlyElement(const ReadOnlyElement other) : _data(other._data) {}
    ReadOnlyElement(ReadOnlyElement&& other) : _data(nullptr)
    {
        _data = _other._data;
        _other.data = nullptr;
    }
    ~ReadOnlyElement() {}
};

#endif
