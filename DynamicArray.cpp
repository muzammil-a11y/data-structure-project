#ifndef DYNAMICARRAY_CPP // changed: guard added so g++ *.cpp works
#define DYNAMICARRAY_CPP

#include "DynamicArray.h"
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>

void DynamicArray<T>::grow(){
    int new_cap;

    if (capacity == 0) {
        new_cap = 4;
    }

    else {
        new_cap = capacity * 2;
    }
    T *new_data = new T[new_cap];

    for(int i=0; i<count; i++){
        new_data[i] = data[i];
    }

    delete[] data;
    data = new_data;
    capacity = new_cap;
}

template <typename T>

DynamicArray<T>::DynamicArray(){
    data = nullptr;
    count = 0;
    capacity = 0;
}

template <typename T>

DynamicArray<T>::~DynamicArray(){
    delete[] data;
}

template <typename T>

DynamicArray<T>::DynamicArray(DynamicArray<T> &other){
    count = other.count;
    capacity = other.capacity;

   if(capacity>0){
    data = new T[capacity];
    }
    else{
        data = nullptr;
    }

    for(int i=0; i<count; i++){
        data[i] = other.data[i];
    }
}

template <typename T>

DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T> &other){
    if(this == &other){
        return *this;
    }

    delete[] data;
    count = other.count;

    capacity = other.capacity;
    data = (capacity > 0) ? new T[capacity] : nullptr; // changed: no new T[0] for empty array

    for(int i=0; i<count; i++){
        data[i] = other.data[i];
    }

    return *this;
}

template <typename T>

void DynamicArray<T>::add(T value){
    if(count==capacity){
        grow();
    }

    data[count++] = value;
}

template <typename T>
T& DynamicArray<T>::get(int index){

    if(index<0 || index>=count){
        throw out_of_range("Index out of range");
    }

    return data[index];
}

template <typename T>
void DynamicArray<T>::set(int index, T value){
    get(index) = value;
}

template <typename T>
int DynamicArray<T>::size() const{
    return count;
}

// changed: new function
template <typename T>
void DynamicArray<T>::clear(){
    count = 0;
}

#endif
