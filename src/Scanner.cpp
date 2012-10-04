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

#include <iostream>
#include <stdio.h>

#include "deps/includes/lexer/Scanner.hpp"
#include "deps/includes/utils/Stopwatch.hpp"
#include "deps/includes/lexer/construction/model_representation/dfa_manager.hpp"

#define DEBUG YES

#ifdef DEBUG
    #define DLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str)
#else
    #define DLOG(str) 
#endif

void Scanner::printAnotatedData() const
{
	std::cout << std::endl << "Printing Annotated Data" << std::endl;

	_context->printAnnotatedData();

	std::cout << std::endl << "Finished printing anotated data" <<std::endl;
}

void Scanner::processFile(const std::string& filename, const std::string& permissions)
{
    DLOG("Starting Scanner::processFile\n");

    const size_t BUFFER_LEN = 1048576;

    char buffer[BUFFER_LEN];
    size_t buffer_count = 0;

    //used to hold candidate keywords: the minute we begin to FOLLOW a scan word (advance past startScanWordNode), we start adding to this. 
    //and if we error out while FOLLOWING we shift the contents of the candidateKeyWordBuffer onto main buffer.
    //If we reach an accepting node while the contents of the candidateKeyWordBuffer is non empty, this just means
    //that we've successfully recognized a keyword and we make note of that. Emptying the candidate buffer AND
    //before that, making note of what was in the normal buffer (if anything) and clearing it.
    std::string candidateKeyWordBuffer;

    const ILexerDataProxy* dataProxy = _context->getLexerDataProxy();
    const DfaManager* dfaManager = dataProxy->getDfaManager();

    try
    {
        REPSS_FileHandler::FileHandle fileHandle{filename, permissions};

        Arrays::clearCharacters(buffer, BUFFER_LEN);

        auto scanWords = dataProxy->getRecognizedKeywords();

        auto startPlace = scanWords;
        auto currentPlace = startPlace;

        StopWatch stopwatch;
        stopwatch.start();

        bool isFollowingKeyWord = false;

        while (!REPSS_FileHandler::isEndOfFile(fileHandle))
        {
            const char c = REPSS_FileHandler::getCharacter(fileHandle);

            const ScanWordNode* nextScanWordNode = (currentPlace == nullptr? nullptr : currentPlace->getNextScanWordNode(c));

            if (nextScanWordNode != nullptr)
            {
                //first, if after reading character we care NOT at end of file
                // and second check where we are in terms of scanwords and line breaks.
                if (/* case 0: */ !REPSS_FileHandler::isEndOfFile(fileHandle)
                    /* case 1: */ && currentPlace != startPlace
                    /* case 2: */ && !(dfaManager->isAcceptingNode(nextScanWordNode->getId()))
                    /* case 3: */ && c != '\n')
                {
                    DLOG("currentPlace != startPlace && NOT ACCEPTING NODE && NOT NEWLINE\n");

                    if (!isFollowingKeyWord)
                    {    
                        Arrays::appendCharacter(buffer,buffer_count++,c); 
                    }
                    else
                    {
                        candidateKeyWordBuffer.append(&c, 1);
 
                        currentPlace = nextScanWordNode;
                    }
                }   
                else
                {
                    bool isKeyword = false;

                    if (currentPlace == startPlace)
                    {
                        //we start recording
                        candidateKeyWordBuffer.append(&c, 1);
                        currentPlace = nextScanWordNode;

                        isFollowingKeyWord = true;
                    }
                    else
                    {
                        bool isKeyword = false;

                        char scanWordNameBuffer[50];
                        scanWordNameBuffer[0] = '\0';

                        if (dfaManager->isAcceptingNode(nextScanWordNode->getId()))
                        {
                            //we stop recording, label with end state name
                            if (strlen(buffer) > 0)
                            {
                                //oh yeah it should be clear that capture is to "take, and to clear".
                                //not too obvious...name change required
                                captureBufferAndWrapData(buffer, buffer_count, BUFFER_LEN, isKeyword);
                            }

                            candidateKeyWordBuffer.append(&c, 1);

                            //here we must add the keyword's "official name"
                            std::string scanWordName(dfaManager->getAcceptingNodeName(nextScanWordNode->getId()));
                            scanWordName.append("=<'");
                            scanWordName.append(candidateKeyWordBuffer);
                            scanWordName.append("'>");
                            strcpy(scanWordNameBuffer, scanWordName.c_str());
                            isKeyword = true;
                        }
                        
                        //currently the only other case we handle here, without entering above block
                        //i.e. we're not at accepting state - but we've been following scanword lead
                        //if the current character is \n, theres no point in adding it as it's implied
                        //(line delimited scanning)

                        size_t nameBufferLen = strlen(scanWordNameBuffer);
                        captureBufferAndWrapData(scanWordNameBuffer, nameBufferLen, 50, isKeyword);

                        currentPlace = startPlace;
                        isFollowingKeyWord = false;
                        candidateKeyWordBuffer.clear();
                    }
                }
            }
            else
            {
                bool isKeyword = false;

                //transfer whatever (if anything) is in the 
                if (candidateKeyWordBuffer.size() > 0)
                {
                    //this really means that we chased a keyword and came up short
                    //so transfer contents
                    size_t candidateKeyWordBufferSize = candidateKeyWordBuffer.size();
                    for (size_t i = 0; i < candidateKeyWordBufferSize; i++)
                    {
                        buffer[buffer_count++] = candidateKeyWordBuffer.at(i);
                    }

                    candidateKeyWordBuffer.clear();
                }
 
                if (c == '\n' || c == '%')
                {
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
                else
                {
                    Arrays::appendCharacter(buffer, buffer_count++,c);
                }

                currentPlace = startPlace;
            }
        }

        //check if there is still anything in the keyword buffer, if there is add it to main buffer
        //and it as the contents of a dataline
        if (candidateKeyWordBuffer.size() > 0)
        {
            //...
        }
        
        if (strlen(buffer))
        {
            //if there is still stuff in the buffer append to annotation data as a dataline
            //...
        }

        stopwatch.getElapsedAndPrintfd("\n\nScanning: done scanning in %3.3f milliseconds\n");

        saveAnnotatedData();
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

        saveAnnotatedData();
    }
    catch (REPSS_FileHandler::FileNotFoundError e)
    {
        std::cout << e.what();
        exit(1);
    }

    printAnotatedData();
}

