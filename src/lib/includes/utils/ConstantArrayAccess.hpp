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

#ifndef _CONST_ARRAY_ACCESS_
#define _CONST_ARRAY_ACCESS_

//unfinished class
template<class T>
class ConstantArrayAccess
{
private:
    T* elements;
};

//finished class
template<class T>
class ConstantPtrElement
{
private:
    const T* _element;
    const size_t _size;
public:
    explicit ConstantPtrElement(const T* const element)
        : _element(element), _size(1) {}
    ConstantPtrElement(const T* const element, const size_t size)
        : _element(element), _size(size) {}

    const T* getElement() const
    {
        return _element;
    }

    const size_t getSize() { return _size; }

    //I think this class should have the responsibility 
    //of deleting the allocated container, but only after setting the
    //elements to null
    ~ConstantPtrElement()
    {
        if (_size > 1)
        {
            for (size_t index = 0; index < _size; index++)
            {
                _element[index] = nullptr;
            }
        }
        else
        {
            _element = nullptr;
        }

        delete _element;
    }
};

//finished class
//(1)theres no stopping someone from using
//the other one in code, BUT that one doesn't guarantee
//that if the element is a pointer and it wasn't properly
//initialized as a constant then the const array access would
//necessarily allow you to change content of the whatever the pointer references.
//this is a more top level guard, in case we forget but remember to use this specific class.
//(2)Likewise dont think I can force anyone(myself) to use ptrs with this class either, I don't think it
//makes a differene - I could be mistaken.
template<class T>
class ConstantPtrArrayAccess
{
private:
    //we just did this, now fill in the blanks!
    ConstantPtrElement<T>* _elements;
    size_t _size;
public:
    ConstantPtrArrayAccess(T* elements, size_t size)
        : _elements(elements), _size(size) {}
    ConstantPtrArrayAccess(const ConstantPtrArrayAccess& other)
        : _elements(other._elements), _size(other._size()) {}
    ConstantPtrArrayAccess(ConstantPtrArrayAccess&& other)
        : _elements(nullptr), _size(0)
    {
        _elements = other._elements;
        _size = other._size;

        other._elements = nullptr;
        other._size = 0;
    }

    //put here especially for use.
    explicit ConstantPtrArrayAccess(const AggregateAndApplyFuncBase<T>* aggr)
    {
        const size_t aggrSize = aggr->size();

        //fill up array
        T* anArray = new T[aggrSize];
        for (size_t index = 0; index < aggrSize; index++)
        {
            anArray[index] = aggr->getAt(index);
        }

        _elements = new ConstantPtrElement<T>(anArray, aggrSize);
        
        _size = aggrSize;
    }

    const T getAt() const
    {
        return (_elements + index);
    }
};

//unfinished
//the delete operation will be called on ptr
//i theory if we ever called this we would call delete on elements CONTENT
class ManagedConstantPtrArrayAccess
{
};
#endif
