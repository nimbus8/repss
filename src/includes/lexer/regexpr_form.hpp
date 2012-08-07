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

#ifndef REGEXPR_FORM_HPP_
#define REGEXPR_FORM_HPP_

#include <string.h>

class repss_regexpr_form
{
private:
	std::string _regexpr_repr;
public:
	explicit repss_regexpr_form(std::string regexpr_repr) :
			_regexpr_repr(regexpr_repr)
	{
	}
	explicit repss_regexpr_form(const char *cstr) :
			_regexpr_repr(cstr)
	{
	}
	explicit repss_regexpr_form(char str[]) :
			_regexpr_repr(str)
	{
	}

	repss_regexpr_form(const repss_regexpr_form &src) :
			_regexpr_repr(src._regexpr_repr)
	{
	}

	~repss_regexpr_form()
	{
	}
};

#endif /* REGEXPR_FORM_HPP_ */
