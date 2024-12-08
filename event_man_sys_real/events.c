#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fort/fort.h"
#include "events.h"

//Add an event to the database
void addEvent(sqlite3* db, int id, const char* name, const char* location, const char* date, const char* activities) {
    char sql[1024];
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

//View all events in the database
void viewEvent(sqlite3* db) {
    const char* sql = "SELECT E.ID, E.NAME, E.LOCATION, E.DATE, E.ACTIVITIES, COUNT(A.ID) AS ATTENDEE_COUNT "
        "FROM EVENTS E "
        "LEFT JOIN ATTENDEES A ON E.ID = A.EVENT_ID "
        "GROUP BY E.ID, E.NAME, E.LOCATION, E.DATE, E.ACTIVITIES;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return;
    }

    ft_table_t* table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, " Event ID", " Event Type", "Location", "Date", "Activities", "Attendees");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char id_str[15];
        int id = sqlite3_column_int(stmt, 0);
        snprintf(id_str, sizeof(id_str), "%d", id);

        char attendee_count_str[15];
        int attendee_count = sqlite3_column_int(stmt, 5);
        snprintf(attendee_count_str, sizeof(attendee_count_str), "%d", attendee_count);

        ft_write_ln(table,
            id_str,
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_text(stmt, 3),
            sqlite3_column_text(stmt, 4),
            attendee_count_str
        );
    }

    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    sqlite3_finalize(stmt);
}

//Update an event in the database
void updateEvent(sqlite3* db, int id, const char* name, const char* location, const char* date, const char* activities) {
    char sql[1024];
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

//Delete an event from the database
void deleteEvent(sqlite3* db, int id) {
    char sql[256];
    char* errMsg = 0;

    snprintf(sql, sizeof(sql), "DELETE FROM EVENT_FEEDBACKS WHERE EVENT_ID=%d;", id); 
    sqlite3_exec(db, sql, 0, 0, &errMsg);
    snprintf(sql, sizeof(sql), "DELETE FROM EVENTS WHERE ID=%d;", id);
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else {
        fprintf(stdout, "Event deleted successfully\n");
    }
}

//Add attendees feedback for a specific event
void addEventFeedback(sqlite3* db, int event_id, const char* feedback) {
    char sql[1024];
    snprintf(sql, sizeof(sql), "INSERT INTO EVENT_FEEDBACKS (EVENT_ID, FEEDBACK) "
        "VALUES (%d, '%s');", event_id, feedback);

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else {
        fprintf(stdout, "Feedback added successfully\n");
    }
}

void viewEventFeedbacks(sqlite3* db, int event_id) {
    const char* sql = "SELECT FEEDBACK FROM EVENT_FEEDBACKS WHERE EVENT_ID = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, event_id);

    ft_table_t* table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "Feedback");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* feedback = (const char*)sqlite3_column_text(stmt, 0);
        ft_write_ln(table, feedback);
    }

    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    sqlite3_finalize(stmt);
}

//Input and read event details from the user
void addEventMenu(sqlite3* db) {
    int id;
    char* name = (char*)malloc(1000 * sizeof(char));
    char* location = (char*)malloc(1000 * sizeof(char));
    char* date = (char*)malloc(20 * sizeof(char));
    char* activities = (char*)malloc(2000 * sizeof(char));

    while (1) {
        printf("Enter Event ID: ");
        if (scanf_s("%d", &id) == 1 && id > 0) {
            break;
        }
        else {
            printf("Invalid input. Please enter a positive integer.\n");
            while (getchar() != '\n');
        }
    }
    printf("Enter Event Type: ");
    scanf_s(" %[^\n]%*c", name, 1000);
    printf("Enter Event Location: ");
    scanf_s(" %[^\n]%*c", location, 1000);
    printf("Enter Event Date (YYYY-MM-DD): ");
    scanf_s("%s", date, 20);
    printf("Enter Event Activities (X, X...): ");
    scanf_s(" %[^\n]%*c", activities, 2000);

    addEvent(db, id, name, location, date, activities);

    free(name);
    free(location);
    free(date);
    free(activities);
}

