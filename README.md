# 🏦 ApexVault

[![C](https://img.shields.io/badge/Language-C11-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE.md)
[![Platform](https://img.shields.io/badge/Platform-Terminal%20%7C%20POSIX-orange)](#-getting-started)

**ApexVault** is a terminal-based financial management engine and banking system implemented in C. Featuring a centered Text User Interface (TUI) with interactive arrow-key navigation, secure password masking, dynamic interest reward calculations, and multi-account ledger persistence.

---

## ✨ Features

- **Centered TUI Layout**: Double-line boxed interface rendered with ASCII border alignment.
- **Keyboard Navigation**: Interactive selection with `UP`/`DOWN` arrow keys, `ENTER` selection, and instant `ESC` cancel.
- **Authentication Security**: User signup/login authentication with password masking (`*`) and persistent credentials storage.
- **Multi-Account Support**: Manage Savings, Fixed, and Current account types with individual deposit dates, balances, and account ownership transfer capabilities.
- **Financial Transactions**: Deposit/withdraw routines with real-time balance validation and interest reward calculation.
- **Input Validation**: Strict error handling for invalid account IDs, dates, and non-numeric inputs.

---

## 📋 Table of Contents

- [Features](#-features)
- [System Architecture](#-system-architecture)
- [State Machine & Transaction Flow](#-state-machine--transaction-flow)
- [Getting Started](#-getting-started)
- [Project Structure](#-project-structure)
- [License](#-license)

---

## 🏗️ System Architecture

```mermaid
graph TD
    A[Terminal Launch / main] --> B[UI Layer: TUI Menu Renderer]
    B --> C{Authenticated Session?}
    
    C -- No --> D[Auth System: Login / Register]
    D -->|Validate Credentials| E[(users.txt Database)]
    
    C -- Yes --> F[Main Dashboard Dispatcher]
    F --> G1[Account Management: Create / Update / Remove / Transfer]
    F --> G2[Financial Engine: Deposit / Withdraw / Interest Calc]
    F --> G3[Query Engine: Search Account / List Owned Accounts]
    
    G1 & G2 & G3 --> H[(records.txt Ledger Database)]
```

---

## 📐 State Machine & Transaction Flow

```mermaid
sequenceDiagram
    participant User
    participant TUI as TUI Interface (ui.c)
    participant Auth as Authentication Service (auth.c)
    participant Engine as Transaction Engine (system.c)
    participant DB as File Storage (records.txt)

    User->>TUI: Arrow Keys + Enter (Login)
    TUI->>Auth: loginMenu(username, password)
    Auth-->>TUI: Session Token / User ID
    
    User->>TUI: Select "Make Transaction -> Deposit"
    TUI->>Engine: depositBalance(account_no, amount)
    Engine->>DB: Read & Lock Account Record
    DB-->>Engine: Current Account Balance
    Engine->>Engine: Compute Updated Balance & Monthly Interest
    Engine->>DB: Write Updated Record
    Engine-->>TUI: Display Success & Receipt Summary
    TUI-->>User: Render Centered Confirmation Screen
```

---

## 🚀 Getting Started

### Prerequisites

- **C Compiler**: GCC or Clang
- **Make**: Build utility

### Build & Run

1. **Clone Repository**:
   ```bash
   git clone https://github.com/sahmedhusain/apex-vault.git
   cd apex-vault
   ```

2. **Compile Application**:
   ```bash
   make
   ```

3. **Launch ApexVault**:
   ```bash
   ./apex-vault
   ```

---

## 📂 Project Structure

```
apex-vault/
├── Makefile         # Build rules and dependencies
├── LICENSE.md       # MIT License
├── README.md        # Documentation
├── data/            # Local database storage
│   ├── users.txt    # User credentials store
│   └── records.txt  # Bank account ledger records
└── src/
    ├── main.c       # Application bootstrapper and main loop
    ├── auth.c       # Authentication & user registration routines
    ├── system.c     # Account CRUD operations and financial calculations
    ├── ui.c         # Centered TUI box renderer & keyboard reader
    └── header.h     # Core structs and function signatures
```

---

## 📄 License

Distributed under the MIT License. See [LICENSE](LICENSE.md) for details.
