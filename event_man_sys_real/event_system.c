#include <stdio.h>
#include <stdlib.h>
#include "sqlite\sqlite3.h"
#include "events.h"
#include "attendees.h"
#include "database.h"

//Waits for user input to continue
void continuebutton() {
    printf("Press Enter to go back to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

//Opens up the database
int main() {
    sqlite3* db;
    int rc = sqlite3_open("event_management.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else {
        fprintf(stderr, "------------------------------------------------------------------------------------------------------------------------\n");
    }



    createDatabase(db);
	//Main menu
    int choice;
    while (1) {
        printf("|======================================================================================================================|\n");
        printf("|                                                                                                                      |\n");
        printf("|                                                \033[1;36m Event Management System\033                                              |\n");
        printf("|                                                                                                                      |\n");
        printf("|======================================================================================================================|\n");
		printf("|   1  > Add Event   |   2  > Add Attendee   |   3  > View Event   |   4  > View Attendee   |\n");
        printf("|-------------------------------------------------------------------------------------------|\n");
        printf("|             5  > Manage Event              |             6  > Manage Attendee             |\n");
        printf("|-------------------------------------------------------------------------------------------|\n");
        printf("                                        |7 > Exit|                                           \n");
        printf("                                        -----------                                          \n");
		printf("                                                                                             \n");
        printf("                            PLEASE ENTER THE NUMBER OF YOUR CHOICE                           \n");
        scanf_s("%d", &choice);

		//Switch case to handle the user's choice
        switch (choice) {
        case 1:
			system("cls");
            addEventMenu(db);
            break;
        case 2:
			system("cls");
            addAttendeeMenu(db);
            break;
        case 3:
			system("cls");
            viewEvent(db);
            continuebutton();
            system("cls");
            break;
        case 4:
			system("cls");
            viewAttendeeByEventMenu(db);
            continuebutton();
            system("cls");
            break;
        case 5:
			system("cls");
            manageEvent(db);
            break;
        case 6:
			system("cls");
			manageAttendees(db);
			break;  
        case 7:
			system("cls");
            sqlite3_close(db);
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
	//Close the database
    sqlite3_close(db);
    return 0;
}