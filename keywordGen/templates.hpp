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

#ifndef _GEN_TEMPLATES_
#define _GEN_TEMPLATES_

#include <string>
#include <vector>

std::string createCopyright()
{
    std::string str;

    str.append("/*\n");
    str.append(" REPSS\n");
    str.append(" Copyright (C) 2012  Khalique Williams\n");
    str.append("\n");
    str.append(" This file is part of REPSS.\n");
    str.append("\n");
    str.append(" REPSS is free software: you can redistribute it and/or modify\n");
    str.append(" it under the terms of the   GNU General Public License as published by\n");
    str.append(" the Free Software Foundation, either version 3 of the License, or\n");
    str.append(" (at your option) any later version.\n");
    str.append("\n");
    str.append(" REPSS is distributed in the hope that it will be useful,\n");
    str.append(" but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
    str.append(" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
    str.append(" GNU General Public License for more details.\n");
    str.append("\n");
    str.append(" You should have received a copy of the GNU General Public License\n");
    str.append(" along with REPSS.  If not, see <http://www.gnu.org/licenses/>.\n");
    str.append(" */\n");

    return str;
}

class KeywordsTemplate
{
public:
    KeywordsTemplate() {}
    ~KeywordsTemplate() {}

    std::pair<std::string, std::string> generateClass(const std::string& collectionClassName, std::string& singularClassName)
    {
        auto toWriteStart = createCopyright();
        toWriteStart.append("\n//Should you modify this file? NO\n");

        toWriteStart.append("\n#include <string>\n");

        toWriteStart.append("\nenum class GrammarType_t : char\n");
        toWriteStart.append("{\n");
        toWriteStart.append("    UNDEFINED  = 'U',\n");
        toWriteStart.append("    TERMINAL   = 'T',\n");
        toWriteStart.append("    VARIABLE   = 'V'\n");
        toWriteStart.append("};\n");

        toWriteStart.append("\nclass I");
        toWriteStart.append(singularClassName);
        toWriteStart.append("\n{\n");
        toWriteStart.append("    virtual std::string getName() const = 0;\n");
        toWriteStart.append("};\n");

        toWriteStart.append("\nclass Abstr");
        toWriteStart.append(singularClassName);
        //--//
        toWriteStart.append(" : public I");
        toWriteStart.append(singularClassName);
        toWriteStart.append("\n{\n");
        //--
        toWriteStart.append("private:\n");
        toWriteStart.append("    const std::string	_name;\n");
        toWriteStart.append("    const GrammarType_t	_grammarType;\n");
        toWriteStart.append("public:\n");
        toWriteStart.append("    Abstr");
        toWriteStart.append(singularClassName);
        toWriteStart.append("(const std::string& name, const GrammarType_t grammarType)\n");
        toWriteStart.append("        : _name(name), _grammarType(grammarType) {}\n");
        toWriteStart.append("\n    virtual std::string getName() const { return _name; }\n");
        toWriteStart.append("    GrammarType_t getGrammarType() const { return _grammarType; }\n");
        toWriteStart.append("};\n");

        toWriteStart.append("\nclass ");
        toWriteStart.append(collectionClassName);
        toWriteStart.append("\n{\nprivate:\n    virtual ~");
        toWriteStart.append(collectionClassName);
        toWriteStart.append("() = 0;");

        std::string toWriteEnd("\n};\n");

        return std::pair<std::string,std::string>(toWriteStart, toWriteEnd);
    }

    //one
    std::string generateKeywordsData(const std::vector<std::pair<std::string, std::string>>& keywordDetails, std::string& singularName)
    {
        auto numberOfKeywords = keywordDetails.size();
        std::string output("\nprotected:\n    const class KeywordsData\n    {\n    private:\n");

        output.append("        const Abstr");
        output.append(singularName);
        output.append(" keywords[");
        output.append(std::to_string(numberOfKeywords));
        output.append("] =\n");
        output.append("          {\n");

        int count = 0;
        for (auto keywordPair : keywordDetails)
        {
            output.append("            Abstr");
            output.append(singularName);
            output.append("{ \"");
            output.append(keywordPair.first);
            output.append("\", GrammarType_t::");
            output.append(keywordPair.second);
            output.append(" }");

            if (count < numberOfKeywords - 1)
            {
                output.append(",");
            }

            output.append("\n");
            count ++;
        }

        output.append("          };\n");

        output.append("    public:\n");
        output.append("        Abstr");
        output.append(singularName);
        output.append(" getAt(const size_t index) const\n");
        output.append("        {\n");
        output.append("             return keywords[index];\n");
        output.append("        }\n");
        output.append("\n        size_t getSize() const { return ");
        output.append(std::to_string(numberOfKeywords));
        output.append("; }\n");

        output.append("    } _data;\n");

        return output;
    }
};

#endif

