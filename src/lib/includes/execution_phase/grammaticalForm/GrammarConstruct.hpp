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

#include <string>
#include <vector>

#include "GrammarRules.hpp"

class GrammarConstruct
{
private:
    std::string _name;
    std::string _grammarType;
    std::vector<GrammarRules::Term*> _grammarRules;
public:
    GrammarConstruct(const std::string& name, const std::string& grammarType, const std::vector<GrammarRules::Term*> grammarRules) : _name(name), _grammarType(grammarType), _grammarRules(grammarRules) {}
    GrammarConstruct(const std::string& name, const std::string& grammarType, std::vector<GrammarRules::Term*>& grammarRules) : _name(name), _grammarType(grammarType), _grammarRules(static_cast<std::vector<GrammarRules::Term*>&&>(grammarRules)) {}
    GrammarConstruct(const GrammarConstruct& other)
        : _name(other._name), _grammarType(other._grammarType) {}
    GrammarConstruct(GrammarConstruct&& other)
        : _name(""), _grammarType(""), _grammarRules(0)
    {
        _name = other._name;
        _grammarType = other._grammarType;
        _grammarRules = other._grammarRules;

        other._name = "";
        other._grammarType = "";
        other._grammarRules.clear();
    }
};
