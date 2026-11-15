#include "Constraints.h"

bool Constraints::isroomavailable(Room* r,int day,int slot){
    return !r->getBusy(day,slot);
}
bool Constraints::issectionavailable(Section* r,int day,int slot){
    return !r->getBusy(day,slot);
}
bool Constraints::isteacheravailable(Teacher* r,int day,int slot){
    return !r->getBusy(day,slot);
}
bool Constraints::isRoomTypeMatch(CourseOffering* c, Room* r) {
    return c->getIsLab() == r->getIsLab();
}
bool Constraints::isValidSlot(CourseOffering* c, Room* r, int day, int slot) {
    if (c == nullptr || r == nullptr) return false;

    Teacher* t = c->getTeacher();
    Section* s = c->getSection();

    if (t == nullptr || s == nullptr) return false;

    return isroomavailable(r, day, slot) &&
           isteacheravailable(t, day, slot) &&
           issectionavailable(s, day, slot) &&
           isRoomTypeMatch(c, r);
}