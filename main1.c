#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLACK   "\x1b[1;90m"
#define RED     "\x1b[1;91m"
#define GREEN   "\x1b[1;92m"
#define YELLOW  "\x1b[1;93m"
#define BLUE    "\x1b[1;94m"
#define MAGENTA "\x1b[1;95m"
#define CYAN    "\x1b[1;96m"
#define WHITE   "\x1b[1;97m"
#define RESET   "\x1b[0m"

#define MAX_BOOKINGS 300
#define MAX_USERS 50
#define ADMIN_USER "joshi"
#define ADMIN_PASS "1234"

struct moviedetails {
    char name[50], phone[15], movie[50];
    int seat, id;
    char date[20];
    char time[10];
} bookings[MAX_BOOKINGS];

struct user {
    char username[30],password[20];
} users[MAX_USERS];

int bookingCount = 0, bookingID = 1000, userCount = 0, seat[3][101] = {0};
char *movies[] = {"ONE PIECE RED", "ONE PIECE GOLD", "ONE PIECE Z"};
char *showTimes[] = {"10:00", "14:00", "18:00"};

void mainMenu();
void adminPanel();
void userMenu(const char*);
void loadUsers();
void saveUser(struct user);
void loadBookings();
void saveBooking(struct moviedetails);
void saveAllBookings();
int loginAdmin();
int loginUser();
void registerUser();
void viewBookings();
void viewSeats();
void viewMyBookings(const char*);
void bookTicket(const char*);
void cancelTicket();
void generateTicket(struct moviedetails, int);
void refundMessage();

