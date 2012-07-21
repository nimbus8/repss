#include <iostream>
#include <vector>
using namespace std;
#include "includes/repss.hpp"
#include "tests/test_repss_string.cpp"

int main(int argc, char* argv[]) {
  std::cout << "== REPSS ==\n" << endl;

  vector<char> character_stream = { 'a', 'b', 'c', 'd', 'e', 'f' };
  vector<char> output_stream;

  auto i = 2;
  auto lambda_func = [&output_stream,&i] (char x, int y) ->int {
    output_stream.push_back(x+y+i);
    return i+1;
  };

  std::cout << "(input-stream): " << std::endl;

  for(auto v : character_stream)
  {
        std::cout << v << "\t";
        i = lambda_func(v,1);
  }

  std::cout << "\n\n(output-stream): " << std::endl;

  for(int k=0;k<output_stream.size();k++)
    std::cout << output_stream[k] << "\t";

  std::cout << std::endl << std::endl;

  repss_str_test(argc,argv);

  return 0;
}
