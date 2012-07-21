#include <iostream>
#include <string>
using namespace std;
#include "../includes/repss_str.hpp"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("err, proper usage: test_repss_string str1 str2\n");
		exit(1);
	}
	
	char a[] = "abc";
	char b[] = "abc";
	char str_percent[] = "%%";
	char buffer[10];

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
