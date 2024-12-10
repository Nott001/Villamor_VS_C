#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "sqlite\sqlite3.h"
#include "events.h"
#include "attendees.h"
#include "database.h"

//Waits for user input to go back
void gobackbutton() {
    printf("Press Enter to go back...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

//Waits for user input to continue
void continuebutton() {
    printf("\n\n                                               Press Enter to continue...");
    while (getchar() != '\n');  // Wait for a single Enter key press
}

//Opens up the database
int main() {
    sqlite3* db;
    int rc = sqlite3_open("event_management.db", &db);
    setlocale(LC_ALL, "");

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else {
        fprintf(stderr, "                                           -WELCOME TO EVENT MANAGEMENT SYSTEM-\n");
    }

    printf("\n\n\n\n");
    printf("                                                \033[1;34m_______  __    __  _____    \n");
    printf("                                               |   ____||  \\  /  |/ ____|   \n");
    printf("                                               |  |__   |   \\/   || (___    \n");
    printf("                                               |   __|  |  |\\/|  | \\___ \\ \n");
    printf("                                               |  |____ |  |  |  | ____) |  \n");
    printf("                                               |_______||__|  |__||_____/\033[0m   \n");
    continuebutton();
    system("cls");


    createDatabase(db);
	//Main menu
    int choice;
    while (1) {
        printf("|======================================================================================================================|\n");
        printf("|                                                                                                                      |\n");
        printf("|                                                \033[1;36mEvent Management System\033[0m                                               |\n");
        printf("|                                                                                                                      |\n");
        printf("|======================================================================================================================|\n");
		printf("|       \033[1;36m1  > Add Event\033[0m      |       \033[1;36m2  > Add Attendee\033[0m      |       \033[1;36m3  > View Event\033[0m      |       \033[1;36m4  > View Attendee\033[0m     |\n");
        printf("|----------------------------------------------------------------------------------------------------------------------|\n");
        printf("|                     \033[1;36m5  > Manage Event\033[0m                    |                    \033[1;36m6  > Manage Attendee\033[0m                   |\n");
        printf("|----------------------------------------------------------------------------------------------------------------------|\n");
        printf("                                                       |\033[1;36m7 > Exit\033[0m|                                          \n");
        printf("                                                       ----------                                          \n");
		printf("                                                                                                            \n");
        printf("                                           PLEASE ENTER THE NUMBER OF YOUR CHOICE                           \n");
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
            gobackbutton();
            system("cls");
            break;
        case 4:
			system("cls");
            viewAttendeeByEventMenu(db);
            gobackbutton();
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