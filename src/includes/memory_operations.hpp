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

#ifndef MEMORY_OPERATIONS_HPP_
#define MEMORY_OPERATIONS_HPP_

namespace Arrays
{
#include <functional>
#include <stdio.h>

template<typename T>
inline std::function<void (void)> rpss_clearArrayFunc(T buffer[], unsigned int len)
{
	return [&buffer,len]()->void {memset(buffer, '\0', len);};
}

template<typename T>
inline void rpss_clearArray(T (&buffer)[], unsigned int len)
{
	rpss_clearArrayFunc(buffer, len)();
}

template<typename T>
inline std::function<void (T)> rpss_appendToArrayFunc(T buffer[], unsigned int &insertionIndex)
{
	return [&buffer, &insertionIndex](T value)->void {printf("index: %u\n", (int)insertionIndex); buffer[insertionIndex++] = value;};
}

template<typename T>
inline void rpss_appendToArray(T buffer[], unsigned int &insertionIndex, T value)
{
	rpss_appendToArrayFunc(buffer, insertionIndex)(value);
}


#endif /* MEMORY_OPERATIONS_HPP_ */
}
