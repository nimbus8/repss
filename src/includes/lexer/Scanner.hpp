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
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "../file_handler/FileHandler.hpp"
#include "../memory_operations.hpp"

#ifndef _SCANNER_HPP_
#define _SCANNER_HPP_

#define stringify( name ) # name

class PartsOfGrammer
{
private:
	enum defns
  	{
		empty_str = 0,
		key_word,
		data_line
	};

	const char* enumToString[3] = 
	{
		stringify( empty_str ),
		stringify( key_word ),
		stringify( data_line )
	};
public:
	std::string emptyString() { return std::string(enumToString[empty_str]); }
	std::string keyword() { return std::string(enumToString[key_word]); }
	std::string dataLine() { return std::string(enumToString[data_line]); }
};

class Scanner
{
private:
	PartsOfGrammer _partsOfGrammer;

	std::unordered_set<string> _validKeywords;
	std::unordered_map<string,string> _keywordToPartOfGrammer;

	std::string _annotatedData;	

	void _appendToAnnotatedData(const string& data)
	{
		_annotatedData.append(data);	
	}

	void captureBufferAndWrapData(char* const buffer, size_t& bufferCount, const size_t BUFFER_LEN, const bool isKeyword)
	{
		printf("Captured: %s\n", buffer);

		

		if (strlen(buffer) != 0)
		{
                	const std::string wrappedData{ wrapKeyword(isKeyword? _partsOfGrammer.keyword() : _partsOfGrammer.dataLine(), buffer) };
                	_appendToAnnotatedData(wrappedData);
		}
		else
		{
			//could be because of newlines - at the moment
			const std::string wrappedData{ wrapKeyword(_partsOfGrammer.dataLine(), _partsOfGrammer.emptyString().c_str()) };
			_appendToAnnotatedData(wrappedData);
		}

		Arrays::clearCharacters(buffer,BUFFER_LEN);
                bufferCount = 0;
	
		std::cout << "Found something!!" << std::endl;
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
	Scanner() : _annotatedData() {}
	~Scanner() {}

        void processFile(const std::string& filename, const std::string& permissions)
        {
                const size_t BUFFER_LEN = 1048576;
                char buffer[BUFFER_LEN];
                size_t buffer_count = 0;
                std::string bufferStr = "";

                try
                {
                        REPSS_FileHandler::FileHandle fileHandle{filename, permissions};

                        Arrays::clearCharacters(buffer, BUFFER_LEN);

                        while (!REPSS_FileHandler::isEndOfFile(fileHandle))
                        {
                                char c = REPSS_FileHandler::getCharacter(fileHandle);
                                std::cout << "read: " << c << std::endl;

                                if (!REPSS_FileHandler::isEndOfFile(fileHandle)
                                        && c != '%' && c != '\n')
                                {
					std::cout << "Appended To Array" << std::endl;

                                        Arrays::appendCharacter(buffer,buffer_count++,c);

					std::cout << buffer << std::endl;
                                }
                                else
                                {
					bool isKeyword = false;
					if (c == '%')
					{
        	                                if (strlen(buffer) != 0)
	                                        {
							captureBufferAndWrapData(buffer, buffer_count, BUFFER_LEN, isKeyword);
        	                                }

						Arrays::appendCharacter(buffer,buffer_count++,c);
						isKeyword = true;
					}

					captureBufferAndWrapData(buffer, buffer_count, BUFFER_LEN, isKeyword);
                                }

				std::cout << "done bit";
                        }
                }

                catch (REPSS_FileHandler::FileNotFoundError e)
                {
                        std::cout << e.what();
                        exit(1);
                }

		std::cout << std::endl << "Annotated Data " << std::endl << _annotatedData << std::endl;
	}

};

#endif

