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

#include "deps/includes/lexer/Scanner.hpp"
#include "deps/includes/utils/Stopwatch.hpp"

void Scanner::printAnotatedData() const
{
	std::cout << std::endl << "Printing Annotated Data" << std::endl;

	_context->printAnnotatedData();

	std::cout << std::endl << "Finished printing anotated data" <<std::endl;
}

void Scanner::processFile(const std::string& filename, const std::string& permissions)
{
    const size_t BUFFER_LEN = 1048576;
    char buffer[BUFFER_LEN];
    size_t buffer_count = 0;
    std::string bufferStr = "";

    try
    {
        REPSS_FileHandler::FileHandle fileHandle{filename, permissions};

        Arrays::clearCharacters(buffer, BUFFER_LEN);

        StopWatch stopwatch;
        stopwatch.start();

        while (!REPSS_FileHandler::isEndOfFile(fileHandle))
        {
            char c = REPSS_FileHandler::getCharacter(fileHandle);

            if (!REPSS_FileHandler::isEndOfFile(fileHandle)
                  && c != '%' && c != '\n')
            {
                Arrays::appendCharacter(buffer,buffer_count++,c);
                //std::cout << buffer << std::endl;
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
        }

        stopwatch.getElapsedAndPrintfd("\n\nScanning: done scanning in %d milliseconds\n");
    }
    catch (REPSS_FileHandler::FileNotFoundError e)
    {
        std::cout << e.what();
        exit(1);
    }

    printAnotatedData();

}

void Scanner::processFileTest(const std::string& filename, const std::string& permissions)
{
    const size_t BUFFER_LEN = 1048576;
    char buffer[BUFFER_LEN];
    size_t buffer_count = 0;
    std::string bufferStr = "";

    try
    {
        REPSS_FileHandler::FileHandle fileHandle{filename, permissions};

        Arrays::clearCharacters(buffer, BUFFER_LEN);

        StopWatch stopwatch;
        stopwatch.start();

        while (!REPSS_FileHandler::isEndOfFile(fileHandle))
        {
            char c = REPSS_FileHandler::getCharacter(fileHandle);

            if (!REPSS_FileHandler::isEndOfFile(fileHandle)
                  && c != '%' && c != '\n')
            {
                Arrays::appendCharacter(buffer,buffer_count++,c);
                //std::cout << buffer << std::endl;
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
        }

        stopwatch.getElapsedAndPrintfd("\n\nScanning: done scanning in %d milliseconds\n");
    }
    catch (REPSS_FileHandler::FileNotFoundError e)
    {
        std::cout << e.what();
        exit(1);
    }

    printAnotatedData();
}

