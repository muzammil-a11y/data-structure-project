#ifndef MERGESORT_H
#define MERGESORT_H

#include "DynamicArray.h"

class MergeSort {
private:
    bool comesBefore(ScheduleEntry a, ScheduleEntry b);
    void merge(ScheduleArray& arr, int left, int mid, int right);
    void divide(ScheduleArray& arr, int left, int right);

public:
    void sort(ScheduleArray& arr);
};

#endif