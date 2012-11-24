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
#include <unordered_map>

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

//Note: Order is VERY important when using any of the templates. The order they are defined below in the TempalteSystem
//      should ALWAYS work.
class TemplateSystem
{
private:
    std::string _generatedKeywordsFilePath;
    std::string _generatedAbstractGrammaConfigFilePath;
    std::string _generatedAbstractLexerWordConstructorFilePath;

    std::string _keywordInterfaceName;
    std::string _keywordClassName;
    std::string _keywordsCollectionClassName;
    std::string _abstractGrammarConfigClassName;
public:
    TemplateSystem(std::string generatedAbstractGrammaConfigFilePath, std::string generatedAbstractLexerWordConstructorFilePath)
            : _generatedAbstractGrammaConfigFilePath(generatedAbstractGrammaConfigFilePath), _generatedAbstractLexerWordConstructorFilePath(generatedAbstractLexerWordConstructorFilePath) {}

    //this is for write component of keywords.cpp. Actual path is runtime dependant, defined in some input file.
    void setGeneratedKeywordsFilePath(const std::string& generatedKeywordsFilePath)
    {
        if (!_generatedKeywordsFilePath.empty())
        {
            perror("Error: you MAY NOT set the path for Keywords file TWICE. Only ONCE. Exitting...");
            exit(1);
        }

        _generatedKeywordsFilePath = generatedKeywordsFilePath;
    }

    class KeywordsTemplate;
    class AbstrLexerWordConstructorTemplate;
    class AbstrGrammarConfigTemplate; 
 
    friend class KeywordsTemplate;
    friend class AbstrLexerWordConstructorTemplate;
    friend class AbstrGrammarConfigTemplate;

    class KeywordsTemplate
    {
    private:
        TemplateSystem *_system;
    public:
        KeywordsTemplate(TemplateSystem* aSystem) : _system(aSystem) {}
        ~KeywordsTemplate() {}

        std::pair<std::string, std::string> generateClass(const std::string& collectionClassName, std::string& singularClassName) const
        {
            auto toWriteStart = createCopyright();
            toWriteStart.append("\n//Should you modify this file? NO\n");

            toWriteStart.append("#ifndef _GENRTD_KEYWORDS_\n#define _GENRTD_KEYWORDS_\n");

            toWriteStart.append("\n#include <string>\n");

            //grammar type enum
            toWriteStart.append("\nenum class GrammarType_t : char\n");
            toWriteStart.append("{\n");
            toWriteStart.append("    UNDEFINED  = 'U',\n");
            toWriteStart.append("    TERMINAL   = 'T',\n");
            toWriteStart.append("    VARIABLE   = 'V'\n");
            toWriteStart.append("};\n");

            //interface
            _system->_keywordInterfaceName = std::string{"I"}.append(singularClassName);

            toWriteStart.append("\nclass ");
            toWriteStart.append(_system->_keywordInterfaceName);
            toWriteStart.append("\n{\npublic:\n");
            toWriteStart.append("    virtual ~");
            toWriteStart.append(_system->_keywordInterfaceName);
            toWriteStart.append("() {}\n");
            toWriteStart.append("    virtual std::string getName() const = 0;\n");
            toWriteStart.append("};\n");

            //Concrete class: Keyword
            _system->_keywordClassName = std::string{"Abstr"}.append(singularClassName);

            toWriteStart.append("\nclass ");
            toWriteStart.append(_system->_keywordClassName);
            //--//
            toWriteStart.append(" : public ");
            toWriteStart.append(_system->_keywordInterfaceName);
            toWriteStart.append("\n{\n");
            //--
            toWriteStart.append("private:\n");
            toWriteStart.append("    const std::string	_name;\n");
            toWriteStart.append("    const GrammarType_t	_grammarType;\n");
            toWriteStart.append("public:\n");

            //-- constructor
            toWriteStart.append("    ");
            toWriteStart.append(_system->_keywordClassName);
            toWriteStart.append("(const std::string& name, const GrammarType_t grammarType)\n");
            toWriteStart.append("        : _name(name), _grammarType(grammarType) {}\n");

            //-- copy/move constructor
            toWriteStart.append("    ");
            toWriteStart.append(_system->_keywordClassName);
            toWriteStart.append("(const ");
            toWriteStart.append(_system->_keywordClassName);
            toWriteStart.append("& other)\n        : _name(other._name), _grammarType(other._grammarType) {}\n");

            toWriteStart.append("    ");
            toWriteStart.append(_system->_keywordClassName);
            toWriteStart.append("(");
            toWriteStart.append(_system->_keywordClassName);
            toWriteStart.append("&& other)\n        : _name(other._name), _grammarType(other._grammarType) {}\n");

            //-- destructor
            toWriteStart.append("    ~");
            toWriteStart.append(_system->_keywordClassName);
            toWriteStart.append("() {}\n");

            //-- functions
            toWriteStart.append("\n    std::string getName() const { return _name; }\n");
            toWriteStart.append("    GrammarType_t getGrammarType() const { return _grammarType; }\n");
            toWriteStart.append("};\n");

            _system->_keywordsCollectionClassName = collectionClassName;

            //abstract collection class (start)
            toWriteStart.append("\nclass ");
            toWriteStart.append(_system->_keywordsCollectionClassName);

            //-- private destructor
            toWriteStart.append("\n{\nprivate:\n    //virtual ~");
            toWriteStart.append(_system->_keywordsCollectionClassName);
            toWriteStart.append("() {}");


            //the ending, second part of return pair
            std::string toWriteEnd("\n    ");
            toWriteEnd.append(_system->_keywordsCollectionClassName);
            toWriteEnd.append("()\n");
            toWriteEnd.append("    {\n");
            toWriteEnd.append("    }\n");
            toWriteEnd.append("\n};\n#endif\n");

            return std::pair<std::string,std::string>(toWriteStart, toWriteEnd);
        }

