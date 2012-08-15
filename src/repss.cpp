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
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <future>
#include <thread>
using namespace std;
#include <stdio.h>

#include "includes/repss.hpp"
#include "tests/test_repss_string.cpp"

#include "includes/lexer/lexer_man/lexer_config/lexer_configuration.hpp"
#include "includes/lexer/lexer_man/lexer_manager.hpp"

namespace TATest
{
	char REPSS_SYMB_1[10];
	char REPSS_SYMB_2[10];
	char REPSS_SYMB_3[10];

	void seedLiterals()
	{
		strcpy(REPSS_SYMB_1, DEF_REPSS_SYMB_1);
		strcpy(REPSS_SYMB_2, DEF_REPSS_SYMB_2);
		strcpy(REPSS_SYMB_3, "--");
	}

	/* Fixed Part -- Evaluated at compile time */
	class StackEntry
	{
	public:
		constexpr StackEntry(int nextState, int closureSymbol,
				int prevStackEntryID) :
				_nextState(nextState), _closureSymbol(closureSymbol), _prevStackEntryID(
						prevStackEntryID)
		{
		}
		constexpr double getNextState()
		{
			return _nextState;
		}
	private:
		int _nextState;
		int _closureSymbol;
		int _prevStackEntryID;
	};

//The stack entry index acts like a unique id (stage,symbol)->id
//this is kindof nice I think. This allows some to use this say in a class, override this func to  change the
//way things are parsed. Maybe even just override define statements...
	constexpr int getStackEntryIndex(char stage, char closureSymbol[])
	{
		return (stage == 0 ?
				closureCase(REPSS_SYMB_1,closureSymbol,0,closureCase(REPSS_SYMB_2,closureSymbol,1,-1)) :
				(stage == 1 ?
						closureCase(REPSS_SYMB_1,closureSymbol,10,closureCase(REPSS_SYMB_2,closureSymbol,11,-1)) :
						-1));
	}

	/* we need a way here to ensure what we think is there is there, only then can pass closureSymbol to getStackEntryIndex */
	/* this can be bit tricky, because % and %% are different.  In that case we are waiting for trailing characters like spaced or names*/
//very strange getStackEntry works, but not checkBufferForSymbol when you modify it a little -- something strcmp not being constexpr. Hahah, hack.
	constexpr int checkBufferForSymbol(int stage, char closureSymbol[])
	{
		return (closureCase(REPSS_SYMB_1,closureSymbol,0,
				closureCase(REPSS_SYMB_2, closureSymbol,0,-1)));
	}

	/* Dynamic Part -- Depends on input. We declare list's of variables here. Lookup tables for identifiers. (stage,symbol)->id   */
	void processImpl(char c, int stackEntryIndex)
	{
	}

	/* Fixed + Dynamic - Processor */
	int _currentStage = 0;
	void process(int argc, char *argv[])
	{
		const unsigned int BUFFER_LEN = 256;
		char buffer[BUFFER_LEN];
		unsigned int buffer_count = 0;
		string bufferStr = "";

		if (argc < 4)
		{
			std::cerr
					<< "Error, correct usage:  repss str_cmp1 str_cmp2 input_file"
					<< std::endl;
		}

		try
		{
			File_Handler::File_Handle fileHandle { argv[3], "rt" };

			auto isEndOfFile = File_Handler::rpss_feofFunc(fileHandle);
			auto getCharacter = File_Handler::rpss_fgetcFunc(fileHandle);

			auto appendToBuffer = Arrays::rpss_appendToArrayFunc(buffer,
					buffer_count); //[&buffer,&buffer_count](char c)->void {buffer[buffer_count++];};
			auto clearBuffer = [&buffer](unsigned int n)->void
			{	memset(buffer, '\0', n);};

			clearBuffer(BUFFER_LEN);

			while (!isEndOfFile())
			{
				char c = getCharacter();
				std::cout << "read: " << c << std::endl;

				/* for some reason I can't address indices in the buffer over 16?!!?!!
				 * 	maybe its because I'm low on memory???
				 */
				if (!isEndOfFile() && c != '%'
						&& c != '\n' /*&& buffer_count < 16*/)
				{
					appendToBuffer(c);
				}
				else
				{
					printf("Captured: %s\n", buffer);
					clearBuffer(BUFFER_LEN);
					buffer_count = 0;
					std::cout << "Found something!!" << std::endl;
				}

				std::cout << "done bit";
			}
		}

		catch (File_Handler::File_Not_Found_Error e)
		{
			std::cout << e.what();
			exit(1);
		}

//	processImpl(c, getStackEntryIndex(_currentStage,
	}

	void runTATest(int argc, char *argv[])
	{
		char a[] = "%%";

		seedLiterals();

		int result =
		closureCase(REPSS_SYMB_3,a,555,closureCase(REPSS_SYMB_2,a,556,-333));
		std::cout << "ClosureCase Result: " << result << std::endl;

		auto stackEntryIndex = getStackEntryIndex(1, a);

		std::cout << "Stack entry index: " << stackEntryIndex << std::endl;

		auto stackEntry1 = StackEntry(2, '%', -1);

		std::cout << "Stack entry - nextState: " << stackEntry1.getNextState()
				<< std::endl;

		process(argc, argv);
	}
}

namespace ThreadTesting
{
	void f(std::vector<double>& v)
	{
		for (double i : std::vector<double> { 1.2, 2.2, 3.1 })
		{
			v.push_back(i);
		}
	}

	void testAsync(const vector<double>& some_vec)
	{
		auto foo1 = std::async([]()
		{	return 1;});
		auto foo2 = std::async([]()
		{	return 2;});

		cout << "foo1: " << foo1.get() << ", " << "foo2: " << foo2.get()
				<< '\n';
	}
}

int main(int argc, char* argv[])
{
	try
	{
	std::cout << "== REPSS ==\n" << endl;

	vector<char> character_stream = { 'a', 'b', 'c', 'd', 'e', 'f' };
	vector<char> output_stream;

	auto i = 2;
	auto lambda_func = [&output_stream,&i] (char x, int y) ->int
	{
		output_stream.push_back(x+y+i);
		return i+1;
	};

	std::cout << "(input-stream): " << std::endl;

	for (auto v : character_stream)
	{
		std::cout << v << "\t";
		i = lambda_func(v, 1);
	}

	std::cout << "\n\n(output-stream): " << std::endl;

	for (int k = 0; k < output_stream.size(); k++)
		std::cout << output_stream[k] << "\t";

	std::cout << std::endl << std::endl;

	repss_str_test(argc, argv);

	TATest::runTATest(argc, argv);
	ThreadTesting::testAsync(vector<double> { 1.0, 2.0 });

	//initializing lexer
        const lexer_configuration config;
        const lexer_manager lexMan(&config);
	}
	catch (std::exception& e) 
	{ 
		std::cout << e.what() << std::endl;; 
	}

	return 0;
}
