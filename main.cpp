
#include <iostream>
#include "LinkedList.h"
#include "DataManager.h"
#include "Stack.h"
#include "AVLTree.h" // changed: added
#include "Backtracking.h" // changed: added

using namespace std;

int main(){

    cout << "========== LINKED LIST ==========" << endl;

    LL<int> list;

    list.addFront(10);
    list.addFront(20);
    list.addFront(30);

    cout << "List: ";
    list.display();

    list.insertEnd(40);
    list.insertEnd(50);

    cout << "List: ";
    list.display();

    list.deleteFront();

    cout << "List: ";
    list.display();

    list.deleteEnd();

    cout << "List: ";
    list.display();


    cout << "\n========== STACK ==========" << endl;

    StackClass<int> stack;

    stack.push(10);
    stack.push(20);
    stack.push(30);

    cout << "Top: " << stack.peek() << endl;
    cout << "Size: " << stack.size() << endl;

    stack.pop();

    cout << "Top: " << stack.peek() << endl;
    cout << "Size: " << stack.size() << endl;


    cout << "\n========== DATA MANAGER ==========" << endl;

    DataManager manager;

    manager.loadSampleData();

    cout << "Teachers: " << manager.teachers.size() << endl;
    cout << "Rooms: " << manager.rooms.size() << endl;
    cout << "Schedules: " << manager.schedules.size() << endl;
    cout << "Course Offerings: " << manager.getCourses().size() << endl; // changed: was hardcoded "4"
    cout << "Schedule Stack: " << manager.scheduleStack.size() << endl;


    // changed: everything below is new, tests Member 2's part
    cout << "\n========== AVL TREE ==========" << endl;

    CourseAVLTree tree;

    for(int i=0; i<manager.getCourses().size(); i++){
        tree.insert(manager.getCourses().get(i));
    }

    int ids[] = {201, 203, 204, 999};

    for(int i=0; i<4; i++){
        CourseOffering* c = tree.search(ids[i]);

        cout << "Search " << ids[i] << ": ";
        if(c != nullptr){
            cout << c->courseName << endl;
        }
        else{
            cout << "not found" << endl;
        }
    }


    cout << "\n========== TIMETABLE ==========" << endl;

    Backtracking solver;
    bool ok = solver.generateTimetable(manager);

    if(!ok){
        cout << "No valid timetable found." << endl;
        return 1;
    }

    const char* dayNames[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};
    ScheduleArray& result = manager.getFinalSchedule();

    cout << "Entries: " << result.size() << endl;

    for(int i=0; i<result.size(); i++){
        ScheduleEntry& e = result.get(i);

        string course = "";
        string teacher = "";
        string room = "";

        CourseOffering* c = tree.search(e.courseID);
        if(c != nullptr){
            course = c->courseName;
        }

        for(int t=0; t<manager.teachers.size(); t++){
            if(manager.teachers.get(t).teacherID == e.teacherID){
                teacher = manager.teachers.get(t).name;
            }
        }

        for(int r=0; r<manager.rooms.size(); r++){
            if(manager.rooms.get(r).roomID == e.roomID){
                room = manager.rooms.get(r).roomName;
            }
        }

        cout << dayNames[e.getDay()] << " slot " << e.getSlot() << " | "
             << course << " | " << teacher << " | " << room << endl;
    }

    return 0;
}
