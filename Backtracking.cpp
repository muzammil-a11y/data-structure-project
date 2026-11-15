#include "Backtracking.h"


bool Backtracking::solve(CourseArray& lectureList, int index,
                         RoomArray& rooms, ScheduleStack& stack) {
    // Base case: saare lectures schedule ho gaye
    if (index == lectureList.size()) {
        return true;
    }

    CourseOffering* currentCourse = lectureList.get(index);
    if (currentCourse == nullptr) {
        return solve(lectureList, index + 1, rooms, stack);
    }

    Teacher* teacher = currentCourse->getTeacher();
    Section* section = currentCourse->getSection();

    for (int day = 0; day < 5; day++) {
        for (int slot = 0; slot < 4; slot++) {
            for (int r = 0; r < rooms.size(); r++) {
                Room* room = rooms.get(r);

                if (constraints.isValidSlot(currentCourse, room, day, slot)) {
                    // 1. Assign + Push
                    room->setBusy(day, slot, true);
                    teacher->setBusy(day, slot, true);
                    section->setBusy(day, slot, true);

                    ScheduleEntry entry(currentCourse, room, day, slot);
                    stack.push(entry);

                    
                    if (solve(lectureList, index + 1, rooms, stack)) {
                        return true;
                    }

                    
                    stack.pop();
                    room->setBusy(day, slot, false);
                    teacher->setBusy(day, slot, false);
                    section->setBusy(day, slot, false);
                }
            }
        }
    }

    return false;
}

bool Backtracking::generateTimetable(DataManager& dm) {
    CourseArray& courses = dm.getCourses();
    RoomArray& rooms = dm.getRooms();
    ScheduleArray& finalSchedule = dm.getFinalSchedule();

    finalSchedule.clear();
    dm.resetAllAvailability();

    if (courses.size() == 0 || rooms.size() == 0) {
        return false;
    }

    // Seedha CourseArray use karein — koi double pointer ya delete[] nahi!
    CourseArray lectureList;
    for (int i = 0; i < courses.size(); i++) {
        CourseOffering* c = courses.get(i);
        if (c != nullptr) {
            int count = c->getLecturesPerWeek();
            for (int k = 0; k < count; k++) {
                lectureList.add(c);
            }
        }
    }

    if (lectureList.size() == 0) {
        return false;
    }

    ScheduleStack stack;
    bool success = solve(lectureList, 0, rooms, stack);

    if (success) {
        while (!stack.isEmpty()) {
            finalSchedule.add(stack.pop());
        }
        sorter.sort(finalSchedule);
    }

    return success;
}