#include <stdio.h>
#include "database.h"

void createDatabase(sqlite3* db) {
    char* errMsg = 0;
    const char* sqlEvents = "CREATE TABLE IF NOT EXISTS EVENTS ("
        "ID INT PRIMARY KEY NOT NULL,"
        "NAME TEXT NOT NULL,"
        "LOCATION TEXT NOT NULL,"
        "DATE TEXT NOT NULL,"
        "ACTIVITIES TEXT NOT NULL);";

    const char* sqlAttendees = "CREATE TABLE IF NOT EXISTS ATTENDEES ("
        "ID INT PRIMARY KEY NOT NULL,"
        "NAME TEXT NOT NULL,"
        "EVENT_ID INT NOT NULL);";

    int rc = sqlite3_exec(db, sqlEvents, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    rc = sqlite3_exec(db, sqlAttendees, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}