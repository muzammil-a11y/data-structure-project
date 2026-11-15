#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H
#include "Models.h"
class Constraints
{
private:
    bool isteacheravailable(Teacher* t,int day,int slot);
    bool isroomavailable(Room* r,int day,int slot);
    bool issectionavailable(Section* s,int day,int slot);
    bool isRoomTypeMatch(CourseOffering* c, Room* r);

public:
    bool isValidSlot(CourseOffering* c, Room* r, int day, int slot);
};



#endif