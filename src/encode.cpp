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