int main() {
    loadUsers();
    loadBookings();
    mainMenu();
    return 0;
}
void loadUsers() {
    FILE *f = fopen("users.txt", "r");
    if (f) {
        while (fscanf(f, "%s %s", users[userCount].username, users[userCount].password) == 2 && userCount < MAX_USERS)
            userCount++;
        fclose(f);
    }
}
void saveUser(struct user u) {
    FILE *f = fopen("users.txt", "a");
    if (f) {
        fprintf(f, "%s %s\n", u.username, u.password);
        fclose(f);
    } else {
        printf(RED "Error saving user data.\n" RESET);
    }
}
void loadBookings() {
    FILE *f = fopen("bookings.txt", "r");
    if (f) {
        while (fscanf(f, "%d %s %s %d %s %s %s",
                      &bookings[bookingCount].id, bookings[bookingCount].name, bookings[bookingCount].phone,
                      &bookings[bookingCount].seat, bookings[bookingCount].movie, bookings[bookingCount].date,
                      bookings[bookingCount].time) == 7) {
            for (int i = 0; i < 3; i++) {
                if (strcmp(bookings[bookingCount].movie, movies[i]) == 0)
                    seat[i][bookings[bookingCount].seat] = 1;
            }
            if (bookings[bookingCount].id >= bookingID)
                bookingID = bookings[bookingCount].id + 1;
            bookingCount++;
            if (bookingCount >= MAX_BOOKINGS) break;
        }
        fclose(f);
    }
}
void saveBooking(struct moviedetails b) {
    FILE *f = fopen("bookings.txt", "a");
    if (f) {
        fprintf(f, "%d %s %s %d %s %s %s\n", b.id, b.name, b.phone, b.seat, b.movie, b.date, b.time);
        fclose(f);
    } else {
        printf(RED "Error saving booking data.\n" RESET);
    }
}
void saveAllBookings() {
    FILE *f = fopen("bookings.txt", "w");
    if (f) {
        for (int i = 0; i < bookingCount; i++) {
            fprintf(f, "%d %s %s %d %s %s %s\n",
                    bookings[i].id, bookings[i].name, bookings[i].phone,
                    bookings[i].seat, bookings[i].movie, bookings[i].date, bookings[i].time);
        }
        fclose(f);
    } else {
        printf(RED "Error saving booking data.\n" RESET);
    }
}
void mainMenu() {
    int choice;
    while (1) {
        printf(CYAN "\n==================================================\n" RESET);
        printf(MAGENTA "            MOVIE TICKET BOOKING SYSTEM           \n" RESET);
        printf(CYAN "==================================================\n" RESET);
        printf(YELLOW "1. Admin Panel\n2. Register (New User)\n3. Sign In (Existing User)\n4. Exit\n" RESET);
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
            case 4:
                printf(GREEN "Thank you for using the system. Goodbye!\n" RESET);
                exit(0);
            default:
                printf(RED "Invalid choice. Try again!\n" RESET);
        }
    }
}
void adminPanel() {
    if (!loginAdmin()) return;
    int choice;
    while (1) {
        printf(BLUE "\n================= ADMIN PANEL ====================\n" RESET);
        printf("1. View All Bookings\n2. View Seats\n3. Back to Main Menu\n");
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
    saveUser(newUser);
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
        printf("1. Book Ticket\n2. Cancel Ticket\n3. View Seats\n4. View My Bookings\n5. Logout\n");
        printf(CYAN "--------------------------------------------------\n" RESET);
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: bookTicket(username); break;
            case 2: cancelTicket(); break;
            case 3: viewSeats(); break;
            case 4: viewMyBookings(username); break;
            case 5: {
                char confirm;
                printf(YELLOW "Are you sure you want to logout? (Y/N): " RESET);
                scanf(" %c", &confirm);
                if (confirm == 'Y' || confirm == 'y') return;
                break;
            }
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
        printf(BLUE "--------------------------------------------------\n" RESET);
        printf(GREEN "Booking ID : %d\n", bookings[i].id);
        printf("Movie      : %s\n", bookings[i].movie);
        printf("Name       : %s\n", bookings[i].name);
        printf("Phone      : %s\n", bookings[i].phone);
        printf("Seat No.   : %d\n", bookings[i].seat);
        printf("Date       : %s\n", bookings[i].date);
        printf("Time       : %s\n", bookings[i].time);
    }
    printf(BLUE "--------------------------------------------------\n" RESET);
}
void viewSeats() {
    for (int i = 0; i < 3; i++) {
        printf("\n" MAGENTA "Movie: %s\n" RESET, movies[i]);
        for (int j = 1; j <= 100; j++) {
            printf(seat[i][j] ? RED " X " RESET : GREEN "%2d " RESET, j);
            if (j % 10 == 0) printf("\n");
        }
    }
    printf("\n");
}
void viewMyBookings(const char* username) {
    int found = 0;
    printf(YELLOW "\n================ YOUR BOOKINGS ===================\n" RESET);
    for (int i = 0; i < bookingCount; i++) {
        if (strcmp(bookings[i].name, username) == 0) {
            printf(GREEN "Booking ID: %d | Movie: %s | Seat: %d | Date: %s | Time: %s\n" RESET,
                   bookings[i].id, bookings[i].movie, bookings[i].seat, bookings[i].date, bookings[i].time);
            found = 1;
        }
    }
    if (!found) printf(RED "No bookings found under your name.\n" RESET);
}
void bookTicket(const char* username) {
    int movieIndex, s, timeChoice;
    printf(CYAN "\n----------------- Movie Selection ----------------\n" RESET);
    for (int i = 0; i < 3; i++)
        printf(YELLOW "%d. %s\n" RESET, i + 1, movies[i]);
    printf(CYAN "--------------------------------------------------\n" RESET);
    printf("Choice: ");
    scanf("%d", &movieIndex);
    if (movieIndex < 1 || movieIndex > 3) {
        printf(RED "Invalid movie selection.\n" RESET);
        return;
    }
    printf("\nAvailable Show Timings:\n");
    for (int i = 0; i < 3; i++) {
        printf(YELLOW "%d. %s\n" RESET, i + 1, showTimes[i]);
    }
    printf("Select show time (1-3): ");
    scanf("%d", &timeChoice);
    if (timeChoice < 1 || timeChoice > 3) {
        printf(RED "Invalid time selection.\n" RESET);
        return;
    }
    printf("\nAvailable Seats for %s:\n", movies[movieIndex - 1]);
    for (int i = 1; i <= 100; i++) {
        printf(seat[movieIndex - 1][i] ? RED " X " RESET : GREEN "%2d " RESET, i);
        if (i % 10 == 0) printf("\n");
    }
    printf("\nEnter seat number to book: ");
    scanf("%d", &s);
    if (s < 1 || s > 100 || seat[movieIndex - 1][s]) {
        printf(RED "Seat not available.\n" RESET);
        return;
    }
    struct moviedetails b = {.seat = s, .id = bookingID++};
    strcpy(b.name, username);
    printf("Enter phone number: ");
    scanf("%s", b.phone);
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(b.date, sizeof(b.date), "%d-%m-%Y", t);
    strcpy(b.time, showTimes[timeChoice - 1]);
    strcpy(b.movie, movies[movieIndex - 1]);
    bookings[bookingCount++] = b;
    seat[movieIndex - 1][s] = 1;
    saveBooking(b);
    printf(GREEN "\nTicket booked successfully!\n" RESET);
    generateTicket(b, 100);  
}
void cancelTicket() {
    int id;
    printf("Enter Booking ID to cancel: ");
    scanf("%d", &id);
    int found = -1;
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].id == id) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        printf(RED "Booking ID not found.\n" RESET);
        return;
    }
    for (int i = 0; i < 3; i++) {
        if (strcmp(bookings[found].movie, movies[i]) == 0) {
            seat[i][bookings[found].seat] = 0;
            break;
        }
    }
    printf(GREEN "Booking ID %d canceled successfully.\n" RESET, id);
    for (int i = found; i < bookingCount - 1; i++) {
        bookings[i] = bookings[i + 1];
    }
    bookingCount--;
    saveAllBookings();
    refundMessage();
}
void generateTicket(struct moviedetails b, int price) {
    printf(GREEN "\n+------------------------------------------+\n" RESET);
    printf(GREEN "|               TICKET DETAILS              |\n" RESET);
    printf(GREEN "+------------------------------------------+\n" RESET);
    printf(GREEN "| Booking ID : %-30d |\n", b.id);
    printf(GREEN "| Name       : %-30s |\n", b.name);
    printf(GREEN "| Phone      : %-30s |\n", b.phone);
    printf(GREEN "| Movie      : %-30s |\n", b.movie);
    printf(GREEN "| Seat No.   : %-30d |\n", b.seat);
    printf(GREEN "| Date       : %-30s |\n", b.date);
    printf(GREEN "| Time       : %-30s |\n", b.time);
    printf(GREEN "| Price      : Rs. %-27d |\n", price);
    printf(GREEN "+------------------------------------------+\n\n" RESET);
}
void refundMessage() {
    printf(YELLOW "Refund will be processed within 7 working days.\n" RESET);
}
