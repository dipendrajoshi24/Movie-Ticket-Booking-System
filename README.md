# 🎟️ Movie Ticket Booking System
A fully functional **Movie Ticket Booking System** written in **C**. This console-based application allows users to register, log in, and book movie tickets. Admins can manage movies, schedules, and view bookings. Data is persisted using file handling.
## 📌 Features

### User Features
- ✅ **Registration & Login**: Secure user accounts with username/password.
- ✅ **Browse Movies**: View available movies and show timings.
- ✅ **Book Tickets**: Select seats and book tickets with a unique booking ID.
- ✅ **Cancel Bookings**: Cancel existing bookings using the booking ID.

### Admin Features
- ✅ Add/Remove Movies: Manage the list of movies and schedules.
- ✅ View Bookings: Access all booking records.
- ✅ Admin Login: Dedicated admin portal with password protection.

### General
- ✅ File-Based Storage: Data persisted in text files (`users.dat`, `movies.dat`, `bookings.dat`).
- ✅ Colored Terminal Output: Enhanced UX with ANSI escape codes.

### Prerequisites
- GCC Compiler
- Terminal supporting ANSI colors (e.g., [Windows Terminal](https://aka.ms/terminal), Linux/macOS terminals)

### 🚀 Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/dipendrajoshi24/movie-ticket-booking-system.git
   cd movie-ticket-booking-system
   Compile the code:

bash
gcc main.c -o movie_ticket_system
Run the executable:

bash
./movie_ticket_system
### 🎥 Usage
Launch the Application: Run the compiled executable.

Main Menu:

User: Register or log in to book/cancel tickets.

Admin: Log in with credentials (default: admin/password).

User Flow:

Browse movies → Select showtime → Choose seats → Confirm booking.

Cancel bookings by entering the booking ID.

Admin Flow:

Add/delete movies, manage schedules, and view all bookings.

### 💾 Data Storage
Users: Stored in users.dat (format: username,password).

Bookings: Stored in bookings.dat (format: booking_id,username,movie_id,seats_booked).

### 🤝 Contributing
Contributions are welcome Open an issue or submit a PR:

### 🙏 Acknowledgements
ANSI color codes for terminal styling.

Inspired by real-world ticket booking systems.
