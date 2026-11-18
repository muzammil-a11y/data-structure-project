#ifndef STACK_CPP // changed: guard added so g++ *.cpp works
#define STACK_CPP

#include "Stack.h"

template <typename T>
StackClass<T>::StackClass(){
    count = 0;
}

template <typename T>
void StackClass<T>::push(T d){
    list.addFront(d);
    count++;
}

template <typename T>
bool StackClass<T>::isEmpty(){
    return list.isEmpty();
}

template <typename T>
T StackClass<T>::pop(){
    T value = list.getFront();
    list.popFront();
    count--;
    return value;
}

template <typename T>
T StackClass<T>::peek(){
    return list.getFront();
}

template <typename T>
int StackClass<T>::size(){
    return count;
}

#endif
