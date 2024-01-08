#include <cstdio>
#include <memory>
#include <string>

enum class nodeType{
    start,
    leaf
};

class Encode
{
public:
    Encode(){};
    void readFile(int argc, char** argv);
    void processFile(const char* c);
    void createTree();
    void createCode();
    void codeToWords();
    void writeCode(const char* , FILE* f); //zmienic potem nazwę
    void writeHeader(FILE* file);

private:
    struct node
    {
        const char* character;
        int count;
        char* code;
        nodeType type;
        std::unique_ptr<node> next;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;        
    };

    struct tableOfCodes
    {
        const char* x;
        char code[16];
    };

    std::unique_ptr<node> createNode(const char* c);
    void addNodeToLinkedList(std::unique_ptr<node> p,std::shared_ptr<node> m);
    void generateCode(std::shared_ptr<node> root, char* code);
    void writeBit(int bit, FILE* file);
    char* extractCode(const char* character);

    std::shared_ptr<node> head{nullptr}; //shared ptr will be better option propably
    std::shared_ptr<node> root{nullptr};
    FILE *fp, *fp1;
    std::string compressed_extension{".spd"};
    unsigned char N;
    char padding;
};