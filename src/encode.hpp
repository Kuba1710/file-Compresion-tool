#include <cstdio>
#include <memory>

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
    void writeCode(); //zmienic potem nazwę

private:
    struct node
    {
        const char* character;
        int count;
        const char* code;
        nodeType type;
        std::unique_ptr<node> next;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;        
    };

    std::unique_ptr<node> createNode(const char* c);
    void addNodeToLinkedList(std::unique_ptr<node> p,std::unique_ptr<node> m);

    std::unique_ptr<node> head{nullptr}; //shared ptr will be better option propably
    std::unique_ptr<node> root{nullptr};
    FILE *fp, *fp1;
    std::string compressed_extension{".spd"};

};