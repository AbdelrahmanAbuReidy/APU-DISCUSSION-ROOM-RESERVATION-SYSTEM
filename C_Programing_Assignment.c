#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_BOOKINGS 100 
#define MAX_USERS 100

typedef struct // A structure that will hold each student bookings information
{
    char name[50];
    int id;
    int room;
    int timeslot;
    int endTimeslot;
} Booking;

typedef struct // A stucture that will hold each student registration information
{
    char name[50];
    int tpNumber;
    char password[50];
} User;

// Function prototypes
void displayMenu();
void displayRegistration();
void bookingManagement(Booking *bookings, int *numBookings, int *checkTP, int (*availableSlots)[12]);
void registerUser(User *users, int *numUsers);
bool signINuser(User *users, int *numUsers, int *checkTP);
bool uniquenessOftpNumber(User *users, int *numUsers, int tpNumber);
void selectBlockandRoom(int *numRooms, int *minRoomNumber, char *blockName);
void viewAvailableSlots(Booking *bookings, int *numBookings, int (*availableSlots)[12], User *users, int *numUsers, int *checkTP);
void bookNewSlot(Booking *bookings, int *numBookings, int (*availableSlots)[12], User *users, int *numUsers, int *checkTP);
void viewExistingBookings(Booking *bookings, int *numBookings, int *checkTP);
void editExistingBooking(Booking *bookings, int *numBookings, int *checkTP, int (*availableSlots)[12]);
void cancelExistingBooking(Booking *bookings, int *numBookings, int *checkTP, int (*availableSlots)[12]);
void saveBookingsToFile(Booking *bookings, int *numBookings);
void loadBookingsFromFile(Booking *bookings, int *numBookings);
void saveUsersToFile(User *users, int *numUsers);
void loadUsersFromFile(User *users, int *numUsers);
void clearInputBuffer();

/////////////////////////////////////////////////////////////////////////////////////////////////////

void displayRegistration()
{
    printf("***************************************\n");
    printf("*                                     *\n");
    printf("*            REGISTRATION             *\n");
    printf("*                                     *\n");
    printf("***************************************\n");
    printf("*                                     *\n");
    printf("*  1. Sign up                         *\n");
    printf("*  2. Sign in                         *\n");
    printf("*  3. Exit                            *\n");
    printf("*                                     *\n");
    printf("***************************************\n");
    printf("Enter your choice: ");
}

void displayMenu()
{
    printf("\n\n");
    printf("***************************************\n");
    printf("*                                     *\n");
    printf("*                MENU                 *\n");
    printf("*                                     *\n");
    printf("***************************************\n");
    printf("*                                     *\n");
    printf("*  1. View Available Slots            *\n");
    printf("*  2. Book a New Slot                 *\n");
    printf("*  3. Booking Management              *\n");
    printf("*  4. Exit                            *\n");
    printf("*                                     *\n");
    printf("***************************************\n");
    printf("Enter your choice: ");
}

