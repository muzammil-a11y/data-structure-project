#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include "DataManager.h"
#include "Stack.h"
#include "Constraints.h"
#include "MergeSort.h"

class Backtracking {
private:
    Constraints constraints;
    MergeSort sorter;

    // Sirf is line mein CourseArray& lectureList change hua hai
    bool solve(CourseArray& lectureList, int index, RoomArray& rooms, ScheduleStack& stack);

public:
    bool generateTimetable(DataManager& dm);
};

#endif