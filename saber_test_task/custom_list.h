#ifndef CUSTOM_LIST_H
#define CUSTOM_LIST_H
#include <stdio.h>
#include <string>
#include <memory>

//Реализуйте функции сериализации и десериализации двусвязного списка
// в бинарном формате в файл. Алгоритмическая сложность решения должна быть
// меньше квадратичной.


// структуру ListNode модифицровать нельзя
struct ListNode
{
    ListNode * prev;
    ListNode * next;
    // указатель на произвольный элемент данного списка, либо NULL
    ListNode * rand;
    std::string data;
};

class List
{
    using simplechar_t = char;
    using charuptr_t = std::unique_ptr<simplechar_t[]>;
public:
    List() = default;
    List(std::initializer_list<std::string>_list)
    {
        try
        {
            for (auto& str: _list)
            {
                std::unique_ptr<ListNode> ptr = std::make_unique<ListNode>();
                ptr->prev = head;
                ptr->rand = nullptr;
                ptr->next = nullptr;
                ptr->data = str;
                if (head)
                {
                    head->next = ptr.get();
                }
                head = ptr.release();
                if (!tail)
                {
                    tail = head;
                }
            }
        }
        catch (...)
        {
            deinit();
        }
    }
    // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
    void Serialize (FILE * file)
    {
        ListNode * ptrnode = tail;
        while(ptrnode)
        {
            ListNode * ptrtmp = ptrnode;
            auto size = ptrnode->data.size();
            fwrite(ptrnode->data.c_str(), size + 1, 1, file);
            ptrnode = ptrnode->next;
        }
    }
    std::string GetText()
    {
        ListNode * ptrnode = tail;
        std::string result;
        while(ptrnode)
        {
            ListNode * ptrtmp = ptrnode;
            result += ptrnode->data.c_str();
            result += '\n';
            ptrnode = ptrnode->next;
        }
        return result;
    }
    // загрузка из файла (файл открыт с помощью fopen(path, "rb"))
    void Deserialize(FILE * file)
    {
        fseek(file, 0, SEEK_END);
        std::size_t fsize = ftell(file);
        fseek(file, 0, SEEK_SET);
        if (!fsize) return;
        charuptr_t pairbuff;
        simplechar_t * newbuf = new simplechar_t[fsize];
        pairbuff.reset(newbuf);
        // last symbol should be zero
        if (!newbuf[fsize - 1]) throw std::runtime_error("wrong file");
        auto real_size = fread(newbuf, 1, fsize, file);
        if (real_size != fsize) throw std::runtime_error("file io error");
        std::size_t buff_pos = 0;
        while(buff_pos < fsize)
        {
            ListNode * ptrnode = new ListNode;
            ptrnode->prev = head;
            ptrnode->next = nullptr;
            if (head)
            {
                head->next = ptrnode;
            }
            head = ptrnode;
            if (!tail)
            {
                tail = head;
            }
            char* cptr = pairbuff.get() + buff_pos;
            ptrnode->data = cptr;
            buff_pos += ptrnode->data.size() + 1;
        }
    }
    void deinit() const noexcept
    {
        ListNode * ptr = tail;
        while(ptr)
        {
            ListNode * ptrtmp = ptr;
            ptr = ptr->next;
            delete ptrtmp;
        }
    }
    ~List()
    {
        deinit();
    }
private:
    ListNode * head = nullptr;
    ListNode * tail = nullptr;
    int count = 0;
};

#endif // CUSTOM_LIST_H
