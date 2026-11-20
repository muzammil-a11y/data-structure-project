
#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>
#include "LinkedList.h"

using namespace std;

template <typename T>
class StackClass{
    LL<T> list;
    int count;

    public:
        StackClass();

        void push(T d);
        bool isEmpty();
        T pop();
        T peek();
        int size();
};

#include "Stack.cpp"

#endif
