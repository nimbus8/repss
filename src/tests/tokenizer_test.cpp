#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> splitStringBy(const std::string& str, const std::string seperator)
{
    char buffer[str.size()+1];
    char *token;

    strcpy(buffer, str.c_str());

    token = strtok(buffer, seperator.c_str());

    std::vector<std::string> retTokens;
    while (token != NULL)
    {
        retTokens.push_back(std::string(token));
        token = strtok(NULL, seperator.c_str());
    } 

    return retTokens;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        perror("Usage: split [string] [seperator]\n");
    }

    auto tokens = splitStringBy(argv[1], argv[2]);

    for (auto element : tokens)
    {
        std::cout << element << ", ";
    }

    std::cout << std::endl;
}
