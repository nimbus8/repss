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

#include "AggregatePtrsAndDelete.hpp"

AggregatePtrsAndDelete<char*>* test()
{
        char *chars = new char[10];
	chars[0] = 'H';
	chars[1] = 'E';
	chars[2] = 'L';
	chars[3] = 'L';
	chars[4] = 'O';
	chars[5] = '\0';

        char *chars1 = new char[10];
        chars1[0] = 'P';
        chars1[1] = 'E';
        chars1[2] = 'A';
        chars1[3] = 'R';
        chars1[4] = '\0';	

	AggregatePtrsAndDelete<char*>* agg = 
		new AggregatePtrsAndDelete<char*>(2, chars, chars1);

	return agg;
}

int main()
{
	auto aggRet = test();	
	
	aggRet->applyDelete();
}
