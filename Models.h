#ifndef MODELS_H
#define MODELS_H

#include <iostream>
#include <string>
#include "DynamicArray.h"

using namespace std;

const int DAYS = 5;
const int SLOTS = 4;

class Teacher{
    public:
        int teacherID;
        string name;
        string department;
        bool isBusy[DAYS][SLOTS] = {};

        bool getBusy(int day, int slot){
            return isBusy[day][slot];
        }

        void setBusy(int day, int slot, bool value){
            isBusy[day][slot] = value;
        }

        void resetBusy(){
            for(int d=0; d<DAYS; d++){
                for(int s=0; s<SLOTS; s++){
                    isBusy[d][s] = false;
                }
            }
        }
};

class Room{
    public:
        int roomID;
        string roomName;
        int capacity;
        bool isLab = false;
        bool isBusy[DAYS][SLOTS] = {};

        bool getIsLab(){
            return isLab;
        }

        bool getBusy(int day, int slot){
            return isBusy[day][slot];
        }

        void setBusy(int day, int slot, bool value){
            isBusy[day][slot] = value;
        }

        void resetBusy(){
            for(int d=0; d<DAYS; d++){
                for(int s=0; s<SLOTS; s++){
                    isBusy[d][s] = false;
                }
            }
        }
};

class Section{
    public:
        int sectionID;
        string name;
        bool isBusy[DAYS][SLOTS] = {};

        bool getBusy(int day, int slot){
            return isBusy[day][slot];
        }

        void setBusy(int day, int slot, bool value){
            isBusy[day][slot] = value;
        }

        void resetBusy(){
            for(int d=0; d<DAYS; d++){
                for(int s=0; s<SLOTS; s++){
                    isBusy[d][s] = false;
                }
            }
        }
};

class CourseOffering{
    public:
        int courseID;
        string courseName;
        int creditHours;
        int teacherID;
        int roomID;
        int sectionID = 1;  
        Teacher* teacher = nullptr;
        Section* section = nullptr;
        bool isLab = false;
        int lecturesPerWeek = 0;

        int getId(){
            return courseID;
        }

        Teacher* getTeacher(){
            return teacher;
        }

        Section* getSection(){
            return section;
        }

        bool getIsLab(){
            return isLab;
        }

        int getLecturesPerWeek(){
            return lecturesPerWeek;
        }
};

class ScheduleEntry{
    public:
        int courseID;
        int teacherID;
        int roomID;
        int day;
        int timeSlot;

        ScheduleEntry(){}

        ScheduleEntry(CourseOffering* c, Room* r, int d, int s){
            courseID = c->courseID;
            teacherID = c->teacherID;
            roomID = r->roomID;
            day = d;
            timeSlot = s;
        }

        int getDay(){
            return day;
        }

        int getSlot(){
            return timeSlot;
        }
};

typedef DynamicArray<CourseOffering*> CourseArray;
typedef DynamicArray<Room*> RoomArray;
typedef DynamicArray<ScheduleEntry> ScheduleArray;

#endif