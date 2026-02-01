# ATM Management System 🏦

[![C](https://img.shields.io/badge/Language-C-blue.svg)](<https://en.wikipedia.org/wiki/C_(programming_language)>)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

A robust, terminal-based **ATM Management System** built in C. It features a modern, centered **Text User Interface (TUI)** with keyboard navigation, secure authentication, and comprehensive account management capabilities.

---

## ✨ Features

- **Modern TUI Experience** 🖥️
  - **Centered Layout**: All menus, prompts, and status messages are perfectly centered.
  - **Keyboard Navigation**: Use **UP/DOWN** arrows to navigate menus and **ENTER** to select.
  - **Responsive Input**: Supports **ESC** to cancel any operation instantly.
  - **Boxed Interface**: aesthetically pleasing double-line borders.

- **Secure Authentication** �
  - **Login System**: Secure access with password masking (`*`).
  - **Registration**: New users can securely sign up.
  - **Data Persistence**: All user and account data is saved to local files (`users.txt`, `records.txt`).

- **Account Operations** �
  - **Create Account**: Open new accounts (Savings, Fixed, Current).
  - **Update Account**: Modify phone numbers or country details.
  - **Check Accounts**: View all owned accounts in a list.
  - **Account Details**: Lookup specific account details by **Account Number**.
  - **Remove Account**: Delete closed accounts.
  - **Transfer Ownership**: Transfer accounts to other users securely.

- **Transactions** �
  - **Deposit**: Add funds to your account (Currency: **BHD**).
  - **Withdraw**: Withdraw funds with balance validation.
  - **Validation**: Strict input checks for amounts, dates, and IDs.

---

## 📸 Terminal Examples

The system features a polished, centered interface. Here is exactly what it looks like in your terminal:

### 1. Main Menu

The heart of the navigation. options are highlighted when selected.

```text
        ╔══════════════════════════════════════════════════════════════╗
        ║                    ATM MANAGEMENT SYSTEM                     ║
        ╚══════════════════════════════════════════════════════════════╝

                               === Main Menu ===

                              Create a new account
                           Update account information
                                 Check accounts
                        > Check list of owned account <
                                Make Transaction
                            Remove existing account
                               Transfer ownership
                                      Exit

                Use UP/DOWN arrows to navigate, ENTER to select.
```

### 2. Login Screen

Secure entry point.

```text
        ╔══════════════════════════════════════════════════════════════╗
        ║                    ATM MANAGEMENT SYSTEM                     ║
        ╚══════════════════════════════════════════════════════════════╝

                           === Welcome to ATM System ===

                                    > Login <
                                     Register
                                       Exit

                Use UP/DOWN arrows to navigate, ENTER to select.
```

### 3. Account Details

Clear presentation of account information.

```text
                               === Account Details ===

        Account number: 101
        Deposit Date:   1/15/2024
        Country:        Bahrain
        Phone number:   33001122
        Amount:         BHD 1500.50
        Type:           saving

        Interest Rate: 7%
        Monthly Interest Reward: BHD 8.75

        Press any key to continue...
```

### 4. Input Validation

Robust handling of user input with error messages.

```text
                               === Make Transaction ===

                       Enter the account number: 999

                       [!] Account not found or access denied.

        Press any key to continue...
```

---

## 🚀 Getting Started

### Prerequisites

- GCC Compiler (or any standard C compiler)
- Make (optional, for easy building)

### Installation

1.  **Clone the repository**:

    ```bash
    git clone https://github.com/yourusername/atm-management-system.git
    cd atm-management-system
    ```

2.  **Compile the project**:

    ```bash
    make
    ```

    _Or manually:_ `gcc -o atm src/main.c src/system.c src/auth.c src/ui.c`

3.  **Run the application**:
    ```bash
    ./atm
    ```

---

## �️ Project Structure

```
atm-management-system/
├── src/
│   ├── main.c       # Entry point & menu logic
│   ├── system.c     # Account operations & transaction logic
│   ├── auth.c       # Authentication (Login/Register)
│   ├── ui.c         # TUI handling, Input Validation details
│   ├── header.h     # Data structures & Global declarations
│   └── ui.h         # UI function declarations
├── data/
│   ├── users.txt    # User credentials database
│   └── records.txt  # Account records database
├── Makefile         # Build configuration
└── README.md        # Documentation
```

## 🤝 Contributing

Contributions are welcome! Please fork the repository and submit a pull request for any enhancements or bug fixes.

---

**Developed with ❤️ by Sayed Ahmed**
