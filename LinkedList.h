
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <stdexcept>
#include "Node.h"

using namespace std;



template<typename T>
class LL{
    Node<T> *head;
    Node<T> *tail;

    public:
        LL();
        ~LL();
        void addFront(T d);
        void deleteFront();

        void insertEnd(T d);
        void deleteEnd();

        void display();
        T getFront();
        bool isEmpty();

};

#include "LinkedList.cpp"

#endif
