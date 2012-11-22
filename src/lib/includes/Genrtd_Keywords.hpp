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

//Should you modify this file? NO

#include <string>

enum class GrammarType_t : char
{
    UNDEFINED  = 'U',
    TERMINAL   = 'T',
    VARIABLE   = 'V'
};

class IKeyword
{
    virtual std::string getName() const = 0;
};

class AbstrKeyword : public IKeyword
{
private:
    const std::string	_name;
    const GrammarType_t	_grammarType;
public:
    AbstrKeyword(const std::string& name, const GrammarType_t grammarType)
        : _name(name), _grammarType(grammarType) {}

    virtual std::string getName() const { return _name; }
    GrammarType_t getGrammarType() const { return _grammarType; }
};

class Keywords
{
private:
    virtual ~Keywords() = 0;
protected:
    const class KeywordsData
    {
    private:
        const AbstrKeyword keywords[6] =
          {
            AbstrKeyword{ "reps.named_iteration", GrammarType_t::VARIABLE },
            AbstrKeyword{ "scope", GrammarType_t::VARIABLE },
            AbstrKeyword{ "alternation", GrammarType_t::TERMINAL },
            AbstrKeyword{ "evaluation", GrammarType_t::TERMINAL },
            AbstrKeyword{ "general_end", GrammarType_t::TERMINAL },
            AbstrKeyword{ "reps.named_list_iteration", GrammarType_t::VARIABLE }
          };
    public:
        AbstrKeyword getAt(const size_t index) const
        {
             return keywords[index];
        }

        size_t getSize() const { return 6; }
    } _data;

};
