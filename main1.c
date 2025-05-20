#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI Color Definitions
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define MAX_BOOKINGS 300
#define MAX_USERS 50
#define ADMIN_USER "joshi"
#define ADMIN_PASS "1234"

struct moviedetails {
    char name[50];
    unsigned int phone;
    int seat;
    int id;
    char movie[50];
} bookings[MAX_BOOKINGS];

struct user {
    char username[30];
    char password[20];
} users[MAX_USERS];

int bookingCount = 0;
int bookingID = 1000;
int userCount = 0;
int seat[3][101] = {0};
char *movies[] = {"ONE PIECE RED", "ONE PIECE GOLD", "ONE PIECE Z"};

void mainMenu();
void adminPanel();
void userMenu(const char* username);
int loginAdmin();
int loginUser();
void registerUser();
void viewBookings();
void viewSeats();
void bookTicket(const char* username);
void cancelTicket();
void generateTicket(struct moviedetails b, int price);
void refundMessage();

int main() {
    mainMenu();
    return 0;
}

void mainMenu() {
    int choice;
    while (1) {
        printf(CYAN "\n==================================================\n" RESET);
        printf(MAGENTA "            MOVIE TICKET BOOKING SYSTEM           \n" RESET);
        printf(CYAN "==================================================\n" RESET);
        printf(YELLOW "1. Admin Panel\n");
        printf("2. Register (New User)\n");
        printf("3. Sign In (Existing User)\n");
        printf("4. Exit\n" RESET);
        printf(CYAN "--------------------------------------------------\n" RESET);
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: adminPanel(); break;
            case 2: registerUser(); break;
            case 3: 
                if (loginUser()) {
                    char username[30];
                    printf("Enter your username again to proceed: ");
                    scanf("%s", username);
                    userMenu(username);
                }
                break;
            case 4: exit(0);
            default: printf(RED "Invalid choice. Try again!\n" RESET);
        }
    }
}

void adminPanel() {
    if (!loginAdmin()) return;
    int choice;
    while (1) {
        printf(BLUE "\n================= ADMIN PANEL ====================\n" RESET);
        printf("1. View All Bookings\n");
        printf("2. View Seats\n");
        printf("3. Back to Main Menu\n");
        printf(BLUE "--------------------------------------------------\n" RESET);
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: viewBookings(); break;
            case 2: viewSeats(); break;
            case 3: return;
            default: printf(RED "Invalid choice.\n" RESET);
        }
    }
}

void registerUser() {
    if (userCount >= MAX_USERS) {
        printf(RED "User limit reached. Cannot register more users.\n" RESET);
        return;
    }
    struct user newUser;
    printf(GREEN "\n------------ User Registration ------------\n" RESET);
    printf("Enter a username: ");
    scanf("%s", newUser.username);
    printf("Enter a password: ");
    scanf("%s", newUser.password);

    users[userCount++] = newUser;
    printf(GREEN "Registration successful! Welcome, %s!\n" RESET, newUser.username);
    userMenu(newUser.username);
}

int loginUser() {
    char name[30], pass[20];
    printf(CYAN "\n--------------- User Login -----------------\n" RESET);
    printf("Enter Username: ");
    scanf("%s", name);
    printf("Enter Password: ");
    scanf("%s", pass);
    for (int i = 0; i < userCount; i++) {
        if (strcmp(name, users[i].username) == 0 && strcmp(pass, users[i].password) == 0) {
            printf(GREEN "Welcome, %s!\n" RESET, name);
            return 1;
        }
    }
    printf(RED "Invalid user credentials.\n" RESET);
    return 0;
}

int loginAdmin() {
    char name[30], pass[20];
    printf(CYAN "\n--------------- Admin Login ----------------\n" RESET);
    printf("Enter Admin Username: ");
    scanf("%s", name);
    printf("Enter Admin Password: ");
    scanf("%s", pass);
    if (strcmp(name, ADMIN_USER) == 0 && strcmp(pass, ADMIN_PASS) == 0) {
        printf(GREEN "Admin login successful.\n" RESET);
        return 1;
    }
    printf(RED "Invalid admin credentials.\n" RESET);
    return 0;
}

