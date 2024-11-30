#include <stdio.h>
#include <string.h>
#include "fort/fort.h"
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

    ft_table_t* table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "ID", "Name", "Event ID");

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        rowsFound = 1;
        char id_str[15];
        int id = sqlite3_column_int(stmt, 0);
        snprintf(id_str, sizeof(id_str), "%d", id);

        ft_write_ln(table,
            id_str,
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2)
        );
    }

    if (!rowsFound) {
        printf("No attendees found for Event ID %d.\n", eventId);
    }
    else {
        printf("%s\n", ft_to_string(table));
    }

    ft_destroy_table(table);
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

    addAttendee(db, id, name, eventID);
}

void viewAttendeeByEventMenu(sqlite3* db) {
    int eventId;
    printf("Enter Event ID to view attendees: ");
    if (scanf_s("%d", &eventId) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid Event ID.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

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
        while (getchar() != '\n');
        fprintf(stderr, "Invalid input. Please enter a valid Attendee ID.\n");
        return;
    }

    while (getchar() != '\n');

    deleteAttendee(db, id);
}

void manageAttendance(sqlite3* db) {
    int choice;
    while (1) {
        printf("|===========================================================================================|\n");
        printf("|                                       Manage Attendees                                    |\n");
        printf("|===========================================================================================|\n");
        printf("|            1    >                       Add Attendee                                      |\n");
        printf("|            2    >                      View Attendees                                     |\n");
        printf("|            3    >                     Update Attendees                                    |\n");
        printf("|            4    >                     Delete Attendees                                    |\n");
        printf("|            5    >                         Go Back                                         |\n");
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
            viewAttendeeByEventMenu(db);
            break;
        case 3:
            system("cls");
            updateAttendeeMenu(db);
            break;
        case 4:
            system("cls");
            deleteAttendeeMenu(db);
            break;
        case 5:
            system("cls");
            return;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
}
