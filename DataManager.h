#ifndef DATAMANAGER_H
#define DATAMANAGER_H

// Stack.cpp calls list.popFront(), but LL only has deleteFront().
// LinkedList can't be edited, so map the name here. This must come BEFORE
// Stack.h is included so that Stack.cpp's templates see the replacement.
#define popFront deleteFront

#include "Models.h"
#include "DynamicArray.h"
#include "LinkedList.h"
#include "Stack.h"

typedef StackClass<ScheduleEntry> ScheduleStack;

class DataManager{
    public:
        DynamicArray<Teacher> teachers;
        DynamicArray<Room> rooms;
        DynamicArray<ScheduleEntry> schedules;
        DynamicArray<Section> sections;

        StackClass<ScheduleEntry> scheduleStack;

        LL<CourseOffering*> courseOfferings;

        // courses owns the CourseOffering objects (same pointers as in courseOfferings)
        CourseArray courses;
        RoomArray roomPtrs;
        ScheduleArray finalSchedule;

        ~DataManager();

        void loadSampleData();

        // call again if teachers/rooms/sections are added later
        void linkData();
        void resetAllAvailability();

        CourseArray& getCourses(){
            return courses;
        }

        RoomArray& getRooms(){
            return roomPtrs;
        }

        ScheduleArray& getFinalSchedule(){
            return finalSchedule;
        }
};

#endif
