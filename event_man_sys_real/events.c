#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fort\fort.h"
#include "events.h"

void addEvent(sqlite3* db, int id, const char* name, const char* location, const char* date, const char* activities) {
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO EVENTS (ID, NAME, LOCATION, DATE, ACTIVITIES) "
        "VALUES (%d, '%s', '%s', '%s', '%s');", id, name, location, date, activities);

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else {
        fprintf(stdout, "Event added successfully\n");
    }
}

void viewEvent(sqlite3* db) {
    const char* sql = "SELECT * FROM EVENTS;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return;
    }

    ft_table_t* table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table,"ID", "Name", "Location", "Date", "Activities");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char id_str[15];
        int id = sqlite3_column_int(stmt, 0);
        snprintf(id_str, sizeof(char) * 15, "%d", id);

        ft_write_ln(table,
            id_str,
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_text(stmt, 3),
            sqlite3_column_text(stmt, 4)
        );
    }

    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    sqlite3_finalize(stmt);
}

void updateEvent(sqlite3* db, int id, const char* name, const char* location, const char* date, const char* activities) {
    char sql[256];
    snprintf(sql, sizeof(sql), "UPDATE EVENTS SET NAME='%s', LOCATION='%s', DATE='%s', ACTIVITIES='%s' WHERE ID=%d;",
        name, location, date, activities, id);

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else {
        fprintf(stdout, "Event updated successfully\n");
    }
}

void deleteEvent(sqlite3* db, int id) {
    char sql[256];
    snprintf(sql, sizeof(sql), "DELETE FROM EVENTS WHERE ID=%d;", id);

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else {
        fprintf(stdout, "Event deleted successfully\n");
    }
}

void addEventMenu(sqlite3* db) {
    int id;
    char name[50], location[50], date[10], activities[100];
    printf("Enter Event ID: ");
    scanf_s("%d", &id);
    printf("Enter Event Name: ");
    scanf_s(" %[^\n]%*c", name, (unsigned int)(sizeof(char) * 50));
    printf("Enter Event Location: ");
    scanf_s(" %[^\n]%*c", location, (unsigned int)(sizeof(char) * 50));
    printf("Enter Event Date (YYYY-MM-DD): ");
    scanf_s("%s", date, (unsigned int)(sizeof(char) *  10));
    printf("Enter Event Activities: ");
    scanf_s(" %[^\n]%*c", activities, (unsigned int)(sizeof(char) * 100));
    addEvent(db, id, name, location, date, activities);
}

void updateEventMenu(sqlite3* db) {
    int id;
    char name[50], location[50], date[10], activities[100];
    printf("Enter Event ID to update: ");
    scanf_s("%d", &id);
    printf("Enter Event Name: ");
    scanf_s(" %[^\n]%*c", name, (unsigned int)(sizeof(char) * 50));
    printf("Enter Event Location: ");
    scanf_s(" %[^\n]%*c", location, (unsigned int)(sizeof(char) * 50));
    printf("Enter Event Date (YYYY-MM-DD): ");
    scanf_s("%s", date, (unsigned int)(sizeof(char) * 10));
    printf("Enter Event Activities: ");
    scanf_s(" %[^\n]%*c", activities, (unsigned int)(sizeof(char) * 100));
    updateEvent(db, id, name, location, date, activities);
}

void deleteEventMenu(sqlite3* db) {
    int id;
    printf("Enter Event ID to delete: ");
    scanf_s("%d", &id);
    deleteEvent(db, id);
}

void manageEvent(sqlite3* db) {
    int choice;
    while (1) {
        printf("|===========================================================================================|\n");
        printf("|                                         Manage Events                                     |\n");
        printf("|===========================================================================================|\n");
        printf("|            1    >                         Add Event                                       |\n");
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
            addEventMenu(db);
            break;
		case 2:
			system("cls");
			viewEvent(db);
			break;
        case 3:
            system("cls");
			updateEventMenu(db);
        case 4:
            system("cls");
            deleteEventMenu(db);
            break;
        case 5:
            system("cls");
            viewAttendeeByEventMenu(db);
            break;
        case 6:
			system("cls");
            return;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
}
