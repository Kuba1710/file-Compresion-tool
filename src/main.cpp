#include "encode.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    auto encode = std::make_unique<Encode>();
    auto root = encode->getRoot(); // slabe rozwiazanie potem trzeba zmienic
    
    encode->readFile(argc, argv);
    encode->createTree();
    encode->generateCode(root, "\0");

    FILE *fp, *fp1; // to potem tez wywalic do osobnej funkcji
    fp=fopen(argv[1],"r");
    fp1=fopen(argv[2],"wb");

    encode->writeHeader(fp1);

    const char* ch;
    while(fread(&ch,sizeof(char),1,fp)!=0)
        encode->writeCode(ch, fp1);

    fclose(fp1);

    return 0;
    
}