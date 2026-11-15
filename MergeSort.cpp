#include "MergeSort.h"

bool MergeSort::comesBefore(ScheduleEntry a, ScheduleEntry b) {
    if (a.getDay() != b.getDay()) {
        return a.getDay() < b.getDay();
    }
    return a.getSlot() <= b.getSlot();
}

void MergeSort::merge(ScheduleArray& arr, int left, int mid, int right) {
    int size = right - left + 1;
    ScheduleEntry* temp = new ScheduleEntry[size];

    int i = left;      
    int j = mid + 1;   
    int k = 0;         

    
    while (i <= mid && j <= right) {
        if (comesBefore(arr.get(i), arr.get(j))) {
            temp[k++] = arr.get(i++);
        } else {
            temp[k++] = arr.get(j++);
        }
    }

    
    while (i <= mid) {
        temp[k++] = arr.get(i++);
    }

    
    while (j <= right) {
        temp[k++] = arr.get(j++);
    }

    
    for (int p = 0; p < size; p++) {
        arr.set(left + p, temp[p]);
    }

    delete[] temp;
}

void MergeSort::divide(ScheduleArray& arr, int left, int right) {
    if (left >= right) return;

    int mid = (left + right) / 2;
    divide(arr, left, mid);
    divide(arr, mid + 1, right);  
    merge(arr, left, mid, right);
}

void MergeSort::sort(ScheduleArray& arr) {
    if (arr.size() > 1) {
        divide(arr, 0, arr.size() - 1);
    }
}