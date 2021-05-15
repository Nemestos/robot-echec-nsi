#pragma once
#include "LinkedList.h"
template <class T>
class Queue
{
private:
    LinkedList<T> m_queue;

public:
    void push_back(T elt)
    {
        this->m_queue.add(elt);
    };
    T pop_front()
    {
        if (this->m_queue.size() > 0)
        {
            return this->m_queue.shift();
        }
        return nullptr;
    };
    void clear_queue()
    {
        this->m_queue.clear();
    };
    T get_first()
    {
        if (this->m_queue.size() > 0)
        {
            return this->m_queue.get(0);
        }
        return nullptr;
    };

    int get_lenght()
    {
        return this->m_queue.size();
    }
};
