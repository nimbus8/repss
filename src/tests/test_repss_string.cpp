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

#include <iostream>
#include <string>
using namespace std;
#include "../deps/includes/repss_str.hpp"

int repss_str_test(int argc, char *argv[])
{
	if (argc < 3)
	{
		char firstArg[] = "== repss-str-test ==\n";
		char secondArg[] = "adda";
		char thirdArg[] = "abbba";
		char *argv2[] = { firstArg, secondArg, thirdArg };

		argv = argv2;
	}

	char a[] = "abc";
	char b[] = "abc";
	char str_percent[] = "%%";
	char buffer[10];

	cout << argv[0] << endl;

	cout << b << "==" << a << ": " << repss_str::strcmp_ce(b, a) << " len(a): "
			<< repss_str::strlen_ce(a) << endl;
	cout << buffer << "==" << str_percent << ": "
			<< repss_str::strcmp_ce(buffer, str_percent) << endl;
	buffer[0] = '%';
	cout << buffer << "==" << str_percent << ": "
			<< repss_str::strcmp_ce(buffer, str_percent) << endl;
	cout << argv[1] << "==" << argv[2] << ": "
			<< repss_str::strcmp_ce(argv[1], argv[2]) << ", len(a):"
			<< repss_str::strlen_ce(argv[1]) << " len(b):"
			<< repss_str::strlen_ce(argv[2]) << endl;
	buffer[1] = '%';
	cout << buffer << "==" << str_percent << ": "
			<< repss_str::strcmp_ce(buffer, str_percent) << endl;
	buffer[1] = '!';
	cout << buffer << "==" << str_percent << ": "
			<< repss_str::strcmp_ce(buffer, str_percent) << endl;
	buffer[1] = '%';
	cout << buffer << "==" << str_percent << ": "
			<< repss_str::strcmp_ce(buffer, str_percent) << endl;
	buffer[2] = '\0';
	cout << buffer << "==" << str_percent << ": "
			<< repss_str::strcmp_ce(buffer, str_percent) << endl;

	return 0;
}
