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
#ifndef _GENRTD_KEYWORDS_
#define _GENRTD_KEYWORDS_

#include <string>

enum class GrammarType_t : char
{
    UNDEFINED  = 'U',
    TERMINAL   = 'T',
    VARIABLE   = 'V'
};

class IKeyword
{
public:
    virtual ~IKeyword() {}
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
    AbstrKeyword(const AbstrKeyword& other)
        : _name(other._name), _grammarType(other._grammarType) {}
    AbstrKeyword(AbstrKeyword&& other)
        : _name(other._name), _grammarType(other._grammarType) {}
    ~AbstrKeyword() {}

    std::string getName() const { return _name; }
    GrammarType_t getGrammarType() const { return _grammarType; }
};

class Keywords
{
private:
    //virtual ~Keywords() {}
protected:
    class KeywordsData
    {
    private:
        AbstrKeyword keywords[6] =
          {
            AbstrKeyword{ "reps.named_iteration", GrammarType_t::VARIABLE },
            AbstrKeyword{ "scope", GrammarType_t::VARIABLE },
            AbstrKeyword{ "alternation", GrammarType_t::TERMINAL },
            AbstrKeyword{ "evaluation", GrammarType_t::TERMINAL },
            AbstrKeyword{ "general_end", GrammarType_t::TERMINAL },
            AbstrKeyword{ "reps.named_list_iteration", GrammarType_t::VARIABLE }
          };
    public:
       const AbstrKeyword getAt(const size_t index) const
        {
             return keywords[index];
        }

        size_t getSize() const { return 6; }
    } _data;

public:
    std::string getGrammarTypeForName(const std::string& keywordName) const
    {
        if (keywordName.compare("reps.named_list_iteration") == 0)
        {
            return "VARIABLE";
        }
        else if (keywordName.compare("reps.named_iteration") == 0)
        {
            return "VARIABLE";
        }
        else if (keywordName.compare("general_end") == 0)
        {
            return "TERMINAL";
        }
        else if (keywordName.compare("scope") == 0)
        {
            return "VARIABLE";
        }
        else if (keywordName.compare("evaluation") == 0)
        {
            return "TERMINAL";
        }
        else if (keywordName.compare("alternation") == 0)
        {
            return "TERMINAL";
        }

        return "INVALID";
    }

    Keywords()
    {
    }

};
#endif
