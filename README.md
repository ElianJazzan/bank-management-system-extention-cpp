# Bank Management System - Extension 2 (C++ Console Application)

A fully functional console-based Bank Management System developed in C++. This project is an extended version of the original Bank Management System, adding user authentication, user management, and permission-based access control.

---

## Overview

This system allows management of bank clients, transactions, and system users through a menu-driven interface. Client and user data is stored persistently in text files and can be modified through the application.

The project demonstrates core C++ concepts such as:
- File handling
- Structs
- Vectors
- Modular programming
- Enums
- User authentication
- Permission management
- Menu-driven systems

---

## Features

### Client Management
- View all clients in the system
- Add new clients with validation
- Delete existing clients
- Update client information
- Search for clients by account number

### Financial Operations
- Deposit money into an account
- Withdraw money with balance validation
- Display total balance of all clients

### User Management
- View all users
- Add new users
- Delete users
- Update user information
- Search for users by username

### Authentication
- Username and password login system
- User verification through stored credentials
- Logout functionality

### Permission System
Each user can be assigned specific permissions that control access to different parts of the system.

Available permissions include:
- List Clients
- Add New Client
- Delete Client
- Update Client
- Find Client
- Transactions
- Manage Users

Users can also be granted full access to the system.

---

## Data Persistence

The system uses text files to store data without requiring a database.

Client data is stored in:
```text
clients.txt
```

Each line represents one client record, with fields separated by `///`:
```text
AccountNumber///PinCode///FullName///PhoneNumber///AccountBalance
```

User data is stored in:
```text
users.txt
```

Each line represents one user record, with fields separated by `///`:
```text
UserName///PassWord///Permission
```

`Permission` is stored as a single integer representing a combination of bit flags (e.g. `5` means access to both *List Clients* and *Delete Client*). A value of `-1` means the user has full access to the system.

---

## How Permissions Work

Permissions are defined as powers of two so each one occupies a unique bit:

```cpp
enum enPermission {
    pAll = -1, pList = 1, pAdd = 2, pDelete = 4,
    pUpdate = 8, pFind = 16, pTrans = 32, pManage = 64
};
```

When a user is granted multiple permissions, their values are combined (e.g. `pList + pDelete = 5`). Before any restricted action runs, the system checks whether the required permission bit is present in the current user's stored permission value:

```cpp
bool checkAccessPermission(enPermission permission)
{
    if (currentUser.permission == enPermission::pAll)
        return true;

    if ((permission & currentUser.permission) == permission)
        return true;

    return false;
}
```

If the check fails, the user sees an access denied message and is returned to the menu instead of being allowed to proceed.

---

## Menu Structure

```
Main Menu
├── Show Client List
├── Add New Client
├── Delete Client
├── Update Client Info
├── Find Client
├── Transactions
│   ├── Deposit
│   ├── Withdraw
│   ├── Total Balances
│   └── Main Menu
├── Manage Users
│   ├── List Users
│   ├── Add New User
│   ├── Delete User
│   ├── Update User
│   ├── Find User
│   └── Main Menu
└── Logout
```

Each menu option runs its permission check first; if the current user lacks the required permission, access is denied and the menu is shown again.

---

## Getting Started

1. Compile the source file with a C++ compiler that supports C++17, e.g.:
   ```bash
   g++ -std=c++17 BankSystem_Completed.cpp -o BankSystem
   ```
2. Run the executable. On first run, `clients.txt` and `users.txt` will be created automatically once records are added.
3. Log in with an existing username and password stored in `users.txt`.

> Note: `system("cls")` and `system("pause>0")` are Windows-specific commands used for clearing the screen and pausing execution. On Linux/macOS, replace `"cls"` with `"clear"` and `"pause>0"` with a custom pause implementation (e.g. reading a character from `cin`).

---

## Tech Stack

- Language: C++ (C++17)
- Storage: Plain text files
- No external libraries required
