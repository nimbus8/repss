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

#ifndef _REPSS_STR_
#define _REPSS_STR_

/**
 * namespace repss_str - Warning: Because of compiler restrictions, strings of a certain
 *  length may void evaluation of constexpr functions at compile time
*/
namespace repss_str
{
#define EOS '\0'

constexpr int charEquals(char a, char b)
{
        return a==b?(a==EOS?0:1):-1;
}

constexpr int charEqualsEOS(char a)
{
        return a==EOS?0:-1;
}

constexpr int strlen_ceImpl(char str[], int count)
{
        return (charEqualsEOS(str[count])==-1?strlen_ceImpl(str,count+1):count);
}

constexpr int strlen_ce(char str[])
{
        return strlen_ceImpl(str,0);
}

constexpr int strcmp_ceImpl(char a[], char b[], int startIndex)
{
        return (charEquals(a[startIndex],b[startIndex])>=0?(charEqualsEOS(a[startIndex])==-1?strcmp_ceImpl(a,b,startIndex+1):0):-1);
}

constexpr int strcmp_ce(char a[], char b[])
{
        return (strlen_ce(a)!=strlen_ce(b)?-1:strcmp_ceImpl(a,b,0));
}

}

#endif
