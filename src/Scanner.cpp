#include "includes/lexer/Scanner.hpp"

void Scanner::printAnotatedData() const
{
	std::cout << std::endl << "Printing annotated aata" << std::endl;

	for (auto blockStr : _annotatedData)
	{
		std::cout << blockStr << std::endl;
	}

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


                printAnotatedData();
        }

