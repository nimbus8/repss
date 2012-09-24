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

#include <stdio.h>

#include "deps/includes/lexer/Scanner.hpp"
#include "deps/includes/utils/Stopwatch.hpp"

#define DEBUG YES

#ifdef DEBUG
    #define DLOG(str) printf("%d:%s", __LINE__, str)
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
    const size_t BUFFER_LEN = 1048576;

    char buffer[BUFFER_LEN];
    size_t buffer_count = 0;

    std::string bufferForPossibleKeyword; 
    //used to hold candidate keywords: the minute we begin to FOLLOW a scan word (advance past startScanWordNode), we start adding to this. 
    //and if we error out while FOLLOWING when shift the contents of the candidateKeyWordBuffer onto main buffer.
    //If we reach an accepting node while the contents of the candidateKeyWordBuffer is non empty, this just means
    //that we've successfully recognized a keyword and we make note of that. Emptying the candidate buffer AND
    //before that, making note of what was in the normal (if there is anything in) buffer and clearing it.
    std::string candidateKeyWordBuffer;

    std::string bufferStr = ""; //is this being used?

    DLOG("Starting Scanner::processFile\n"); 

    //okay below, we need to let ILexerDataProxy from ILexerContext
    auto dataProxy = _context->getLexerDataProxy();
    DLOG(" Rerieved lexer data proxy\n");

    auto dfaManager = dataProxy->getDfaManager();
    DLOG(" Retrieved Dfa Manager\n");
    try
    {
        REPSS_FileHandler::FileHandle fileHandle{filename, permissions};

        Arrays::clearCharacters(buffer, BUFFER_LEN);

        auto scanWords = _context->getScanWords();
        
        auto startPlace = scanWords;
        auto currentPlace = startPlace;

        StopWatch stopwatch;
        stopwatch.start();

        bool isFollowingKeyWord = false;

        while (!REPSS_FileHandler::isEndOfFile(fileHandle))
        {
            const char c = REPSS_FileHandler::getCharacter(fileHandle);
            auto nextScanWordNode = (currentPlace == nullptr? nullptr : currentPlace->getNextScanWordNode(c));

            if (nextScanWordNode != nullptr)
            {
                //first, if after reading character we care NOT at end of file
                // and second check where we are in terms of scanwords and line breaks.
                if (/* case 0: */ !REPSS_FileHandler::isEndOfFile(fileHandle)
                    /* case 1: */ && currentPlace != startPlace
                    /* case 2: */ && !(dfaManager->isAcceptingNode(nextScanWordNode->getId()))
                    /* case 3: */ && c != '\n')
                {
                    //- If case for where after we've read a character and we know it
                    //doesn't look like a keyword is present at this point (already determined
                    //we're NOT at startplace/acceptingplace or a newline/endoffile) so further
                    //we're not (in the process of) following the progression of a certain scanword
                    //past the startplace. So we add to main buffer.
                    //- else case where we ARE following the progression of some scanword(could be just one 
                    //or even a group - the group representing scanwords(recognized keywords) with identical 
                    //uninterrupted transitions from the startplace). In this case we add to the candidateKeyWord buffer.

                    if (!isFollowingKeyWord)
                    {    
                        Arrays::appendCharacter(buffer,buffer_count++,c); 
                        DLOG(buffer);
                    }
                    else
                    {
                        candidateKeyWordBuffer.append(&c, 1);
                        DLOG((candidateKeyWordBuffer.c_str()));
                    }
                }
                else
                {
                    bool isKeyword = false;

                    if (currentPlace == startPlace)
                    {
                        //we start recording
                        //Arrays::appendCharacter(buffer,buffer_count++,c);
                        candidateKeyWordBuffer.append(&c, 1);
                        currentPlace = nextScanWordNode;

                        DLOG((candidateKeyWordBuffer.c_str()));
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
                                //oh yeah it should be clear that capture is to "take, basically clearing".
                                //not too obvious...name change required
                                captureBufferAndWrapData(buffer, buffer_count, BUFFER_LEN, isKeyword);
                            }

                            //here we must add the keyword "the official name"
                            std::string scanWordName(dfaManager->getAcceptingNodeName(nextScanWordNode->getId()));
                            strcpy(scanWordNameBuffer, scanWordName.c_str());
                        }

                        size_t nameBufferLen = strlen(scanWordNameBuffer);
                        captureBufferAndWrapData(scanWordNameBuffer, nameBufferLen, 50, isKeyword);

                        currentPlace = startPlace;
                    }

                    /*
                    if (c == '%')
                    {
                        if (strlen(buffer) != 0)
                        {
                            //oh yeah it should be clear that capture is to "take, basically clearing".
                            //not too obvious...name change required
                            captureBufferAndWrapData(buffer, buffer_count, BUFFER_LEN, isKeyword);
                        }

                        Arrays::appendCharacter(buffer,buffer_count++,c);
                        isKeyword = true;
                    }

                    captureBufferAndWrapData(buffer, buffer_count, BUFFER_LEN, isKeyword);
                    */
                }
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

