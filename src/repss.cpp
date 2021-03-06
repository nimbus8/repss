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

 If you wish to contact the author of REPSS, you may do so at
 kwillia.pub@gmail.com
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

#include "base/includes/repss.hpp"
#include "base/includes/ContextManager.hpp"
#include "base/includes/PhasedExecution.hpp"
#include "base/includes/execution_phase/grammaticalForm/GrammarKeywords.hpp"

#include "tests/test_repss_string.cpp"

#define DEBUG
#ifdef DEBUG
    #define DeLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str);
    #define DLOG(str) printf("%s %d:%s", __FILE__, __LINE__, str)
#else
    #define DeLOG(str)
    #define DLOG(str)
#endif

std::string createCopyright();

namespace ImplTest
{
	/*
		Much of the below, although cool, unfortunately became somewhat hectic. 
		todo: Erase much of the below.	The Scanner tests stays.
	*/
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

	int _currentStage = 0;

	void runClosureAndScanTests(ContextManager* const contextMan, int argc, char *argv[])
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

        //lambdas are cool, its nice C++ now has them standard
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

        ThreadTesting::testAsync(vector<double> { 1.0, 2.0 });

        PhasedExecution program;
        program.execute(argc, argv);

        //std::testGrammarKeywords();
    }
    catch (std::exception& e) 
    { 
        std::cout << "REPSS: Exception!!!" << e.what() << std::endl;; 
    }

    return 0;
}

std::string createCopyright()
{
    std::string str;

    str.append("/*\n");
    str.append(" REPSS\n");
    str.append(" Copyright (C) 2012,2013  Khalique Williams\n");
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

#undef DEBUG
#undef DLOG
#undef DeLOG