void bookingManagement(Booking *bookings, int *numBookings, int *checkTP, int (*availableSlots)[12])
{
    int option;
    do
    {
        printf("***************************************\n");
        printf("*                                     *\n");
        printf("*  1. View Existing Bookings          *\n");
        printf("*  2. Edit an Existing Booking        *\n");
        printf("*  3. Cancel an Existing Booking      *\n");
        printf("*  4. Back to the Main Menu           *\n");
        printf("*                                     *\n");
        printf("***************************************\n");
        printf("Enter your choice: ");
        scanf("%d",&option);
        switch (option)
        {
        case 1:
            viewExistingBookings(bookings, numBookings, checkTP);
            break;
        case 2:
            editExistingBooking(bookings, numBookings, checkTP, availableSlots); 
            break;
        case 3:
            cancelExistingBooking(bookings, numBookings, checkTP, availableSlots);
            break;
        case 4:
            return;
        default:
            printf("\n\n\t~~~> WRONG CHOICE, PLEASE TRY AGAIN <~~~\n\n");
            continue;
        }

    } while (1); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool uniquenessOftpNumber(User *users, int *numUsers, int tpNumber)
{
    for (int i = 0; i < *numUsers; i++)
    {
        if (users[i].tpNumber == tpNumber)
        {
            return false; // TP number is not unique
        }
    }
    return true; // TP number is unique
}

void registerUser(User *users, int *numUsers)
{
    if (*numUsers >= MAX_USERS)
    {
        printf("\nUSER CAPACITY REACHED, NO MORE SIGN-UPS ALLOWED\n");
        return;
    }
    User newUser;
    printf("Enter your name: ");
    scanf("%s", newUser.name);

    int tpNumber;
    int tryCount = 0;
    do
    {
        if (tryCount > 0)
        {
            printf("\n\t\t~~~> INVALID TP NUMBER OR NOT 6 DIGITS. PLEASE TRY AGAIN <~~~\n\n");
            if (tryCount >= 3)
            {
                printf("\n\t\t~~~>    TOO MANY UNSUCCESSFUL ATTEMPTS. EXITING PROGRAM <~~~\n\n");
                return;
            }
        }
        printf("ENTER YOUR TP NUMBER (6 DIGITS ): TP");
        scanf("%6d", &tpNumber);
        tryCount++;
    } while (!uniquenessOftpNumber(users, numUsers, tpNumber) || tpNumber < 100000 || tpNumber > 999999);
    newUser.tpNumber = tpNumber;

    char password[80];
    int tryCount2 = 0;
    do
    {
        if (tryCount2 > 0)
        {
            printf("\n\t\t~~~> PASSWORD SHOULD BE AT LEAST 8 CHARACTERS AND NOT MORE THAN 16 CHARACTERS. PLEASE TRY AGAIN <~~~\n\n");
            if (tryCount2 >= 3)
            {
                printf("\n\t\t~~~>          TOO MANY UNSUCCESSFUL ATTEMPTS. EXITING PROGRAM <~~~\n\n");
                return;
            }
        }

        printf("ENTER A PASSWORD (MINIMUM 8 CHARACTERS, MAXIMUM 16 CHARACTERS): ");
        scanf("%s", password);
        tryCount2++;
    } while (strlen(password) < 8 || strlen(password) > 16);
    strcpy(newUser.password, password);

    users[*numUsers] = newUser;
    (*numUsers)++;
    printf("\n\t\t~~~> SIGN-UP SUCCESSFUL <~~~\n\n");
    saveUsersToFile(users, numUsers);
}

bool signINuser(User *users, int *numUsers, int *checkTP)
{
    int tpNumber;
    char password[80];

    printf("Enter your TP number: TP");
    scanf("%d", &tpNumber);
    printf("Enter your password: ");
    scanf("%s", password);

    for (int i = 0; i < *numUsers; i++)
    {
        if (users[i].tpNumber == tpNumber && strcmp(users[i].password, password) == 0)
        {
            printf("\n\t\t~~~> SIGN-IN SUCCESSFUL <~~~ \n");
            *checkTP = tpNumber;
            return true; // User allowed
        }
    }
    printf("\n\t\t~~~> INVALID TP NUMBER OR PASSWORD, PLEASE TRY AGAIN <~~~\n\n");
    return false; // User not allowed
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void saveSlotsToFile(int (*availableSlots)[12])
{
    FILE *fileBlocka = fopen("C:\\Users\\Abood\\Desktop\\year1,S2\\C assignment\\DISCUSSION ROOM RESERVATION SYSTEM\\slotsTrackingBA.txt", "w");
    if (fileBlocka == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            fprintf(fileBlocka, "%d ", availableSlots[i][j]);
        }
        fputs("\n", fileBlocka);
    }
    fclose(fileBlocka);
}

void loadSlotsFromFile(int (*availableSlots)[12])
{
    FILE *fileBlocka = fopen("C:\\Users\\Abood\\Desktop\\year1,S2\\C assignment\\DISCUSSION ROOM RESERVATION SYSTEM\\slotsTrackingBA.txt", "r");
    if (fileBlocka == NULL)
    {
        printf("Error opening file(s) for reading.\n");
        return;
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            fscanf(fileBlocka, "%d", &availableSlots[i][j]);
        }
    }
    fclose(fileBlocka);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void selectBlockandRoom(int *numRooms, int *minRoomNumber, char *blockName)
{
    int blockChoice;
    for (;;)
    {
        printf("\n\t\t~~~> SELECT A BLOCK: <~~~\n\n");
        printf("1. Block A (Library)\n");
        printf("Enter your choice: ");
        scanf("%d", &blockChoice);
        if (blockChoice != 1)
        {
            printf("\n\t\t~~~> TRY AGAIN PLEASE <~~~\n\n");
            continue;
        }
        break;
    }

    int roomTypeSelection;
    // Set the number of rooms and the minimum room number based on the block choice
    for (;;)
    {
        printf("\n\t~~~> CHOSE THE TYPE OF ROOM YOU WOULD LIKE TO BOOK: <~~~\n\n");
        printf("1. Small Room\n2. Big Room\n3. Rooms details\n4. Go to previous page\n");     
        printf("Enter your choice: ");
        scanf("%d", &roomTypeSelection);

        while (roomTypeSelection < 1 && roomTypeSelection > 4)
        {
            printf("\n\t~~~> CHOSE THE TYPE OF ROOM YOU WOULD LIKE TO BOOK: <~~~\n\n");
            printf("1. Small Room\n2. Big Room\n3. Rooms details\n4. Go to previous page\n");
            printf("Enter your choice: ");
            scanf("%d", &roomTypeSelection);
        }
        switch (roomTypeSelection)
        {
        case 1:
            *numRooms = 8;
            *minRoomNumber = 1;
            strcpy(blockName, "Block A (Library, Small Room)");
            break;
        case 2:
            *numRooms = 8;
            *minRoomNumber = 1;
            strcpy(blockName, "Block A (Library, Big Room)");
            break;
        case 3:
            for (;;)
            {
                int choice;
                printf("\n\t         ~~~> CHOSE THE TYPE OF ROOM: <~~~\n\n");
                printf("1. Small Room\n2. Big Room\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if (choice == 1)
                {
                    printf("\n\nSmall Room Inclouds:\n\n---[Monitor]------[5 Chairs]------[HDMI cable]\n");
                    break;
                }
                else if (choice == 2)
                {
                    printf("\n\nBig Room Inclouds:\n\n---[Monitor]------[10 Chairs]------[HDMI cable]------[Speakers]------[Computer]\n");
                    break;
                }
                else
                    continue;
            }
        case 4:
            selectBlockandRoom(numRooms, minRoomNumber, blockName);
            break;
        }
        break;
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void viewAvailableSlots(Booking *bookings, int *numBookings, int (*availableSlots)[12], User *users, int *numUsers, int *checkTP)
{
    printf("\n\t      ~~~> VIEW AVAILABLE SLOTS <~~~\n\n");
    int numRooms;      // number of rooms in a selected block
    int minRoomNumber; // the minimum number of rooms 
    char blockName[30];
    selectBlockandRoom(&numRooms, &minRoomNumber, blockName); 

    // Initialize availableSlots array with zeroes
    for (int i = 0; i < numRooms; i++) // loop inside the array for the number of rooms as rows
    {
        for (int j = 0; j < 12; j++) // the inner loop for the columns represents the slots hours
        {
            availableSlots[i][j] = 0; // so, for each room will be 12 hour (and this will set all values in the array as 0 which represent that all slots are available)
        }
    }
    loadSlotsFromFile(availableSlots);
    saveSlotsToFile(availableSlots);

    printf("\n\t          ~~~> AVAILABLE TIME SLOTS FOR ROOMS IN %s: <~~~\n\n", blockName);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("| Time   |   8:00  |   9:00  |  10:00  |  11:00  |  12:00  |  13:00  |  14:00  |  15:00  |  16:00  |  17:00  |  18:00  |  19:00  |\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    for (int i = 0; i < numRooms; i++)
    {
        if (i == 4)
        {
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printf("                                                       Big Rooms                                                                  \n");
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        }
        printf("[ ROOM %d |", minRoomNumber + i);
        for (int j = 0; j < 12; j++)
        {
            if (availableSlots[i][j] == 0)
            {
                printf("   { }   |");
            }
            else
            {
                printf("   {X}   |");
            }
        }
        printf("\n");
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    for (;;)
    {
        printf("\n\t\t~~~> SELECT AN ACTION: <~~~\n\n");
        printf("1. Book a slot\n");
        printf("2. Go back to the previous page\n");
        printf("3. Return to the main menu\n");
        printf("Enter your choice: ");
        int actionChoice;
        scanf("%d", &actionChoice);

        switch (actionChoice)
        {
        case 1:
            bookNewSlot(bookings, numBookings, availableSlots, users, numUsers, checkTP);
            break;
        case 2:
            // Go back to the previous page
            viewAvailableSlots(bookings, numBookings, availableSlots, users, numUsers, checkTP);
            break;
        case 3:
            // go back to the main menu
            break;
        default:
            printf("\n\t~~~> INVALID CHOICE, TRY AGAIN <~~~ \n\n");
            continue;
        }
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void bookNewSlot(Booking *bookings, int *numBookings, int (*availableSlots)[12], User *users, int *numUsers, int *checkTP)
{
    if (*numBookings >= MAX_BOOKINGS)
    {
        printf("\nBOOKING CAPACITY REACHED. NO MORE BOOKINGS ALLOWED\n");
        return;
    }
    Booking newBooking; // a structure variable that will store the booking details from the user (name,tp,room,timeslot)
    // check if the tp number is the same for the entered user
    for (;;)
    {
        printf("\n\t\t~~~> WRITE YOUR DETAILS: <~~~\n\n");
        printf("Enter your name: ");
        scanf("%s", newBooking.name);
        printf("Enter your TP number: TP");
        scanf("%d", &newBooking.id);

        if (newBooking.id != *checkTP)
        {
            printf("\nTHE TP NUMBER YOU HAVE ENTERED IS NOT CORRECT, IT SHOULD MATCH THE SIGIN-IN TP NUMBER\n");
            continue;
        }
        else
            break;
    }

    int numRooms;
    int minRoomNumber;
    char blockName[30];
    selectBlockandRoom(&numRooms, &minRoomNumber, blockName);

    // Initialize availableSlots array with zeroes
    for (int i = 0; i < numRooms; i++) 
    {
        for (int j = 0; j < 12; j++)
        {
            availableSlots[i][j] = 0; 
        }
    }
    loadSlotsFromFile(availableSlots);
    saveSlotsToFile(availableSlots);

    for (;;) // Prompt the user to choose a room within the selected block
    {
        printf("\n\t   ~~~> AVAILABLE ROOMS IN [%s]: <~~~\n\n", blockName);
        for (int room = minRoomNumber; room < minRoomNumber + numRooms; room++)
        {
            printf("\t\t| Room %d |\n", room);
        }
        printf("Enter the room number: ");
        scanf("%d", &newBooking.room);

        // Validate room number
        if (newBooking.room < minRoomNumber || newBooking.room >= minRoomNumber + numRooms)
        {
            printf("\nINVALID ROOM NUMBER. PLEASE TRY AGAIN\n");
            continue;
        }
        break;
    }

    int startHour, numHours, endHour, startSlot, endSlot;
    while (1)
    {
        printf("\n\t          ~~~> AVAILABLE TIME SLOTS FOR ROOM %d: <~~~\n\n", newBooking.room);
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("| Time   |   8:00  |   9:00  |  10:00  |  11:00  |  12:00  |  13:00  |  14:00  |  15:00  |  16:00  |  17:00  |  18:00  |  19:00  |\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("[ ROOM %d |", newBooking.room);
        for (int j = 0; j < 12; j++)
        {
            if (availableSlots[newBooking.room - minRoomNumber][j] == 0)
            {
                printf("   { }   |");
            }
            else
            {
                printf("   {X}   |");
            }
        }
        printf("\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("\nEnter the starting hour (8 to 19): ");
        scanf("%d", &startHour);
        printf("\nEnter the number of continuous hours you want to book (up to 3 hours): ");
        scanf("%d", &numHours);

        // Validate start hour and number of hours
        if ((startHour < 8 || startHour > 19) || (numHours < 1 || numHours > 3))
        {
            printf("\nINVALID HOUR CHOICE. MAXIMUM HOURS YOU CAN BOOK IS 3 HOURS, PLEASE TRY AGAIN\n\n");
            continue;
        }
        endHour = startHour + numHours - 1;
        // implement the real time 
        time_t currentTime = time(NULL);
        struct tm *localTime = localtime(&currentTime);
        // check if the starting hour less that the current real time, if yes will not accept the time
        if (startHour < localTime->tm_hour)
        {
            printf("\nTHE CHOSEN HOUR HAS PASSED, PLEASE CHOOSE ANOTHER HOUR\n\n");
            continue;
        }

        // Calculate the slot indices (startSlot and endSlot)
        startSlot = startHour - 8;
        endSlot = startSlot + numHours - 1;

        // Check if the selected slots are available
        bool slotsAvailable = true;
        for (int slot = startSlot; slot <= endSlot; slot++)
        {
            if (availableSlots[newBooking.room - minRoomNumber][slot] != 0)
            {
                slotsAvailable = false;
                break;
            }
        }
        // If slots are available, proceed with booking
        if (slotsAvailable)
        {
            // Update the availableSlots array to mark the slots as booked
            for (int slot = startSlot; slot <= endSlot; slot++)
            {
                availableSlots[newBooking.room - minRoomNumber][slot] = 1;
            }
            printf("\n\t\t~~~> BOOKING SUCCESSFUL <~~~\n\n");
            saveSlotsToFile(availableSlots);
        }
        else
        {
            printf("\n\t\t~~~> THE SELECTED TIME SLOTS ARE NOT AVAILABLE. PLEASE CHOOSE ANOTHER TIME <~~~\n\n");
            int ask;
            printf("\nDO YOU WANT TO:\n(1) Try again\n(2) Go to main menu\n");
            printf("Enter your choice:");
            scanf("%d", &ask);
            switch (ask)
            {
            case 1:
                continue;
                break;
            case 2:
                return;
                break;
            default:
                break;
            }
        }
        break;
    }

    newBooking.timeslot = startHour;
    newBooking.endTimeslot = endHour;
    strcpy(bookings[*numBookings].name, newBooking.name);
    bookings[*numBookings].id = newBooking.id;
    bookings[*numBookings].room = newBooking.room;
    bookings[*numBookings].timeslot = newBooking.timeslot;
    bookings[*numBookings].endTimeslot = newBooking.endTimeslot;
    (*numBookings)++;
    saveBookingsToFile(bookings, numBookings);

    printf("\n\tPRESS (ENTER) KEY TO RETURN TO THE MAIN MENU:\n");
    getchar();
    getchar();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void viewExistingBookings(Booking *bookings, int *numBookings, int *checkTP)
{
    int tpNumber;
    int foundBooking = 0; // Flag to check if a booking is found
    do
    {
        printf("\nEnter your TP number:");
        scanf("%d", &tpNumber);
        if (tpNumber != *checkTP)
        {
            printf("\n\t~~~> THE TP NUMBER YOU HAVE ENTERED IS NOT CORRECT, IT SHOULD MATCH THE SIGN-IN TP NUMBER\n\n");
            continue;
        }
        printf("\n\t\t~~~> YOUR BOOKINGS ARE <~~~\n\n");
        for (int i = 0; i < *numBookings; i++)
        {
            if (tpNumber == bookings[i].id)
            {
                foundBooking = 1; // booking is found
                printf("------------------------------\n");
                printf("\tBooking Number %d:\n\n", i + 1);
                printf("Name: %s\n", bookings[i].name);
                printf("TP Number: %d\n", bookings[i].id);
                printf("Room: %d\n", bookings[i].room);
                printf("Starting Time Slot: %d\n", bookings[i].timeslot);
                printf("Ending Time Slot: %d\n", bookings[i].endTimeslot);
                printf("------------------------------\n\n");
            }
        }
        break;
    } while (1);

    if (!foundBooking) // no bookings found 
    {
        printf("\n\t\t~~~> THERE IS NO EXISTING BOOKING FOR YOU <~~~\n\n");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void editExistingBooking(Booking *bookings, int *numBookings, int *checkTP, int (*availableSlots)[12])
{
    viewExistingBookings(bookings, numBookings, checkTP); //calling the function to let the user see his bookings
    int room, startTime, duration;
    printf("Enter the room number of the booking you want to edit: ");
    scanf("%d", &room);
    printf("\nEnter the starting hour of the booking you want to edit: ");
    scanf("%d", &startTime);
    printf("\nEnter the ending time slot of the booking you want to edit: ");
    scanf("%d", &duration);
    // validate the existence of bookings
    int bookingIndex = -1;
    for (int i = 0; i < *numBookings; i++)
    {
        if (room == bookings[i].room && startTime == bookings[i].timeslot && duration == bookings[i].endTimeslot && *checkTP == bookings[i].id)
        {
            bookingIndex = i;
            break;
        }
    }
    if (bookingIndex == -1)
    {
        printf("\n\t\t~~~> BOOKING NOT FOUND OR YOU ARE NOT AUTHORIZED TO EDIT THIS BOOKING <~~~\n\n");
        return;
    }
    // Check if the new slots are available
    int newRoom, newStartTime, newDuration;
    for (;;)
    {
        printf("\n\t    <====> View Available Slots <====>\n\n");

        int numRooms;      
        int minRoomNumber; 
        char blockName[30];
        selectBlockandRoom(&numRooms, &minRoomNumber, blockName); 

        // Initialize availableSlots array with zeroes
        for (int i = 0; i < numRooms; i++) 
        {
            for (int j = 0; j < 12; j++) 
            {
                availableSlots[i][j] = 0; 
            }
        }
        loadSlotsFromFile(availableSlots);
        saveSlotsToFile(availableSlots);

        printf("\n\t~~~> Available time slots for Rooms in %s: <~~~\n\n", blockName);
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("| Time   |   8:00  |   9:00  |  10:00  |  11:00  |  12:00  |  13:00  |  14:00  |  15:00  |  16:00  |  17:00  |  18:00  |  19:00  |\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        for (int i = 0; i < numRooms; i++)
        {
            if (i == 4)
            {
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                printf("                                                   Big Rooms                                                                      \n");
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            }
            printf("[ ROOM %d |", minRoomNumber + i);
            for (int j = 0; j < 12; j++)
            {
                if (availableSlots[i][j] == 0)
                {
                    printf("   { }   |");
                }
                else
                {
                    printf("   {X}   |");
                }
            }
            printf("\n");
        }
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

        printf("Enter the new room number: ");
        scanf("%d", &newRoom);
        printf("Enter the new starting hour (8 to 18): ");
        scanf("%d", &newStartTime);
        printf("Enter the new duration (up to 3 hours): ");
        scanf("%d", &newDuration);
        // validate the time and duration
        if ((newStartTime < 8 || newStartTime > 19) || (newDuration < 1 || newDuration > 3))
        {
            printf("\nINVALID HOUR CHOICE OR DURATION. PLEASE TRY AGAIN\n\n");
            continue;
        }
        // arrange indexes
        int newEndSlot = newStartTime + newDuration - 1;
        int oldStartSlot = startTime - 8;
        int oldEndSlot = startTime + duration - 1;

        // Check if the new slots are available
        bool slotsAvailable = true;
        for (int slot = newStartTime - 8; slot <= newEndSlot - 8; slot++)
        {
            if (availableSlots[newRoom - minRoomNumber][slot] != 0)
            {
                slotsAvailable = false;
                break;
            }
        }

        // If slots are available, proceed with the booking update
        if (slotsAvailable)
        {
            // Update the availableSlots array to mark the old slots as available and the new slots as booked
            for (int slot = oldStartSlot; slot <= oldEndSlot; slot++)
            {
                availableSlots[room - minRoomNumber][slot] = 0;
            }
            for (int slot = newStartTime - 8; slot <= newEndSlot - 8; slot++)
            {
                availableSlots[newRoom - minRoomNumber][slot] = 1;
            }

            // Update the booking details in the bookings array
            bookings[bookingIndex].room = newRoom;
            bookings[bookingIndex].timeslot = newStartTime;
            bookings[bookingIndex].endTimeslot = newEndSlot;

            printf("\n\t\t~~~> BOOKING SUCCESSFULLY UPDATED <~~~\n\n");
            saveBookingsToFile(bookings, numBookings);
            saveSlotsToFile(availableSlots);
        }
        return;
    }
}

// ////////////////////////////////////////////////////////////////////////////////////////////////

void cancelExistingBooking(Booking *bookings, int *numBookings, int *checkTP, int (*availableSlots)[12])
{
    viewExistingBookings(bookings, numBookings, checkTP);
    printf("\n\t\t~~~> CANCEL EXISTING BOOKING <~~~\n\n");

    int room, startTime, duration;
    printf("Enter the room number of the booking you want to cancel: ");
    scanf("%d", &room);
    printf("Enter the starting hour of the booking you want to cancel: ");
    scanf("%d", &startTime);
    printf("Enter the ending Time Slot hour of the booking you want to cancel: ");
    scanf("%d", &duration);

    int bookingIndex = -1;
    for (int i = 0; i < *numBookings; i++)
    {
        if (room == bookings[i].room && startTime == bookings[i].timeslot && duration == bookings[i].endTimeslot)
        {
            bookingIndex = i;
            break;
        }
    }
    if (bookingIndex == -1)
    {
        printf("\n\t\t~~~> BOOKING NOT FOUND OR YOU ARE NOT AUTHORIZED TO CANCEL THIS BOOKING <~~~\n\n");
        return;
    }
    printf("\n\t\t~~~> CONFIRM BOOKING CANCELLATION <~~~\n\n");
    printf("Booking details:\n");
    printf("Room Number: %d\n", bookings[bookingIndex].room);
    printf("Start Time: %d:00\n", bookings[bookingIndex].timeslot);
    printf("Duration: %d hour(s)\n", bookings[bookingIndex].endTimeslot - bookings[bookingIndex].timeslot + 1);

    char confirmation;
    printf("\nAre you sure you want to cancel this booking? (Y/N): ");
    scanf(" %c", &confirmation);

    if (confirmation == 'Y' || confirmation == 'y')
    {
        for (int i = bookingIndex; i < *numBookings - 1; i++)
        {
            bookings[i] = bookings[i + 1];
        }
        (*numBookings)--;
        for (int i = startTime - 8; i <= bookings[bookingIndex].endTimeslot - 8; i++)
        {
            availableSlots[room - 1][i] = 0;
        }
          
        printf("\n\t\t~~~> BOOKING SUCCESSFULLY CANCELED! <~~~\n\n");
        saveSlotsToFile(availableSlots);
        saveBookingsToFile(bookings, numBookings);
    }
    else
    {
        printf("\n\t\t~~~> BOOKING CANCELLATION ABORTED <~~~\n\n");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void saveBookingsToFile(Booking *bookings, int *numBookings)
{
    FILE *file = fopen("C:\\Users\\Abood\\Desktop\\year1,S2\\C assignment\\DISCUSSION ROOM RESERVATION SYSTEM\\bookings.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < *numBookings; i++)
    {
        fprintf(file, "Name: %s\n", bookings[i].name); // fprintf(), allow us to write in the file and use some formats that we can use them to pront somthing
        fprintf(file, "TPNumber: %d\n", bookings[i].id);
        fprintf(file, "Room: %d\n", bookings[i].room);
        fprintf(file, "Starting Time Slot: %d\n", bookings[i].timeslot);
        fprintf(file, "Ending Time Slot: %d\n", bookings[i].endTimeslot);
        fputs("\n", file); // fputs(), this function will allow us to write only a string inside the file without any formating
    }

    fclose(file);
}

void loadBookingsFromFile(Booking *bookings, int *numBookings)
{
    FILE *file = fopen("C:\\Users\\Abood\\Desktop\\year1,S2\\C assignment\\DISCUSSION ROOM RESERVATION SYSTEM\\bookings.txt", "r+");
    if (file == NULL)
    {
        printf("No existing bookings found.\n");
        return;
    }
    char line[1100];
    *numBookings = 0;
    while (*numBookings < MAX_BOOKINGS && fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "Name:", 5) == 0)
        {
            sscanf(line, "Name: %[^\n]", bookings[*numBookings].name);
            fgets(line, sizeof(line), file);
            sscanf(line, "TPNumber: %d", &bookings[*numBookings].id);
            fgets(line, sizeof(line), file);
            sscanf(line, "Room: %d", &bookings[*numBookings].room);
            fgets(line, sizeof(line), file);
            sscanf(line, "Starting Time Slot: %d", &bookings[*numBookings].timeslot);
            fgets(line, sizeof(line), file);
            sscanf(line, "Duration: %d", &bookings[*numBookings].endTimeslot);

            (*numBookings)++;
        }
    }

    fclose(file);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void saveUsersToFile(User *users, int *numUsers)
{
    FILE *savingUser = fopen("C:\\Users\\Abood\\Desktop\\year1,S2\\C assignment\\DISCUSSION ROOM RESERVATION SYSTEM\\userRegistration.txt", "w");
    if (savingUser == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < *numUsers; i++)
    {
        fprintf(savingUser, "Name: %s\n", users[i].name);
        fprintf(savingUser, "TPNumber: %d\n", users[i].tpNumber);
        fprintf(savingUser, "Password: %s\n", users[i].password);
        fputs("\n", savingUser);
    }
    fclose(savingUser);
}

void loadUsersFromFile(User *users, int *numUsers)
{
    FILE *loadingUser = fopen("C:\\Users\\Abood\\Desktop\\year1,S2\\C assignment\\DISCUSSION ROOM RESERVATION SYSTEM\\userRegistration.txt", "r");
    if (loadingUser == NULL)
    {
        printf("No existing user registrations found.\n");
        return;
    }
    *numUsers = 0;
    char line[1000];

    while (fgets(line, sizeof(line), loadingUser) != NULL) // read all lines in the file until it reach the end of the file which is NULL means no value there
    {
        if (strncmp(line, "Name:", 5) == 0) // this will specify the starting point of reading, so when the program reads a line starting with Name it will count that is a new user
        {
            sscanf(line, "Name: %[^\n]", users[*numUsers].name);
            fgets(line, sizeof(line), loadingUser);
            sscanf(line, "TPNumber: %d", &users[*numUsers].tpNumber);
            fgets(line, sizeof(line), loadingUser);
            sscanf(line, "Password: %[^\n]", users[*numUsers].password);

            (*numUsers)++;
        }
    }
    fclose(loadingUser);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
    printf("Invalid choice. Please try again.\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    Booking bookings[MAX_BOOKINGS]; // bookings structure with a maximum 100 booking, using the same format of Booking sturucture
    int numBookings = 0;            
    int availableSlots[8][12];      

    User users[MAX_USERS]; // users structure with the same format of User stucture and maximum 100 user
    int numUsers = 0;
    int checkTP;
    // loading the information from the files 
    loadBookingsFromFile(bookings, &numBookings); 
    loadUsersFromFile(users, &numUsers);

    int choice;
    int chose;
    do
    {
        while (1) // infinity loop till the some condition to be happened 
        {
            printf("\n\n\t  ~~~~> WELCOME TO APU DISCUSSION ROOM RESERVATION SYSTEM <~~~~\n\n");
            displayRegistration();
            scanf("%d", &choice);
            if (choice > 3 || choice < 1)
            {
                printf("\nINVALID ACTION CHOICE, PLEASE TRY AGAIN\n");
                break;
            }
            else if (choice == 3)
            {
                printf("\n\n\t  ~~~~> THANKS FOR USING APU LIBRARY ROOM RESERVATION SYSTEM, HAVE A NICE DAY <~~~~\n\n");
                break;
            }

            break;
        }
        switch (choice)
        {
        case 1:
            registerUser(users, &numUsers); 
            break;
        case 2:
            if (signINuser(users, &numUsers, &checkTP))
            {
                do // infinity loop that breaks when the condition = to 4
                {  
                    displayMenu(); // displaying the main functionality of the program 
                    scanf("%d", &chose);
                    switch (chose)
                    {
                    case 1:
                        viewAvailableSlots(bookings, &numBookings, availableSlots, users, &numUsers, &checkTP);
                        break;
                    case 2:
                        bookNewSlot(bookings, &numBookings, availableSlots, users, &numUsers, &checkTP);
                        break;
                    case 3:
                        bookingManagement(bookings, &numBookings, &checkTP, availableSlots);
                        break;
                    case 4:
                        saveBookingsToFile(bookings, &numBookings);
                        printf("\n");
                        printf("***************************************\n");
                        printf("*         Exiting program...          *\n");
                        printf("***************************************\n");
                        break;
                    default:
                        clearInputBuffer();
                        break;
                    }
                } while (chose != 4);
            }
        }
    } while (choice != 3);

    return 0;
}
