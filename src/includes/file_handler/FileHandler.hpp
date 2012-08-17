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

#ifndef FILE_HANDLER_
#define FILE_HANDLER_

/*
 * Aug 15 2012
 * This File is being REORGANIZED, if unsuccessful it will be taken out
 * Remember: We are introducing naming system for files to exclude _ (underscore)
 */

#include <functional>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include "FileHandle.hpp"

namespace REPSS_FileHandler
{
	inline const int isEndOfFile(const FileHandle& handle)
	{
		FILE* const theFile = handle.getFile();
		const int endOfFile = feof(theFile);

		return endOfFile;
	}

	inline const int getCharacter(const FileHandle& handle)
	{
		FILE* const theFile = handle.getFile();
		const int character = fgetc(theFile);

		return character;
	}
}

#endif /* FILE_HANDLER_HPP_ */
