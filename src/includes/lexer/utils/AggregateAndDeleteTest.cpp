#include "AggregatePtrsAndDelete.hpp"

AggregatePtrsAndDelete<char*>* test()
{
        char *chars = new char[10];
	chars[0] = 'H';
	chars[1] = 'E';
	chars[2] = 'L';
	chars[3] = 'L';
	chars[4] = 'O';
	chars[5] = '\0';

        char *chars1 = new char[10];
        chars1[0] = 'P';
        chars1[1] = 'E';
        chars1[2] = 'A';
        chars1[3] = 'R';
        chars1[4] = '\0';	

	AggregatePtrsAndDelete<char*>* agg = 
		new AggregatePtrsAndDelete<char*>(2, chars, chars1);

	return agg;
}

int main()
{
	auto aggRet = test();	
	
	aggRet->applyDelete();
}