        std::string generateKeywordsData(const std::vector<std::tuple<std::string, std::string, std::string>>& keywordDetails) const
        {
            std::unordered_map<std::string, std::string> keywordNameToGrammarType;

            auto numberOfKeywords = keywordDetails.size();
            std::string output("\nprotected:\n    class KeywordsData\n    {\n    private:\n");

            output.append("        ");
            output.append(_system->_keywordClassName);
            output.append(" keywords[");
            output.append(std::to_string(numberOfKeywords));
            output.append("] =\n");
            output.append("          {\n");

            int count = 0;
            for (auto keywordPair : keywordDetails)
            {
                const std::string keywordName(std::get<0>(keywordPair));
                const std::string grammarType(std::get<1>(keywordPair));
                output.append("            ");
                output.append(_system->_keywordClassName);
                output.append("{ \"");
                output.append(keywordName);
                output.append("\", GrammarType_t::");
                output.append(grammarType);
                output.append(" }");

                //add keywordName => GrammarType mapping
                keywordNameToGrammarType.emplace(std::pair<std::string,std::string>(keywordName, grammarType));

                if (count < numberOfKeywords - 1)
                {
                    output.append(",");
                }

                output.append("\n");
                count ++;
            }

            output.append("          };\n");

            output.append("    public:\n");
            output.append("       const ");
            output.append(_system->_keywordClassName);
            output.append(" getAt(const size_t index) const\n");
            output.append("        {\n");
            output.append("             return keywords[index];\n");
            output.append("        }\n");
            output.append("\n        size_t getSize() const { return ");
            output.append(std::to_string(numberOfKeywords));
            output.append("; }\n");

            output.append("    } _data;\n");

            output.append("\npublic:\n    std::string getGrammarTypeForName(const std::string& keywordName) const\n");
            output.append("    {\n");

            count = 0;
            for (auto keywordNameAndType : keywordNameToGrammarType)
            {
                if (count == 0)
                {
                    output.append("        if (keywordName.compare(\"");
                    output.append(keywordNameAndType.first);
                    output.append("\") == 0)\n");
                    output.append("        {\n");
                    output.append("            return \"");
                    output.append(keywordNameAndType.second);
                    output.append("\";\n");
                    output.append("        }\n");
                }
                else
                {
                    output.append("        else if (keywordName.compare(\"");
                    output.append(keywordNameAndType.first);
                    output.append("\") == 0)\n");
                    output.append("        {\n");
                    output.append("            return \"");
                    output.append(keywordNameAndType.second);
                    output.append("\";\n");
                    output.append("        }\n");
                }

                count++;
            }

            output.append("\n        return \"INVALID\";\n");
            output.append("    }\n");

            return output;
        }
    };

