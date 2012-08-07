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

#ifndef FILE_HANDLE_HPP_
#define FILE_HANDLE_HPP_

#include <stdio.h>
#include "file_io_error.hpp"


class File_Handle
{
private:
	FILE *_file;
public:
	File_Handle(const char *filename, const char *permissions)
	{
		_file = fopen(filename, permissions);
		//if (_file==0) exit(1);
		doSomething(filename);

		if (_file == 0) throw File_Not_Found_Error(filename);
	}

	File_Handle(const std::string& filename, const char *permissions)
	{
		_file = fopen(filename.c_str(), permissions);
		//if (_file==0) exit(1);

		doSomething(filename.c_str());

		if (_file == 0) throw File_Not_Found_Error(filename);
	}

	~File_Handle()
	{
		fclose(_file);
	}

	File_Handle(const File_Handle& src) :
			_file(src._file)
	{
	}

	FILE* getFile() const
	{
		return _file;
	}
};

#endif /* FILE_HANDLER_HPP_ */
