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
    void processFile(char c);
    void createTree();
    void createCode();
    void codeToWords();
    void writeCode(); //zmienic potem nazwę

private:
    struct node
    {
        char character;
        int count;
        char* code;
        nodeType type;
        std::unique_ptr<node> next;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;        
    };

    std::unique_ptr<node> createNode(char c);
    void addNodeToLinkedList(std::unique_ptr<node> p,std::unique_ptr<node> m);

    std::unique_ptr<node> head{nullptr};
    std::unique_ptr<node> root{nullptr};
    FILE *fp, *fp1;
    std::string compressed_extension{".spd"};

};