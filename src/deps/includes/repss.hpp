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

#include "repss_str.hpp"
//#include "file_handler/file_handler.hpp"
#include "file_handler/FileHandler.hpp"
#include "memory_operations.hpp"

#ifndef REPSS_HPP_
#define REPSS_HPP_

#define REPSS_PERCENT "%"
#define REPSS_DOUBLE_PERCENT "%%"

#define DEF_REPSS_SYMB_1 REPSS_PERCENT
#define DEF_REPSS_SYMB_2 REPSS_DOUBLE_PERCENT

//S - symbol in input, N - name in code, T - positive result, F - false result
#define closureCase(S,N,T,F) repss_str::strcmp_ce((S),(N))==0?(T):(F)

#endif