//Uodate event details for a specific event
void updateEventMenu(sqlite3* db) {
    int id;
    char* name = (char*)malloc(1000 * sizeof(char));
    char* location = (char*)malloc(1000 * sizeof(char));
    char* date = (char*)malloc(20 * sizeof(char));
    char* activities = (char*)malloc(2000 * sizeof(char));

    printf("Enter Event ID to update: ");
    if (scanf_s("%d", &id) != 1) {
        fprintf(stderr, "Invalid input for Event ID\n");
        return;
    }

    char* checkSql = "SELECT COUNT(*) FROM EVENTS WHERE ID = ?";
    sqlite3_stmt* checkStmt;
    if (sqlite3_prepare_v2(db, checkSql, -1, &checkStmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(checkStmt, 1, id);
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(checkStmt, 0);
            if (count == 0) {
                fprintf(stderr, "Please input an existing Event ID\n");
                sqlite3_finalize(checkStmt);
                free(name);
                free(location);
                free(date);
                free(activities);
                return;
            }
        }
        sqlite3_finalize(checkStmt);
    }
    else {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        free(name);
        free(location);
        free(date);
        free(activities);
        return;
    }

    printf("Enter Event Type: ");
    scanf_s(" %[^\n]%*c", name, 1000);
    printf("Enter Event Location: ");
    scanf_s(" %[^\n]%*c", location, 1000);
    printf("Enter Event Date (YYYY-MM-DD): ");
    scanf_s("%s", date, 20);
    printf("Enter Event Activities (X, X...): ");
    scanf_s(" %[^\n]%*c", activities, 2000);

    updateEvent(db, id, name, location, date, activities);

    free(name);
    free(location);
    free(date);
    free(activities);
}

//Input and read attendees feedback for a specific event
void addEventFeedbackMenu(sqlite3* db) {
    int event_id;
    char* feedback = (char*)malloc(2000 * sizeof(char));

    printf("Enter Event ID: ");
    scanf_s("%d", &event_id);

    char* checkSql = "SELECT COUNT(*) FROM EVENTS WHERE ID = ?";
    sqlite3_stmt* checkStmt;
    if (sqlite3_prepare_v2(db, checkSql, -1, &checkStmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(checkStmt, 1, event_id);
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(checkStmt, 0);
            if (count == 0) {
                fprintf(stderr, "Please input an existing Event ID\n");
                sqlite3_finalize(checkStmt);
                return;
            }
        }
        sqlite3_finalize(checkStmt);
    }
    else {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("Enter Feedback: ");
    scanf_s(" %[^\n]%*c", feedback, 2000);

    addEventFeedback(db, event_id, feedback);

    free(feedback);
}

//Delete an event from the database
void deleteEventMenu(sqlite3* db) {
    int id;
    printf("Enter Event ID to delete: ");
    if (scanf_s("%d", &id) != 1) {
        fprintf(stderr, "Invalid input for Event ID\n");
        return;
    }

    char* checkSql = "SELECT COUNT(*) FROM EVENTS WHERE ID = ?";
    sqlite3_stmt* checkStmt;
    if (sqlite3_prepare_v2(db, checkSql, -1, &checkStmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(checkStmt, 1, id);
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(checkStmt, 0);
            if (count == 0) {
                fprintf(stderr, "Please input an existing Event ID\n");
                sqlite3_finalize(checkStmt);
                return;
            }
        }
        sqlite3_finalize(checkStmt);
    }
    else {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    deleteEvent(db, id);
}

//Display the manage events menu and handle user's choice
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
        printf("|            6    >                     Add Event Feedback                                  |\n");
        printf("|            7    >                    View Event Feedbacks                                 |\n");
        printf("|            8    >                         Go Back                                         |\n");
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
            break;
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
            addEventFeedbackMenu(db);
            break;
        case 7:
            system("cls");
            int event_id;
            printf("Enter Event ID to view feedbacks: ");
            if (scanf_s("%d", &event_id) == 1) {
                viewEventFeedbacks(db, event_id);
            }
            else {
                fprintf(stderr, "Invalid input for Event ID\n");
            }
            break;
        case 8:
            system("cls");
            return;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
}

