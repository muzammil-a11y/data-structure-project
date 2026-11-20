#include "DataManager.h"

DataManager::~DataManager(){
    // CourseOffering objects were created with new in loadSampleData
    for(int i=0; i<courses.size(); i++){
        delete courses.get(i);
    }
    courses.clear();
}

void DataManager::loadSampleData(){

    Teacher teacher1;
    teacher1.teacherID = 1;
    teacher1.name = "Ali";
    teacher1.department = "Computer Science";
    teachers.add(teacher1);

    Teacher teacher2;
    teacher2.teacherID = 2;
    teacher2.name = "Sara";
    teacher2.department = "Software Engineering";
    teachers.add(teacher2);

    Teacher teacher3;
    teacher3.teacherID = 3;
    teacher3.name = "Ahmed";
    teacher3.department = "Artificial Intelligence";
    teachers.add(teacher3);


    Room room1;
    room1.roomID = 101;
    room1.roomName = "Lab A";
    room1.capacity = 30;
    room1.isLab = true;
    rooms.add(room1);

    Room room2;
    room2.roomID = 102;
    room2.roomName = "Room B";
    room2.capacity = 40;
    rooms.add(room2);

    Room room3;
    room3.roomID = 103;
    room3.roomName = "Room C";
    room3.capacity = 25;
    rooms.add(room3);


    Section section1;
    section1.sectionID = 1;
    section1.name = "BSCS-3A";
    sections.add(section1);


    CourseOffering* course1 = new CourseOffering();
    course1->courseID = 201;
    course1->courseName = "Programming Fundamentals";
    course1->creditHours = 3;
    course1->teacherID = 1;
    course1->roomID = 101;
    course1->isLab = true;
    courseOfferings.insertEnd(course1);
    courses.add(course1);

    CourseOffering* course2 = new CourseOffering();
    course2->courseID = 202;
    course2->courseName = "Data Structures";
    course2->creditHours = 3;
    course2->teacherID = 2;
    course2->roomID = 102;
    courseOfferings.insertEnd(course2);
    courses.add(course2);

    CourseOffering* course3 = new CourseOffering();
    course3->courseID = 203;
    course3->courseName = "Database Systems";
    course3->creditHours = 3;
    course3->teacherID = 3;
    course3->roomID = 103;
    courseOfferings.insertEnd(course3);
    courses.add(course3);

    CourseOffering* course4 = new CourseOffering();
    course4->courseID = 204;
    course4->courseName = "Discrete Mathematics";
    course4->creditHours = 3;
    course4->teacherID = 1;
    course4->roomID = 102;
    courseOfferings.insertEnd(course4);
    courses.add(course4);


    ScheduleEntry entry1;
    entry1.courseID = 201;
    entry1.teacherID = 1;
    entry1.roomID = 101;
    entry1.day = 0;
    entry1.timeSlot = 0;

    teachers.get(0).isBusy[0][0] = true;
    rooms.get(0).isBusy[0][0] = true;

    schedules.add(entry1);
    scheduleStack.push(entry1);


    ScheduleEntry entry2;
    entry2.courseID = 202;
    entry2.teacherID = 2;
    entry2.roomID = 102;
    entry2.day = 0;
    entry2.timeSlot = 1;

    teachers.get(1).isBusy[0][1] = true;
    rooms.get(1).isBusy[0][1] = true;

    schedules.add(entry2);
    scheduleStack.push(entry2);


    ScheduleEntry entry3;
    entry3.courseID = 203;
    entry3.teacherID = 3;
    entry3.roomID = 103;
    entry3.day = 1;
    entry3.timeSlot = 0;

    teachers.get(2).isBusy[1][0] = true;
    rooms.get(2).isBusy[1][0] = true;

    schedules.add(entry3);
    scheduleStack.push(entry3);


    ScheduleEntry entry4;
    entry4.courseID = 204;
    entry4.teacherID = 1;
    entry4.roomID = 102;
    entry4.day = 1;
    entry4.timeSlot = 1;

    teachers.get(0).isBusy[1][1] = true;
    rooms.get(1).isBusy[1][1] = true;

    schedules.add(entry4);
    scheduleStack.push(entry4);

    linkData();
}

// connects teacher/section/room pointers; call after all adds are done
// (the pointers point into the arrays, so adding more items later invalidates them)
void DataManager::linkData(){
    roomPtrs.clear();

    for(int i=0; i<rooms.size(); i++){
        roomPtrs.add(&rooms.get(i));
    }

    for(int k=0; k<courses.size(); k++){
        CourseOffering* c = courses.get(k);

        c->teacher = nullptr;
        for(int i=0; i<teachers.size(); i++){
            if(teachers.get(i).teacherID == c->teacherID){
                c->teacher = &teachers.get(i);
                break;
            }
        }

        if(sections.size() > 0){
            c->section = nullptr;
            for(int i=0; i<sections.size(); i++){
                if(sections.get(i).sectionID == c->sectionID){
                    c->section = &sections.get(i);
                    break;
                }
            }
        }
        else{
            c->section = nullptr;
        }

        if(c->lecturesPerWeek == 0){
            c->lecturesPerWeek = c->creditHours;
        }
    }
}

void DataManager::resetAllAvailability(){
    for(int i=0; i<teachers.size(); i++){
        teachers.get(i).resetBusy();
    }

    for(int i=0; i<rooms.size(); i++){
        rooms.get(i).resetBusy();
    }

    for(int i=0; i<sections.size(); i++){
        sections.get(i).resetBusy();
    }
}