    class AbstrLexerWordConstructorTemplate
    {
    private:
        TemplateSystem* _system;
    public:
        AbstrLexerWordConstructorTemplate(TemplateSystem* aSystem) : _system(aSystem) {}
        ~AbstrLexerWordConstructorTemplate() {}

        //construction AbstractClass - to be updated and enforced (AbstrLexerWordConstructor)
        std::string generateStartOfClass() const
        {
            //this is very specific
            //some keyword names are essentially compositions reps.named_iteration, which
            //lets us play with language constructs like building blocks. By additionally
            //requiring an equivalent 'AlphaOnly' name, we setup a naming convention for
            //function names that refer to routines that build and return the representation
            //for any particular language construct.

            std::string output = createCopyright();
            output.append("\n//Should you modify this file? NO\n");

            output.append("\n#include \"construction/ConstructionTypedefs.hpp\"\n");

            output.append("\n#ifndef _ABSTR_LEXER_WORD_CONSTRUCTOR_\n#define _ABSTR_LEXER_WORD_CONSTRUCTOR_\n");

            output.append("\nclass AbstrLexerWordConstructor\n");
            output.append("{\n");
            output.append("protected:\n");
            output.append("    std::vector<word_start_and_aggregated_nodes_Pair_t> _words;\n");
            output.append("    std::vector<aggregated_transitions_ptr_t> _dfaTransitions;\n");

            output.append("\n    virtual ~AbstrLexerWordConstructor()\n    {\n");
            output.append("        //...\n");
            output.append("    }\n");

            output.append("public:\n");

            return output;
        }

        std::string generateInitAndWordFunctions(const std::vector<std::string>& keywordNamesAlphaOnly) const
        {
            std::string output;

            output.append("    void _initWords()\n");
            output.append("    {\n");
            output.append("        //the first in container is a pair with lexer_dfa accessible\n");
            output.append("        //the second in container is a reminder for us to delete DfaTransitions when we're done\n");

            for (auto keywordName : keywordNamesAlphaOnly)
            {
                output.append("\n        auto a");
                output.append(keywordName);
                output.append("Word = _construct");
                output.append(keywordName);
                output.append("();\n");

                output.append("        _words.push_back(a");
                output.append(keywordName);
                output.append("Word.first);\n");

                output.append("        _dfaTransitions.push_back(a");
                output.append(keywordName);
                output.append("Word.second);\n");
            }

            output.append("    }\n\n");

            for (auto keywordName : keywordNamesAlphaOnly)
            {
                output.append("    virtual wordrepr_and_transition_Pair_t _construct");
                output.append(keywordName);
                output.append("() = 0;\n");
            }

            //here we have a mapping function: keywordName => index, 

            return output;
        }

        std::string generateEmbeddedWordFunctions(const std::vector<std::string>& embeddedFunctionNames) const
        {
            std::string output("\n");

            for (auto embeddedFunctionName : embeddedFunctionNames)
            {
                output.append("    virtual wordrepr_and_transition_Pair_t __insert");
                output.append(embeddedFunctionName);
                output.append("ParamsDfa(lexer_dfa_ptr_t fromDfa, lexer_dfa_ptr_t toDfa, unsigned int tentativeNameKey) = 0;\n");
            }

            return output;
        }

        std::string generateEndOfClass() const
        {
            std::string output;

            output.append("};\n\n#endif\n");

            return output;
        }
    };

    class AbstrGrammarConfigTemplate
    {
    private:
        TemplateSystem* _system;
    public:
        AbstrGrammarConfigTemplate(TemplateSystem* aSystem) : _system(aSystem) {}
        ~AbstrGrammarConfigTemplate() {}

        class GrammarKeywordInfo
        {
        private:
            std::string _name;
            std::string _type;
            std::string _allAlphaName;
        public:
            GrammarKeywordInfo(std::string& name, std::string& type, std::string& allAlphaName)
                : _name(name), _type(type), _allAlphaName(allAlphaName) {}
            GrammarKeywordInfo(const GrammarKeywordInfo& other)
                : _name(other._name), _type(other._type), _allAlphaName(other._allAlphaName) {}
            GrammarKeywordInfo(GrammarKeywordInfo&& other)
                : _name(""), _type(""), _allAlphaName("")
            {
                _name = other._name;
                _type = other._type;
                _allAlphaName = other._allAlphaName;

                other._name = "";
                other._type = "";
                other._allAlphaName = "";
            }

