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

#define ABSTR_LEXER_WORD_CONSTRUCTOR_FROM_INCLUDES_PATH "execution_phase/lexer/Genrtd_AbstrLexerWordConstructor.hpp"
#define ABSTR_LEXER_WORD_CONSTRUCTOR_PATH "src/base/includes/"ABSTR_LEXER_WORD_CONSTRUCTOR_FROM_INCLUDES_PATH
#define ABSTR_GRAMMAR_CONFIG_FROM_INCLUDES_PATH "Genrtd_AbstrGrammarConfig.hpp"
#define ABSTR_GRAMMAR_CONFIG_PATH "src/base/includes/"ABSTR_GRAMMAR_CONFIG_FROM_INCLUDES_PATH

#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <vector>

#include "templates.hpp"

#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:  %s\n", __FILE__, __LINE__, str);
    #define DeNEWLINE() printf("\n");
#else
    #define DeLOG(str)
    #define DeNEWLINE()
#endif

enum class ModeInfo_t : unsigned int
{
    EMPTY                         = 0x002,
    ERROR                         = 0x004,

    READ_DONE                     = 0x008,
    WRITE_DONE                    = 0x020,

    SAVED_READ_DATA_TO_TEMP_FILE  = 0x040

    // exlusive hexadecimal counting: 0x0008, 0x0020, 0x0040, 0x0080, 0x0200, etc [1]
};

class Status
{
private:
    unsigned int _modeInfo;
public:
    Status() : _modeInfo(static_cast<unsigned int>(ModeInfo_t::EMPTY)) {}
    explicit Status(const ModeInfo_t& singleModeInfo) 
        : _modeInfo(static_cast<unsigned int>(singleModeInfo)) {}
    explicit Status(const unsigned int& compoundModeInfo) : _modeInfo(compoundModeInfo) {}

    Status(const Status& other) : _modeInfo(other._modeInfo) {}
    ~Status() {}

    void addModeInfo(const ModeInfo_t info); 

    bool isEmpty() const { return (_modeInfo & static_cast<unsigned int>(ModeInfo_t::EMPTY)); }
    bool isError() const { return (_modeInfo & static_cast<unsigned int>(ModeInfo_t::ERROR)); }
};

void Status::addModeInfo(const ModeInfo_t infoToAdd)
{
    unsigned int modeInfoToAddUINT = static_cast<unsigned int>(infoToAdd);
    if (!(_modeInfo & modeInfoToAddUINT))
    {
        _modeInfo ^= modeInfoToAddUINT;

        if (!(_modeInfo & modeInfoToAddUINT))
        {
            std::cout << "add mode info failed!!! oops!" << std::endl;
            exit(1);
        }
    }
}

bool confirmFirstNonWhiteSpaceInBuffer(char *buffer, size_t len, char character)
{
    for (size_t i = 0;  i < len; i++)
    {
        char val = *(buffer + i);
        if (val != ' ' &&  val != character)
        {
            return false;
        }
        else if (val == character)
        {
            return true;
        }
    }

    return false;
}

enum class FilterType_t { MATCH, ANYTHING_BUT };

//note to self, maybe templatize tis stuff. Seems useful, maybe there std functions for it?
//def typedef some of this

std::vector<std::tuple<std::string, std::string, std::string>> filterTuples(const std::vector<std::tuple<std::string, std::string, std::string>>& tuples, const FilterType_t filterType, const std::vector<std::string>& keys)
{
    std::vector<std::tuple<std::string, std::string, std::string>> retTuples;

    for (auto aTuple : tuples)
    {
        if (filterType == FilterType_t::MATCH)
        {
           bool isMatch = false;
           
           for (auto matchKey : keys)
           {
               if (std::get<0>(aTuple).compare(matchKey) == 0)
               {
                   isMatch = true;
               }
           }

           if (isMatch)
           {
               retTuples.push_back(aTuple);
           }
        }
        else if (filterType == FilterType_t::ANYTHING_BUT)
        {
           bool keepThisTuple = true;
             
           for (auto ignoreKey : keys)
           {
               if (std::get<0>(aTuple).compare(ignoreKey) == 0)
               {
                   keepThisTuple = false;
               }
           }

           if (keepThisTuple) 
           {
               retTuples.push_back(aTuple);
           }
        }
    }

    return retTuples;
}

