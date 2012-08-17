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

#ifndef FILE_HANDLER_HPP_
#define FILE_HANDLER_HPP_

/*
 * Aug 15 2012
 * This File is being REORGANIZED, if unsuccessful it will be taken out
 */

namespace File_Handler
{
#include <functional>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include "file_handle.hpp"

	template<typename T>
	void sort(std::vector<T>& aVec)
	{
		std::sort(aVec.begin(), aVec.end());
	}

	void testSort()
	{
		std::vector<std::string> aVec { "a", "b", "a" };
		sort(aVec);
	}

	inline int rpss_fgetc(const File_Handle &handle)
	{
		return fgetc(handle.getFile());
	}

	inline int rpss_feof(const File_Handle &handle)
	{
		return feof(handle.getFile());
	}

	inline std::function<char (void)> rpss_fgetcFunc(const File_Handle &handle)
	{
		FILE *theFile = handle.getFile();
		return [theFile]() { return (char)fgetc(theFile); };
	}

	inline std::function<int (void)> rpss_feofFunc(const File_Handle &handle)
	{
		FILE *theFile = handle.getFile();
		return [theFile]() { return feof(theFile); };
	}
}

#endif /* FILE_HANDLER_HPP_ */
