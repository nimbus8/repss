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

enum class GrammarType_t : char
{
    UNDEFINED  = 'U',
    TERMINAL   = 'T',
    VARIABLE   = 'V'
};

class AbstrKeywords
{
private:
    virtual IKeywords() = 0;
protected:
    class KeywordsData
    {
    private:
        const AbstrKeywordDefn keywords[5] =
          {
            AbstractKeywordDefn{ "reps.named_iteration", GrammarType_t::VARIABLE },
            AbstractKeywordDefn{ "scope", GrammarType_t::VARIABLE },
            AbstractKeywordDefn{ "alternation", GrammarType_t::TERMINAL },
            AbstractKeywordDefn{ "evaluation", GrammarType_t::TERMINAL },
            AbstractKeywordDefn{ "general_end", GrammarType_t::TERMINAL }
          };
    };
};