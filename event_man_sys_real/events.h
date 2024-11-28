#ifndef EVENTS_H
#define EVENTS_H
#include "sqlite\sqlite3.h"

void addEvent(sqlite3* db, int id, const char *name, const char *location, const char *date, const char *activities);
void viewEvent(sqlite3* db);
void updateEvent(sqlite3* db, int id, const char *name, const char *location, const char *date, const char *ctivities);
void deleteEvent(sqlite3* db, int id);

void addEventMenu(sqlite3* db);
void manageEvent(sqlite3* db);
void updateEventMenu(sqlite3* db);
void deleteEventMenu(sqlite3* db);

#endif // EVENTS_H
