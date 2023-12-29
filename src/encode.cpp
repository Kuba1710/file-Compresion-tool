#include "encode.hpp"
#include "string"
#include <cstdlib>

void Encode::readFile(int argc, char** argv)
{
    char ch;
    int t;
    if(argc <= 2)
    {
        argv[2]=(char *)std::malloc(sizeof(char)*(std::strlen(argv[1])+std::strlen(compressed_extension)+1));
        std::strcpy(argv[2],argv[1]);
        std::strcpy(argv[2],compressed_extension);
        argc++;
    }
};