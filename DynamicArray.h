#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

template <typename T>

class DynamicArray{
    T *data;
    int count;
    int capacity;

    void grow();

    public:
        DynamicArray();

        ~DynamicArray();

        DynamicArray(DynamicArray &other);

        DynamicArray& operator=(DynamicArray &other);

        void add(T value);

        T& get(int index);

        void set(int index, T value);

        int size() const;

        void clear(); // changed: new function
};

#include "DynamicArray.cpp"

#endif