std::vector<std::tuple<std::string, std::string, std::string>> filterTuples(const std::vector<std::tuple<std::string, std::string, std::string>>& tuples, const FilterType_t& filterType, const std::string& key)
{
   return filterTuples(tuples, filterType, std::vector<std::string>{key});
}

void clearArray(char *buffer, size_t size)
{
    memset(buffer, 0, size);
}

bool checkIfTheTwoCharactersFollow(const std::string& str, char first, char second, std::vector<char> cannotAcceptInBetween)
{
    bool inExpectingMode = false;
    for (const char& c : str)
    {
        if (!inExpectingMode)
        {
            if (c == first)
            {
                inExpectingMode = true;
            }
        }
        else
        {
            for (const char& c2 : cannotAcceptInBetween)
            {
                if (c == c2) return false;
            }

            if (c == second) return true;
        }
    }

    return false;
}


typedef std::vector<std::tuple<std::string,std::string,std::vector<std::tuple<std::string,std::string,std::string>>>> ObjectDataVector_t;
const size_t ID_INDEX_IN_OBJ_TUPLE = 0;
const size_t TYPE_INDEX_IN_OBJ_TUPLE = 1;
const size_t DETAILS_INDEX_IN_OBJ_TUPLE = 2;

Status readInformation(ObjectDataVector_t& objects, FILE* const file)
{
    const size_t MAX_LINE_LENGTH = 1028;

    //some useful lambda functions
    auto trim_string = [] (std::string& str)
      {
        std::stringstream trimmer;
        trimmer << str;
        str.clear();
        trimmer >> str;
      };

    if (file != NULL)
    {
        typedef enum
        {
             LOOKING_FOR_OBJECT,
             LOOKING_FOR_LEFT_CURLY_BRACKET,
             LOOKING_FOR_OBJECT_DETAILS,
             LOOKING_FOR_RIGHT_CURLY_BRACKET
        } AttentionState_t;

        AttentionState_t currentAttentionState = LOOKING_FOR_OBJECT;
        char buffer[MAX_LINE_LENGTH];

        std::string                                     objId;
        std::string                                     objType;
        std::string					objIdAllAlpha;
        std::vector<std::tuple<std::string,std::string, std::string>> objDetails;

        while (!feof(file))
        {
            fgets (buffer , MAX_LINE_LENGTH, file);

            size_t len = strlen(buffer);
            if(buffer[len-1] == '\n')
            {
                buffer[len-1] = 0;
            }

            bool isThereValidComma = checkIfTheTwoCharactersFollow(buffer,'{',',', std::vector<char>{ '}' });

            //tokenize line based on 'attention state'
            switch (currentAttentionState)
            {
                case LOOKING_FOR_OBJECT:
                {
                    char* token;

                    DeLOG(
                        std::string{"Splittng string '"}.append(buffer).append("' into tokens.").c_str()
                    );
                    token = strtok(buffer, ":");
                    if (token == NULL)
                    {
                        //insert temp object cleanup...here
                        break;
                    }
                    std::string identifierName(token);
                    objId = identifierName;
                    DeLOG(
                        std::string{"Identifier string: '"}.append(identifierName).append("' into tokens.").c_str()
                    );


                    token = strtok(NULL, " ");
                    if (token == NULL)
                    {
                        //insert temp object cleanup...here
                        break;
                    }
                    std::string objectType(token);
                    objType = objectType;
                    DeLOG(
                        std::string{"Object-type string: '"}.append(token).append("'\n").c_str()
                    );


                    token = strtok(NULL, " ");
                    if (token == NULL)
                    {
                        //insert temp object cleanup...here
                        break;
                    }
                    if (*token == '{')
                    {
                        currentAttentionState = LOOKING_FOR_OBJECT_DETAILS;
                    }
                    else
                    {
                        currentAttentionState = LOOKING_FOR_LEFT_CURLY_BRACKET;
                    }
                    break;
                }
                case LOOKING_FOR_LEFT_CURLY_BRACKET:
                {
                    DeLOG("case: looking for left curly brace\n");

                    bool isLeftCurlyBraceFirstNonWhiteSpace =
                        confirmFirstNonWhiteSpaceInBuffer(buffer, strlen(buffer), '{');

                    if (isLeftCurlyBraceFirstNonWhiteSpace)
                    {
                        currentAttentionState = LOOKING_FOR_OBJECT_DETAILS;
                    }
                    else
                    {
                        //insert temp object cleanup...here
                        currentAttentionState = LOOKING_FOR_OBJECT;
                    }

                    break;
                }
                case LOOKING_FOR_OBJECT_DETAILS:
                {
                    DeLOG("case: looking for object details");

                    char* token;
          
                    //first look for right curly brace as first non whitespace
                    bool isRightCurlyBraceFirstNonWhiteSpace =
                        confirmFirstNonWhiteSpaceInBuffer(buffer, strlen(buffer), '}');
                    if (isRightCurlyBraceFirstNonWhiteSpace)
                    {
                        DeLOG(std::string("Obj-Id:\t").append(objId).c_str());
                        DeLOG(std::string("Obj-Type:\t").append(objType).c_str());
                        for (auto details : objDetails)
                        {
                            DeLOG(
                                std::string("  Obj-Detail:  ").append(std::get<0>(details)).append(" =").append(std::get<1>(details)).append(", ").append(std::get<2>(details)).c_str()
                            );
                        }

                        std::tuple<std::string,std::string, std::vector<std::tuple<std::string,std::string,std::string>>> createdObj{};
                        std::get<ID_INDEX_IN_OBJ_TUPLE>(createdObj) = objId;
                        std::get<TYPE_INDEX_IN_OBJ_TUPLE>(createdObj) = objType;
                        std::get<DETAILS_INDEX_IN_OBJ_TUPLE>(createdObj) = objDetails;

                        objects.push_back(createdObj);

                        objDetails.clear();
                        objId.clear();
                        objType.clear();

                        currentAttentionState = LOOKING_FOR_OBJECT;
                        //break;
                    }
                    else
                    {

                        //if at any place we encounter something unexpected erase, temp objects and 
                        //change attentionState to LOOKING_FOR_OBJECT (reset)
                        DeLOG(
                            std::string{"Splittng string '"}.append(buffer).append("' into tokens.").c_str()
                        );
                        token = strtok(buffer, "+");
                        if (token == NULL)
                        {
                            //insert temp object cleanup...here
                            DeNEWLINE(); 
                            break;
                        }
                        token = strtok(NULL, "{");
                        if (token == NULL)
                        {
                            //insert temp object cleanup...here
                            DeNEWLINE();
                            break;
                        }
                        std::string keywordName(token);

                        //here we trim the string (we know that there may be a space at end)

                        trim_string(keywordName);

                        DeLOG(
                            std::string{"KeywordName string: '"}.append(keywordName).append("'.").c_str()
                        );


                        //try looking for ',': this means there's > 1 parameter
                        if (isThereValidComma)
                        {
                            DeLOG("Found , trying to get multiple params\n");

                            token = strtok(NULL, ",");
                            std::string keywordGrammarType(token);

                            trim_string(keywordGrammarType);
                            DeLOG(
                                std::string{"KeywordGrammarType string: '"}.append(keywordGrammarType).append("'.\n").c_str()
                            );

                            //here we can do some sort of recursion to keep checking for
                            //extra values.  For Now we look for just ONE.
                            token = strtok(NULL, " ");
                            if (token == NULL)
                            {
                                //this is lax processing...
                                //we skip this [line] and continue
                                // looking for more object details
                                // ...btw, we might want to mark/repory this as a soft error.
                                // . . . chances are they wanted to do something just misformatted it

                                DeNEWLINE();
                                break;
                            }

                            //record second value (specify this here as : keywordNameAllAlpha)
                            std::string keywordNameAllAlpha(token);
                            trim_string(keywordNameAllAlpha);

                            DeLOG(
                                std::string{"KeywordNameAllAlpha string: '"}.append(keywordNameAllAlpha).append("'.").c_str()
                            );

                            objDetails.push_back(std::make_tuple(keywordName, keywordGrammarType, keywordNameAllAlpha));
                        }
                        else
                        {
                            DeLOG("Could not find ',' assuming only single param\n");
                            token = strtok(NULL, " ");
                            if (token == NULL)
                            {
                                //insert temp object cleanup...here
                                DeNEWLINE();
                                break;
                            }
                            std::string keywordGrammarType(token);
                            trim_string(keywordGrammarType);

                            DeLOG(
                                std::string{"KeywordGrammarType string: '"}.append(keywordGrammarType).append("'.\n").c_str()
                            );

                            objDetails.push_back(std::make_tuple(keywordName, keywordGrammarType, keywordName));                    
                        }
                    }

                    break;
                }
                default:
                {
                    DeLOG("case: default\n");
                    break;
                }
            };

            clearArray(buffer, MAX_LINE_LENGTH);
        }
    }
    else
    {
        perror ("Error opening file");        
        return Status{ ModeInfo_t::ERROR };
    }

    return Status{ ModeInfo_t::READ_DONE };
}

