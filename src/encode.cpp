#include "encode.hpp"
#include "string"
#include "iostream"
#include <cstdlib>

void Encode::readFile(int argc, char** argv)
{
    const char* ch;
    int t;
    if(argc <= 2)
    {
        argv[2]=(char *)std::malloc(sizeof(char)*(std::strlen(argv[1])+std::strlen(compressed_extension.c_str())+1));
        std::strcpy(argv[2],argv[1]);
        std::strcpy(argv[2],compressed_extension.c_str());
        argc++;
    }
    else{ return; };

    fp = fopen(argv[1], "rb");
    if(fp == nullptr){ std::cout << "error, wrong input"; return;};
    while(fread(&ch,sizeof(char),1,fp)!=0) { processFile(ch);};
    fclose(fp);
};

void Encode::processFile(const char* c)
{
    std::unique_ptr<Encode::node> p, q, m;

    if(head == nullptr)
    {
        head = createNode(c);
        return;
    }

    p = std::make_unique<Encode::node>(*head);
    q = nullptr;

    if(p->character == c)
    {
        p->count+=1;
        if(p->next == nullptr)
            return;
        if(p->count > p->next->count)
        {
            head = std::move(p->next);
            addNodeToLinkedList(std::move(p), head);
        }
        return;
    }

    while(p->next != nullptr && p->character != c)
    {
        q = std::move(p);
        p = std::move(q->next);
    }

    if(p->character == c)
    {
        p->character += 1;
        if(p->next == nullptr)
            return;
        if(p->count > p->next->count)
        {
            q->next = std::move(p->next);
            addNodeToLinkedList(std::move(p), std::move(head));
        };
    }
    else
    {
        q = createNode(c);
        q->next = std::make_unique<Encode::node>(*head);
        head = std::move(q);
    }
}

void Encode::addNodeToLinkedList(std::unique_ptr<Encode::node> p, std::shared_ptr<Encode::node> m)
{
    if(m->next == nullptr)
    {
        m->next = std::move(p);
        return;
    }

    while(m->next->count < p->count)
    {
        m = std::move(m->next);
        if(m->next == nullptr)
        {
            m->next = std::move(p);
            return;
        }
    }

    p->next = std::move(m->next);
    m->next = std::move(p);
}

std::unique_ptr<Encode::node> Encode::createNode(const char* c)
{
    auto temp = std::make_unique<node>();
    temp->character = c;
    temp->count = 1;
    temp->type = nodeType::leaf;
    temp->next = nullptr;
    temp->left = nullptr;
    temp->right = nullptr;
    return temp;
}

void Encode::createTree()
{
    std::unique_ptr<Encode::node> p;
    std::unique_ptr<Encode::node> q;
    p = std::make_unique<Encode::node>(*head);

    while( p != nullptr)
    {
        q = createNode("@");
        q->type = nodeType::start;
        q->left = std::move(p);
        q->count = p->count;
        if(p->next != nullptr)
        {
            p = std::move(p->next);
            q->right = std::move(p);
            q->count += p->count;
        }

        p = std::move(p->next);
        if( p == nullptr)
            break;
        
        if(q->count <= p->count)
        {
            q->next = std::move(p);
            p = std::move(q);
        }
        else
            addNodeToLinkedList(std::move(q), std::move(p));
    }

    root = std::move(q);
}

void Encode::generateCode(std::shared_ptr<Encode::node> p, char* code)
{
    char* leftCode;
    char* rightCode;
    static std::unique_ptr<Encode::node> s;
    static int flag;

    if( p != nullptr)
    {
        if(p->type == nodeType::leaf)
        {
            if(flag == 0)
            {
                flag = 1;
                head = p;
            }
            else
                s->next = std::make_unique<Encode::node>(*p);
            
            p->next = nullptr;
            s = std::make_unique<Encode::node>(*p);
        }

        p->code = code;
        leftCode = (char *)malloc(strlen(code)+2);
        rightCode = (char *)malloc(strlen(code)+2);
        generateCode(std::make_shared<Encode::node>(p->left), leftCode);
        generateCode(std::make_shared<Encode::node>(p->right), rightCode);
    }
}

void Encode::writeHeader(FILE* file)
{
    tableOfCodes table;
    std::unique_ptr<Encode::node> p = std::make_unique<Encode::node>(*head);
    int temp = 0;
    int i = 0;

    while( p != nullptr)
    {
        temp+=(strlen(p->code)) * (p->count);
        if(strlen(p->code) > 16)
            std::cout << "code is too long";
        temp %= 8;
        i++;
        p = std::move(p->next);
    }

    i == 256 ? N = 0 : N = i;

    p = std::make_unique<Encode::node>(*head);

    while( p != nullptr)
    {
        table.x = p->character;
        strcpy(table.code, p->code);
        fwrite(&table,sizeof(tableOfCodes),1,file);
        p = std::move(p->next);
    }

    padding = 8-(char)temp;
    fwrite(&padding,sizeof(char),1,file);
    for(i=0;i<padding;i++)
        writeBit(0,file);
}

void Encode::writeBit(int bit, FILE* file)
{
    static char byte;
    static char count;
    char temp;

    if(bit == 1)
    {
        temp = 1;
        temp = temp<<(7-count);
        byte = byte | temp;
    }

    count++;

    if(count == 8)
    {
        fwrite(&byte, sizeof(char), 1, file);
        count = 0;
        byte = 0;
        return;
    }
    return;
}

void Encode::writeCode(const char* character, FILE* file)
{
    char* code;
    code = extractCode(character);
    while( *code != '\0')
    {
        *code == '1' ? writeBit(1, file) : writeBit(0, file);
        code++;
    }
}

char* Encode::extractCode(const char* character)
{
    std::unique_ptr<node> p = std::make_unique<Encode::node>(*head);

    while( p != nullptr)
    {
        if( p->character == character)
            return p->code;
        
        p = std::move(p->next);
    }

    return nullptr;
}
