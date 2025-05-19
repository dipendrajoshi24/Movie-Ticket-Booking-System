#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        printf("\n==================================================\n");
        printf("            MOVIE TICKET BOOKING SYSTEM           \n");
        printf("==================================================\n");
        printf("1. Admin Panel\n");
        printf("2. Register (New User)\n");
        printf("3. Sign In (Existing User)\n");
        printf("4. Exit\n");
        printf("--------------------------------------------------\n");
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
            default: printf("Invalid choice. Try again!\n");
        }
    }
}

void adminPanel() {
    if (!loginAdmin()) return;
    int choice;
    while (1) {
        printf("\n================= ADMIN PANEL ====================\n");
        printf("1. View All Bookings\n");
        printf("2. View Seats\n");
        printf("3. Back to Main Menu\n");
        printf("--------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: viewBookings(); break;
            case 2: viewSeats(); break;
            case 3: return;
            default: printf("Invalid choice.\n");
        }
    }
}

void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached. Cannot register more users.\n");
        return;
    }
    struct user newUser;
    printf("\n------------ User Registration ------------\n");
    printf("Enter a username: ");
    scanf("%s", newUser.username);
    printf("Enter a password: ");
    scanf("%s", newUser.password);

    users[userCount++] = newUser;
    printf("Registration successful! Welcome, %s!\n", newUser.username);
    userMenu(newUser.username);
}

int loginUser() {
    char name[30], pass[20];
    printf("\n--------------- User Login -----------------\n");
    printf("Enter Username: ");
    scanf("%s", name);
    printf("Enter Password: ");
    scanf("%s", pass);
    for (int i = 0; i < userCount; i++) {
        if (strcmp(name, users[i].username) == 0 && strcmp(pass, users[i].password) == 0) {
            printf("Welcome, %s!\n", name);
            return 1;
        }
    }
    printf("Invalid user credentials.\n");
    return 0;
}

int loginAdmin() {
    char name[30], pass[20];
    printf("\n--------------- Admin Login ----------------\n");
    printf("Enter Admin Username: ");
    scanf("%s", name);
    printf("Enter Admin Password: ");
    scanf("%s", pass);
    if (strcmp(name, ADMIN_USER) == 0 && strcmp(pass, ADMIN_PASS) == 0) {
        printf("Admin login successful.\n");
        return 1;
    }
    printf("Invalid admin credentials.\n");
    return 0;
}

void userMenu(const char* username) {
    int choice;
    while (1) {
        printf("\n================== USER PANEL ====================\n");
        printf("1. Book Ticket\n");
        printf("2. Cancel Ticket\n");
        printf("3. View Seats\n");
        printf("4. Logout\n");
        printf("--------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: bookTicket(username); break;
            case 2: cancelTicket(); break;
            case 3: viewSeats(); break;
            case 4: return;
            default: printf("Invalid choice.\n");
        }
    }
}

void viewBookings() {
    printf("\n================= ALL BOOKINGS ===================\n");
    if (bookingCount == 0) {
        printf("No bookings found.\n");
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
        printf("\nMovie: %s\n", movies[i]);
        for (int j = 1; j <= 100; j++) {
            if (seat[i][j])
                printf(" X ");
            else
                printf("%2d ", j);
            if (j % 10 == 0) printf("\n");
        }
    }
}

void bookTicket(const char* username) {
    int movieIndex, s;
    printf("\n----------------- Movie Selection ----------------\n");
    for (int i = 0; i < 3; i++) {
        printf("%d. %s\n", i + 1, movies[i]);
    }
    printf("--------------------------------------------------\n");
    printf("Choice: ");
    scanf("%d", &movieIndex);
    if (movieIndex < 1 || movieIndex > 3) {
        printf("Invalid movie selection.\n");
        return;
    }
    printf("\nAvailable Seats for %s:\n", movies[movieIndex - 1]);
    for (int i = 1; i <= 100; i++) {
        if (seat[movieIndex - 1][i])
            printf(" X ");
        else
            printf("%2d ", i);
        if (i % 10 == 0) printf("\n");
    }
    printf("\nEnter seat number to book: ");
    scanf("%d", &s);
    if (s < 1 || s > 100 || seat[movieIndex - 1][s]) {
        printf("Seat not available.\n");
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
            printf("Booking for %s (Seat %d in %s) cancelled.\n",
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
    printf("Booking ID not found.\n");
}

void refundMessage() {
    printf("Refund process initiated. Amount will be credited back within 3-5 business days.\n");
}

void generateTicket(struct moviedetails b, int price) {
    printf("\n==================================================\n");
    printf("                 MOVIE TICKET                     \n");
    printf("==================================================\n");
    printf("Booking ID : %d\n", b.id);
    printf("Movie      : %s\n", b.movie);
    printf("Name       : %s\n", b.name);
    printf("Seat No.   : %d\n", b.seat);
    printf("Phone No.  : %u\n", b.phone);
    printf("Price      : Rs. %d\n", price);
    printf("==================================================\n");
}
