#include <stdio.h>
#include <string.h>
#include "attendees.h"

void addAttendee(sqlite3* db, int id, const char* name, int eventID) {
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO ATTENDEES (ID, NAME, EVENT_ID) "
        "VALUES (%d, '%s', %d);", id, name, eventID);

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else {
        fprintf(stdout, "Attendee added successfully\n");
    }
}

void updateAttendee(sqlite3* db, int id, const char* name, int eventID) {
    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE ATTENDEES SET NAME='%s', EVENT_ID=%d WHERE ID=%d;",
        name, eventID, id);

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else {
        fprintf(stdout, "Attendee updated successfully\n");
    }
}

void viewAttendeeByEvent(sqlite3* db, int eventId) {
    const char* sql = "SELECT * FROM ATTENDEES WHERE EVENT_ID = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    rc = sqlite3_bind_int(stmt, 1, eventId);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }

    printf("\nAttendees for Event ID %d:\n", eventId);
    int rowsFound = 0;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        rowsFound = 1;
        printf("ID: %d, Name: %s, Event ID: %d\n",
            sqlite3_column_int(stmt, 0),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_int(stmt, 2));
    }

    if (!rowsFound) {
        printf("No attendees found for Event ID %d.\n", eventId);
    }
    else if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error retrieving data: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void deleteAttendee(sqlite3* db, int id) {
    char sql[256];
    snprintf(sql, sizeof(sql), "DELETE FROM ATTENDEES WHERE ID=%d;", id);

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else {
        fprintf(stdout, "Attendee deleted successfully\n");
    }
}

void addAttendeeMenu(sqlite3* db) {
    int id, eventID;
    char name[50];
    printf("Enter Attendee ID: ");
    if (scanf_s("%d", &id) != 1) {
        // Clear the input buffer
        while (getchar() != '\n');
        fprintf(stderr, "Invalid input. Please enter a valid Attendee ID.\n");
        return;
    }

    // Clear the input buffer
    while (getchar() != '\n');

    printf("Enter Attendee Name: ");
    if (scanf_s(" %[^\n]%*c", name, (unsigned int)(sizeof(char) * 50)) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid Name.\n");
        return;
    }

    printf("Enter Event ID: ");
    if (scanf_s("%d", &eventID) != 1) {
        // Clear the input buffer
        while (getchar() != '\n');
        fprintf(stderr, "Invalid input. Please enter a valid Event ID.\n");
        return;
    }

    // Clear the input buffer
    while (getchar() != '\n');

    addAttendee(db, id, name, eventID);
}

void viewAttendeeByEventMenu(sqlite3* db) {
    int eventId;
    printf("Enter Event ID to view attendees: ");
    if (scanf_s("%d", &eventId) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid Event ID.\n");
        return;
    }
    viewAttendeeByEvent(db, eventId);
}

void updateAttendeeMenu(sqlite3* db) {
    int id, eventID;
    char name[50];
    printf("Enter Attendee ID to update: ");
    if (scanf_s("%d", &id) != 1) {
        while (getchar() != '\n');
        fprintf(stderr, "Invalid input. Please enter a valid Attendee ID.\n");
        return;
    }
    while (getchar() != '\n');

    printf("Enter Attendee Name: ");
    if (scanf_s(" %[^\n]%*c", name, (unsigned int)(sizeof(char) * 50)) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid Name.\n");
        return;
    }
    printf("Enter Event ID: ");
    if (scanf_s("%d", &eventID) != 1) {
        while (getchar() != '\n');
        fprintf(stderr, "Invalid input. Please enter a valid Event ID.\n");
        return;
    }
    while (getchar() != '\n');
    updateAttendee(db, id, name, eventID);
}

void deleteAttendeeMenu(sqlite3* db) {
    int id;
    printf("Enter Attendee ID to delete: ");
    if (scanf_s("%d", &id) != 1) {
        // Clear the input buffer
        while (getchar() != '\n');
        fprintf(stderr, "Invalid input. Please enter a valid Attendee ID.\n");
        return;
    }

    // Clear the input buffer
    while (getchar() != '\n');

    deleteAttendee(db, id);
}

void manageAttendance(sqlite3* db) {
    int choice;
    while (1) {
        printf("|===========================================================================================|\n");
        printf("|                                         Manage Events                                     |\n");
        printf("|===========================================================================================|\n");
        printf("|            1    >                         Add Attendee                                    |\n");
        printf("|            2    >                        View Events                                      |\n");
        printf("|            3    >                       Update Events                                     |\n");
        printf("|            4    >                       Delete Events                                     |\n");
        printf("|            5    >                    View Event Attendees                                 |\n");
        printf("|            6    >                         Go Back                                         |\n");
        printf("|===========================================================================================|\n");
        printf("                                        Enter your choice:                                   \n");
        if (scanf_s("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice) {
        case 1:
            system("cls");
            addAttendeeMenu(db);
            break;
        case 2:
            system("cls");
			updateAttendeeMenu(db);
		case 3: 
            system("cls");
            deleteAttendeeMenu(db);
            break;
        case 4:
            system("cls");
            viewAttendeeByEventMenu(db);
            break;
        case 5:
            system("cls");
            return;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
}