Status writeGeneratedContent(const ObjectDataVector_t& objects, const Status& prevStatus)
{
    if (prevStatus.isError())
    {
        perror("Cannot write generated content to file, error occured earlier.");

        return prevStatus;
    } 

    Status currentStatus{prevStatus};

    //we hook all our templates to TemplateSystem, so it can record/use common info - reducing redundancy
    TemplateSystem theMainTemplateSystem(ABSTR_LEXER_WORD_CONSTRUCTOR_FROM_INCLUDES_PATH, ABSTR_GRAMMAR_CONFIG_FROM_INCLUDES_PATH);

    std::vector<std::string> keywordNamesAllAlpha;
    std::vector<std::string> embeddedKeywordNames;

    typedef TemplateSystem::AbstrGrammarConfigTemplate::GrammarKeywordInfo GrammarKeywordInfo_t;
    std::vector<GrammarKeywordInfo_t> grammarKeywordInfoObjs;

    DeLOG("About to write based on objects\n");

    for (auto obj : objects)
    {
        DeLOG("iteration over object\n");
        auto objName = std::get<ID_INDEX_IN_OBJ_TUPLE>(obj);

        auto objType = std::get<TYPE_INDEX_IN_OBJ_TUPLE>(obj);

        if (objName.compare("EmbeddedKeywords") == 0 && objType.compare("functions") == 0)
        {
            const auto objDetailTuples = std::get<DETAILS_INDEX_IN_OBJ_TUPLE>(obj);

            auto onlyKeywordElementDetails = filterTuples(objDetailTuples, FilterType_t::ANYTHING_BUT, std::vector<std::string>{ "path", "elementName" });
            for (auto detailTuple : onlyKeywordElementDetails)
            {
                DeLOG(std::string{"Pushing"}.append(std::get<2>(detailTuple)).append("back to embedded keyword names").c_str());
                embeddedKeywordNames.push_back(std::get<2>(detailTuple));
            } 
        }
        else if (objName.compare("Keywords") == 0 && objType.compare("obj-collection") == 0)
        {
            TemplateSystem::KeywordsTemplate keywordTemplate(&theMainTemplateSystem);

            const auto objDetailTuples = std::get<DETAILS_INDEX_IN_OBJ_TUPLE>(obj);

            const auto matchesForPath =
                    filterTuples(objDetailTuples, FilterType_t::MATCH, "path");

            if (matchesForPath.size() == 0)
            {
                perror(std::string{"Error: A path is REQUIRED for an object defintion. Skipping :"}.append(objName).append(" definition.\n").c_str());

                currentStatus.addModeInfo(ModeInfo_t::ERROR);
            }
            else
            {
                std::string outputFilename = std::get<1>(matchesForPath[0]); 

                theMainTemplateSystem.setGeneratedKeywordsFilePath(outputFilename);

                //if file already exists [readonly], this allows us to overwrite it.
                chmod(outputFilename.c_str(), S_IRUSR | S_IWUSR);
                FILE* outputFile = fopen(outputFilename.c_str(), "wt");

                if (outputFile == NULL)
                {
                    perror(std::string{"Error: Could not open output file:"}.append(outputFilename).append(". Skipping :").append(objName).append(" definition\n").c_str());

                    currentStatus.addModeInfo(ModeInfo_t::ERROR);
                }
                else
                {
                    std::string elementObjName;

                    if (objType.compare("obj-collection") == 0)
                    {
                        const auto matchesForElementName =
                        filterTuples(objDetailTuples, FilterType_t::MATCH, "elementName");

                        elementObjName = (matchesForElementName.size() > 0? std::get<1>(matchesForElementName[0]) : objName);
                    }
                    else
                    {
                        //note: INSIDE template objName is prepended with 'I' or 'Abstr' to make
                        // two seperate classes, so there is no conflict with 'objName' class.
                        elementObjName = objName;
                    }

                    auto signatureAndClose = keywordTemplate.generateClass(objName, elementObjName);

                    std::vector<std::string> ignoreStrings = { "path", "elementName" };

                    auto keywordDefns 
                        = filterTuples(objDetailTuples, FilterType_t::ANYTHING_BUT, ignoreStrings);

                    auto keywordsData = keywordTemplate.generateKeywordsData(keywordDefns);

                    fputs(signatureAndClose.first.c_str(), outputFile); 
                    fputs(keywordsData.c_str(), outputFile);           
                    fputs(signatureAndClose.second.c_str(), outputFile);

                    fclose(outputFile);

                    //Making file 'readonly'. This is not even a real measure. Just another hoop, to remind us NOT to
                    //change files that've been generated.
                    chmod(outputFilename.c_str(), S_IRUSR);

                    //do work to get alpha names from details into a a new vector of strings, then feed that to generateAbstrLexerWordConstructorClass() in KeywordsTemplate

//                    std::vector<std::string> keywordNamesAllAlpha;

                    auto onlyKeywordElementDetails = filterTuples(objDetailTuples, FilterType_t::ANYTHING_BUT, std::vector<std::string>{ "path", "elementName" });
                    for (auto detailTuple : onlyKeywordElementDetails)
                    { 
                        keywordNamesAllAlpha.push_back(std::get<2>(detailTuple));

                        //do processing on obj type as it can be multiple values, seperated by '|'
                        const auto keywordTypesVector = splitStringBy(std::get<1>(detailTuple), std::string("|"));
                        std::string akeywordType;
                        //this is kindof icky, take the first non recursive object type descriptor
                        for (const auto aType : keywordTypesVector)
                        {
                            if (aType.compare("RECURSIVE") != 0)
                            {
                                akeywordType = aType;
                                break;
                            }
                        }

                        if (akeywordType.empty() && keywordTypesVector.size() > 0)
                        {
                            akeywordType = std::string("RECURSIVE");
                        }

                        //while we're at it lets add GrammarKeywordInfo_t to a vector
                        grammarKeywordInfoObjs.push_back(GrammarKeywordInfo_t(std::get<0>(detailTuple), akeywordType, std::get<2>(detailTuple)));
                    }
                }


            }
        }
    }

    DeLOG("\nFinished reading objects\n");

    TemplateSystem::AbstrLexerWordConstructorTemplate lwcTemplate(&theMainTemplateSystem);
    auto lexerWordConstructorStartOfClass = lwcTemplate.generateStartOfClass();
    auto lexerWordConstructorInitAndWordFunctions = lwcTemplate.generateInitAndWordFunctions(keywordNamesAllAlpha);
    auto lexerWordConstructorEmbeddedFunctions = lwcTemplate.generateEmbeddedWordFunctions(embeddedKeywordNames);
    auto lexerWordConstructorEndOfClass = lwcTemplate.generateEndOfClass();

    DeLOG("Finished creating content\n");

    chmod(ABSTR_LEXER_WORD_CONSTRUCTOR_PATH, S_IRUSR | S_IWUSR);

    DeLOG("Finished Changing Permissions Of AbstrLexerWordConstructor.hpp\n");

    FILE* lexerWordConstructorOutputFile = fopen(ABSTR_LEXER_WORD_CONSTRUCTOR_PATH, "wt");

    DeLOG("Opened AbstrLexerWordConstructor.hpp\n");

    fputs(lexerWordConstructorStartOfClass.c_str(), lexerWordConstructorOutputFile);
    fputs(lexerWordConstructorInitAndWordFunctions.c_str(), lexerWordConstructorOutputFile);
    fputs(lexerWordConstructorEmbeddedFunctions.c_str(), lexerWordConstructorOutputFile);
    fputs(lexerWordConstructorEndOfClass.c_str(), lexerWordConstructorOutputFile);

    DeLOG("Finished writing to file\n");

    fclose(lexerWordConstructorOutputFile);

    DeLOG("Closing file");

    chmod(ABSTR_LEXER_WORD_CONSTRUCTOR_PATH, S_IRUSR);

    DeLOG("Changing permissions to READ on written file\n");


//TemplateSystem::AbstrGrammarConfigTemplate::GrammarKeywordInfo
    TemplateSystem::AbstrGrammarConfigTemplate abstrGrammarTemplate(&theMainTemplateSystem);
    chmod(ABSTR_GRAMMAR_CONFIG_PATH, S_IRUSR | S_IWUSR);

    DeLOG("Finished Changing Permissions Of AbstrGrammarConfig.hpp\n");

    FILE* abstrGrammarConfigOutputFile = fopen(ABSTR_GRAMMAR_CONFIG_PATH, "wt");

    DeLOG("Opened AbstrGrammarConfig.hpp\n");

    std::string abstrConfigFileContents = abstrGrammarTemplate.generateClass(grammarKeywordInfoObjs);
    fputs(abstrConfigFileContents.c_str(), abstrGrammarConfigOutputFile);

    DeLOG("Finished writing to file\n");

    fclose(abstrGrammarConfigOutputFile);

    DeLOG("Closing file");

    chmod(ABSTR_GRAMMAR_CONFIG_PATH, S_IRUSR);

    DeLOG("Changing permissions to READ on written file\n");

    if (currentStatus.isError())
    {
        DeLOG("Generation finished with some errors. Generation skips what it can, but it's best to investigate the cause(s)\n");
    }
    else
    {
        DeLOG("Generation finished SUCESSFULLY.");
    }

    return currentStatus;   
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        DeLOG("Usage: ./keywordGen [input-filename.ext]"); 
    }

    ObjectDataVector_t objects;
    FILE* defnFile = fopen(argv[1], "rt");

    DeLOG("About to read file\n");
    const Status readStatus = readInformation(objects, defnFile);

    fclose(defnFile);

    const Status writeStatus = writeGeneratedContent(objects, readStatus);

    return 0;
}

