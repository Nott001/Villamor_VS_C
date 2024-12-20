#ifndef ATTENDEES_H
#define ATTENDEES_H

#include "sqlite\sqlite3.h"
//Header file for attendees.c
void addAttendee(sqlite3* db, int id, const char* name, int eventID);
void viewAttendeeByEvent(sqlite3* db, int eventID);
void updateAttendee(sqlite3* db, int id, const char* name, int eventID);
void deleteAttendee(sqlite3* db, int id);
void deleteAttendeesByEventID(sqlite3* db, int eventID);

void viewAttendeeByEventMenu(sqlite3* db);
void manageAttendees(sqlite3* db);
void addAttendeeMenu(sqlite3* db);
void updateAttendeeMenu(sqlite3* db);
void deleteAttendeeMenu(sqlite3* db);

#endif // ATTENDEES_H
