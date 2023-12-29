#include <cstdio>

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
        char c;
        int count;
        char* code;
        int type;
        struct node* next;
        struct node* left;
        struct node* right;        
    };

    node *head = nullptr;
    node *root = nullptr;
    FILE *fp, *fp1;

};