#undef DeLOG
#undef DeNEWLINE

/*
 + Footnote:

     + [1] : Exclusive Hexadecimal Counting
              - made up phrasing, meaning the
                way we count in hexadecimal so that, by assigning each number to a particular 
                named enumeration choice, we can use the logical AND ('&') in combination with 
                the Exclusive OR operation (XOR, '^' in c) to let one unsigned interger variable represent a 
                great /[(many|number)] of combinations of said choices (states, or configurations, or w\e)

                   eg:
                       Thursday=0x0, Friday=0x2, Saturday=0x4, RecognizedNonPartyDay=0x8
                       Partying=0x20, Studying=0x40, DrunkAt2am=0x80

                       This is a particularly terrible example, because, for example we're
                       insuating that it can be Thursday, Friday, AND Saturday all at the same
                       time. Also, Friday may be one of those 'Recognized Non Party Days',
                       which is of course a false reaility. And I've personally never partied and
                       studyed in the same day (we all know that ONE guy, but of course he
                       lies). College students in particular are exempt from the whole
                       RecognizedNonPartyDay concept, alot of people don't like that -
                       especially the ones who missed out.

                       In all seriousnesss,
                       here, correctness isn't as important as is the representation. Ensuring
                       all interesting possibilies are defined and properly enumerated.
                       Maybe we SHOULD enforce correctness here. But my only oppsition to that
                       is that it would require much more thought and definately a bit more
                       work. Furthermore, there's no reason that the rules to restrict this 'system'
                       could not be done at a higher level, where these representations are
                       a given and from that hugher level are unchangeable.

                           Whats particularly good about this, is that we can accurately describe
                           my current situation. It let's me state, correctly, that it's
                           Friday=0x2, I'm Studying=0x40 and I'm NOT partying (which lucky
                           for us, just kindof goes without saying).
                                To represent this as a number stored in 
                                hexadecimal (re:computers) format:

                                    Friday        0x02
                                 +  Studying      0x40
                                 ---------------------
                                    Lame          0x42

                                And it's that simple.
                                 => Notice that leading zeros mean nothing; its to help out the
                                    poor soul who doesn't find hexidecimal intuitive (i.e. me).


                           And just for laugh's (plus some extra education)
                           ...
                           It's  a bit too early (1:00am) to say whether I'll be
                           drunk at 2:00am. But say I am, such a state can be represented
                           by: Friday=0x2, Studying=0x40, DrunkAt2am=0x80
                               To represent this as a number stored in hexadecimal:

                                    Friday        0x02
                                    Studying      0x40
                                 +  DrunkAt2am    0x80
                                 ---------------------
                                    Less Lame     0xc2

                               This may look foreign, but it's no coincidence the 2 at the end
                               is present both here and in our previous example. It's because
                               each digit in a hexadecimal number is like a column, and
                               additions take place inside the columns of those stacked
                               hexadecimal numbers. Your likely familiar with ths. It's not
                               new, it's the same as with the more common decimal represention. 
                               But here, (1) instead of 10 being the limit that can never be 
                               reached it's 16. Oh and (2)because we counted so nicely and don't 
                               permit multiple additions of the same number(name), we needn't 
                               worry about overflow. So technically we can add multiple
                               hexadecimal numbers, specifically  the contents of each column,
                               together in any order we please and the end result will be
                               the same. And so here in this example:
                                  4 + 8 = 12 => c          | a=10, b=11, c=12, ..., f=15.  
                               You may ask, well, what if we were Partying=0x20, Studying=0x40, 
                               and DrunkAt2am=0x80? (The infamous Trifecta) Partying btw 
                               involving at least 3 other people than yourself. Well then we'd 
                               have:
                                   2 + 4 + 8 = 14 => e
                               Which given our scheme, is just about the only way 14(e) will
                               show up. To recap, we have a set of options that can be combined 
                               into a single number, then later picked apart knowing only the 
                               domain of possible options available - this schema works, and it
                               isn't terrible.
*/
