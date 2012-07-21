#include <iostream>
#include <string>
using namespace std;
#include "../includes/repss_str.hpp"

int repss_str_test(int argc, char *argv[])
{
	if (argc < 3)
	{
	    char firstArg[] = "== repss-str-test ==\n";
	    char secondArg[] = "adda";
	    char thirdArg[] = "abbba";
        char *argv2[] = {firstArg,secondArg,thirdArg};

        argv = argv2;
	}
	
	char a[] = "abc";
	char b[] = "abc";
	char str_percent[] = "%%";
	char buffer[10];

    cout << argv[0] << endl;

	cout << b << "==" << a << ": " << repss_str::strcmp_ce(b,a) << " len(a): " << repss_str::strlen_ce(a) << endl;	
	cout << buffer << "==" << str_percent << ": " << repss_str::strcmp_ce(buffer,str_percent) << endl;
	buffer[0] = '%';
	cout << buffer << "==" << str_percent << ": " << repss_str::strcmp_ce(buffer,str_percent) << endl;
	cout << argv[1] << "==" << argv[2] << ": " << repss_str::strcmp_ce(argv[1], argv[2]) << ", len(a):" << repss_str::strlen_ce(argv[1]) << " len(b):" << repss_str::strlen_ce(argv[2]) << endl;
    buffer[1] = '%';
    cout << buffer << "==" << str_percent << ": " << repss_str::strcmp_ce(buffer,str_percent) << endl;
	buffer[1] = '!';
    cout << buffer << "==" << str_percent << ": " << repss_str::strcmp_ce(buffer,str_percent) << endl;
    buffer[1] = '%';
    cout << buffer << "==" << str_percent << ": " << repss_str::strcmp_ce(buffer,str_percent) << endl;
    buffer[2] = '\0';
    cout << buffer << "==" << str_percent << ": " << repss_str::strcmp_ce(buffer,str_percent) << endl;

	return 0;
}
