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

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

#ifndef _SCANNER_HPP_
#define _SCANNER_HPP_

//todo:will really think about not doing this
#include "../repss.hpp"
#include "../ContextManager.hpp"

#define stringify( name ) # name

class PartsOfGrammer
{
private:
    enum defns
    {
        eempty_str = 0,
        ekey_word,
        edata_line
    };

    const char* enumToString[3] = 
    {
        stringify( empty_str ),
        stringify( key_word ),
        stringify( data_line )
    };
public:
    std::string emptyString() { return std::string(enumToString[eempty_str]); }
    std::string keyword() { return std::string(enumToString[ekey_word]); }
    std::string dataLine() { return std::string(enumToString[edata_line]); }
};

class Scanner
{
private:
    ContextManager::TypedContext<ContextType::AllowedTypes, ContextType::Lexer>* _context;

    PartsOfGrammer _partsOfGrammer;

    std::unordered_set<std::string> _validKeywords;
    std::unordered_map<std::string, std::string> _keywordToPartOfGrammer;

    void _appendToAnnotatedData(const std::string& data)
    {
        _context->appendToAnnotatedData(data);	
    }

    void captureBufferAndWrapData(char* const buffer, size_t& bufferCount, const size_t BUFFER_LEN, const bool isKeyword)
    {
        //printf("Captured: %s\n", buffer);
        bool emptyBuffer = strlen(buffer) == 0? true : false;

        const std::string wrappedData{ 
            wrapKeyword(
                (
                    emptyBuffer?
                    _partsOfGrammer.dataLine()  
                    : (isKeyword? _partsOfGrammer.keyword() : _partsOfGrammer.dataLine())
                )
                , 
                (
                    emptyBuffer?
                    _partsOfGrammer.emptyString().c_str()
                    : buffer 
                )
            )
        };

        _appendToAnnotatedData(wrappedData);

        Arrays::clearCharacters(buffer,BUFFER_LEN);
        bufferCount = 0;
    }

    std::string wrapKeyword(const std::string& partOfGrammer, const char* const keyWord) const
    {
        std::string wrappedString("{");
        wrappedString.append(partOfGrammer);
        wrappedString.append(":");
        wrappedString.append(keyWord);
        wrappedString.append("}");

        return wrappedString;
    }
public:
    Scanner() : _context(nullptr) {}
    explicit Scanner(ContextManager::TypedContext<ContextType::AllowedTypes, ContextType::Lexer>* context) : _context(context) {}
    ~Scanner() {}

    void processFileTest(const std::string& filename, const std::string& permissions);
    void processFile(const std::string& filename, const std::string& permissions);
    void printAnotatedData() const;
};

#endif

