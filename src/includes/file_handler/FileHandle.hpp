/*
 REPSS
 Copyright (C) 2012  Khalique Williams

 This file is part of REPSS.

 REPSS is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 REPSS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with REPSS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FILE_HANDLE_
#define FILE_HANDLE_

#include <stdio.h>
#include "FileIOError.hpp"

namespace REPSS_FileHandler
{

class FileHandle
{
private:
	FILE *_file;
public:
	FileHandle(const char *filename, const char *permissions)
	{
		_file = fopen(filename, permissions);
		if (_file == 0)
		{
			throw FileNotFoundError(filename);
		}
	}

	FileHandle(const std::string& filename, const std::string& permissions)
	{
		_file = fopen(filename.c_str(), permissions.c_str());
		if (_file == 0)
		{
			throw FileNotFoundError(filename);
		}
	}

	~FileHandle()
	{
		fclose(_file);
	}

	FileHandle(const FileHandle& src) :
			_file(src._file)
	{
	}

	FILE* getFile() const
	{
		return _file;
	}
};
}

#endif /* FILE_HANDLER_HPP_ */
