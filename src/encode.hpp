#include <cstdio>
#include <memory>
#include <string>

enum class nodeType{
    start,
    leaf
};

struct node
{
    node() = default;
    node(const node& nodeToCopy)
    {
        character = nodeToCopy.character;
        count = nodeToCopy.count;
        code = nodeToCopy.code;
        type = nodeToCopy.type;
        next = std::make_unique<node>(*nodeToCopy.next);
        left = std::make_unique<node>(*nodeToCopy.left);
        right = std::make_unique<node>(*nodeToCopy.right);
    };

    const char* character;
    int count;
    const char* code;
    nodeType type;
    std::shared_ptr<node> next;
    std::shared_ptr<node> left;
    std::shared_ptr<node> right;        
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
    void generateCode(std::shared_ptr<node> root, const char* code);
    std::shared_ptr<node> getRoot() { return root; };

private:

    struct tableOfCodes
    {
        const char* x;
        char code[16];
    };

    std::unique_ptr<node> createNode(const char* c);
    void addNodeToLinkedList(std::unique_ptr<node> p,std::shared_ptr<node> m);
    void writeBit(int bit, FILE* file);
    const char* extractCode(const char* character);

    std::shared_ptr<node> head{nullptr}; //shared ptr will be better option propably
    std::shared_ptr<node> root{nullptr};
    FILE *fp, *fp1;
    std::string compressed_extension{".spd"};
    unsigned char N;
    char padding;
};
