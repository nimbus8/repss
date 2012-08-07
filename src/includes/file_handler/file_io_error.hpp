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

#ifndef FILE_IO_ERROR_HPP_
#define FILE_IO_ERROR_HPP_

#include <stdexcept>
#include <stdio.h>
#include <string>

class File_IO_Error: public std::runtime_error
{
public:
	File_IO_Error(const std::string& message) :
			std::runtime_error(message)
	{
	}
};

class File_Not_Found_Error: public File_IO_Error
{
private:
	std::string _errorMessage = "File Not Found Error: ";
	std::string _filename;
public:
	explicit File_Not_Found_Error(const std::string& filename) :
			File_IO_Error(_errorMessage), _filename(filename)
	{
	}

	explicit File_Not_Found_Error(const char *filename) :
					File_IO_Error(_errorMessage), _filename(filename)
	{
	}

	virtual const char* what() const throw ()
	{
		return (_errorMessage + _filename).c_str();
	}
};

void doSomething(const char *filename)
{
	printf("\n-- Do Something -- %s\n", filename);
}

#endif /* FILE_IO_ERROR_HPP_ */
