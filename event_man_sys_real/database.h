#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite\sqlite3.h"
//Create the database schema
void createDatabase(sqlite3* db);

#endif // DATABASE_H