void userMenu(const char* username) {
    int choice;
    while (1) {
        printf(CYAN "\n================== USER PANEL ====================\n" RESET);
        printf("1. Book Ticket\n");
        printf("2. Cancel Ticket\n");
        printf("3. View Seats\n");
        printf("4. Logout\n");
        printf(CYAN "--------------------------------------------------\n" RESET);
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: bookTicket(username); break;
            case 2: cancelTicket(); break;
            case 3: viewSeats(); break;
            case 4: return;
            default: printf(RED "Invalid choice.\n" RESET);
        }
    }
}

void viewBookings() {
    printf(YELLOW "\n================= ALL BOOKINGS ===================\n" RESET);
    if (bookingCount == 0) {
        printf(RED "No bookings found.\n" RESET);
        return;
    }
    for (int i = 0; i < bookingCount; i++) {
        printf("--------------------------------------------------\n");
        printf("Booking ID : %d\n", bookings[i].id);
        printf("Movie      : %s\n", bookings[i].movie);
        printf("Name       : %s\n", bookings[i].name);
        printf("Phone      : %u\n", bookings[i].phone);
        printf("Seat No.   : %d\n", bookings[i].seat);
    }
    printf("--------------------------------------------------\n");
}

void viewSeats() {
    for (int i = 0; i < 3; i++) {
        printf("\n" MAGENTA "Movie: %s\n" RESET, movies[i]);
        for (int j = 1; j <= 100; j++) {
            if (seat[i][j])
                printf(RED " X " RESET);
            else
                printf(GREEN "%2d " RESET, j);
            if (j % 10 == 0) printf("\n");
        }
    }
}

void bookTicket(const char* username) {
    int movieIndex, s;
    printf(CYAN "\n----------------- Movie Selection ----------------\n" RESET);
    for (int i = 0; i < 3; i++) {
        printf("%d. %s\n", i + 1, movies[i]);
    }
    printf(CYAN "--------------------------------------------------\n" RESET);
    printf("Choice: ");
    scanf("%d", &movieIndex);
    if (movieIndex < 1 || movieIndex > 3) {
        printf(RED "Invalid movie selection.\n" RESET);
        return;
    }
    printf("\nAvailable Seats for %s:\n", movies[movieIndex - 1]);
    for (int i = 1; i <= 100; i++) {
        if (seat[movieIndex - 1][i])
            printf(RED " X " RESET);
        else
            printf(GREEN "%2d " RESET, i);
        if (i % 10 == 0) printf("\n");
    }
    printf("\nEnter seat number to book: ");
    scanf("%d", &s);
    if (s < 1 || s > 100 || seat[movieIndex - 1][s]) {
        printf(RED "Seat not available.\n" RESET);
        return;
    }
    struct moviedetails b;
    strcpy(b.name, username);
    printf("Enter phone number: ");
    scanf("%u", &b.phone);
    b.seat = s;
    b.id = bookingID++;
    strcpy(b.movie, movies[movieIndex - 1]);
    bookings[bookingCount++] = b;
    seat[movieIndex - 1][s] = 1;
    generateTicket(b, 500);
}

void cancelTicket() {
    int id;
    printf("\nEnter Booking ID to cancel: ");
    scanf("%d", &id);
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].id == id) {
            printf(YELLOW "Booking for %s (Seat %d in %s) cancelled.\n" RESET,
                   bookings[i].name, bookings[i].seat, bookings[i].movie);
            for (int j = 0; j < 3; j++) {
                if (strcmp(bookings[i].movie, movies[j]) == 0) {
                    seat[j][bookings[i].seat] = 0;
                    break;
                }
            }
            for (int j = i; j < bookingCount - 1; j++)
                bookings[j] = bookings[j + 1];
            bookingCount--;
            refundMessage();
            return;
        }
    }
    printf(RED "Booking ID not found.\n" RESET);
}

void refundMessage() {
    printf(YELLOW "Refund process initiated. Amount will be credited back within 3-5 business days.\n" RESET);
}

void generateTicket(struct moviedetails b, int price) {
    printf(GREEN "\n==================================================\n" RESET);
    printf(GREEN "                 MOVIE TICKET                     \n" RESET);
    printf(GREEN "==================================================\n" RESET);
    printf("Booking ID : %d\n", b.id);
    printf("Movie      : %s\n", b.movie);
    printf("Name       : %s\n", b.name);
    printf("Seat No.   : %d\n", b.seat);
    printf("Phone No.  : %u\n", b.phone);
    printf("Price      : Rs. %d\n", price);
    printf(GREEN "==================================================\n" RESET);
}
