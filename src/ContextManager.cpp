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

#include <iostream>
#include <string>

#include "deps/includes/lexer/ContextManager.hpp"

int TestContextMan()
{
	std::cout << ContextType::Lexer << ContextType::Parser << std::endl;

	ContextManager man;

	man.getContext<ContextType::AllowedTypes, ContextType::Lexer>().doMe();
	man.getContext<ContextType::AllowedTypes, ContextType::Parser>().doMe();
        man.getContext<ContextType::AllowedTypes, ContextType::Lexer>().doMe();

	ContextManager::TypedContext<ContextType::AllowedTypes, ContextType::Lexer> ctx;
	ctx.doMe();

	return 0;
}