            std::string getName() const { return _name; }
            std::string getGrammarType() const { return _type; }
            std::string getAllAlphaName() const { return _allAlphaName; }
        };

        std::string generateClass(const std::vector<GrammarKeywordInfo> grammarKeywords)
        {
             std::string output = createCopyright();
             output.append("\n//Should you modify this file? NO\n");

             output.append("\n#ifndef _ABSTR_GRAMMAR_CONFIG_\n#define _ABSTR_GRAMMAR_CONFIG\n");
             output.append("\n#include <string>\n#include <vector>\n");

             output.append("\n#include \"../../../");
             output.append(_system->_generatedKeywordsFilePath);
             output.append("\"\n");

             output.append("#include \"ILanguageAndGrammar.hpp\"\n");

             output.append("#include \"utils/ReadOnlyData.hpp\"\n");


             _system->_abstractGrammarConfigClassName = "AbstrGrammarConfig";

             output.append("\nclass ");
             output.append(_system->_abstractGrammarConfigClassName);
             output.append("\n{\n");
             output.append("private:\n    ");
             output.append(_system->_keywordsCollectionClassName);
             output.append(" _keywords;\n");
             output.append("protected:\n    ReadOnlyData<std::vector<std::tuple<std::string, std::string, std::vector<GrammarRules::Term*>>>>* _grammarRules;\n");

             output.append("\n    virtual ~");
             output.append(_system->_abstractGrammarConfigClassName);
             output.append("()\n    {\n        delete _grammarRules;\n    }\n");

             output.append("public:\n");

             output.append("    ");
             output.append(_system->_abstractGrammarConfigClassName);
             output.append("()\n    {\n    }\n");

             //init function

             output.append("\n    void _init()\n");
             output.append("    {\n");

             output.append("        std::vector<std::tuple<std::string, std::string, std::vector<GrammarRules::Term*>>> _grammarRulesData;\n");

             for (auto grammarKeywordInfoObj : grammarKeywords)
             {
                 output.append("\n        auto grammarRules__");
                 output.append(grammarKeywordInfoObj.getAllAlphaName());
                 output.append(" = _defineGrammarKeyword_");
                 output.append(grammarKeywordInfoObj.getAllAlphaName());
                 output.append("(std::make_pair(\"");
                 output.append(grammarKeywordInfoObj.getName());
                 output.append("\", _keywords.getGrammarTypeForName(\"");
                 output.append(grammarKeywordInfoObj.getName());
                 output.append("\")));\n");

                 output.append("        _grammarRulesData.push_back(std::make_tuple(\"");
                 output.append(grammarKeywordInfoObj.getName());
                 output.append("\", \"");
                 output.append(grammarKeywordInfoObj.getGrammarType());
                 output.append("\", grammarRules__");
                 output.append(grammarKeywordInfoObj.getAllAlphaName());
                 output.append("));\n");
             }

             output.append("\n        _grammarRules = new ReadOnlyData<std::vector<std::tuple<std::string, std::string, std::vector<GrammarRules::Term*>>>>(static_cast<std::vector<std::tuple<std::string, std::string, std::vector<GrammarRules::Term*>>>&&>(_grammarRulesData));\n");

             output.append("    }\n\n");

             //virtual functions

             for (auto grammarKeywordInfoObj : grammarKeywords)
             {
                 output.append("    virtual std::vector<GrammarRules::Term*> _defineGrammarKeyword_");
                 output.append(grammarKeywordInfoObj.getAllAlphaName());
                 output.append("(std::pair<std::string, std::string> nameAndGrammarType__");
                 output.append(grammarKeywordInfoObj.getAllAlphaName());
                 output.append("__");
                 output.append(grammarKeywordInfoObj.getGrammarType());
                 output.append(") = 0;\n");
             }

             output.append("};\n\n#endif\n");

             return output;
        }
    };
};

#endif

