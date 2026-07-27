# Personal Finance Manager

A desktop application built with **C++** and **Qt 6 Widgets** that helps users manage their personal finances by tracking income and expenses, monitoring account balances, and viewing financial statistics.

---

## Preview

> 🚧 Project currently under development.

Screenshots will be added as features are completed.

---

## Features

### Dashboard
- View current balance
- View total income
- View total expenses
- View total number of transactions

### Transaction Management
- Add income
- Add expenses
- Edit transactions
- Delete transactions

### Transaction History
- View all recorded transactions
- Search transactions
- Filter by category or type

### Statistics
- Current balance
- Total income
- Total expenses
- Largest expense
- Largest income
- Monthly spending summary

### Data Storage
- Save transactions locally
- Automatically load saved data on startup

---

## Technologies

- C++
- Qt 6 (Qt Widgets)
- CMake
- Git
- GitHub

---

## Project Structure

```
PersonalFinanceManager/
│
├── src/
│
├── include/
│
├── resources/
│
├── screenshots/
│
├── README.md
├── .gitignore
└── CMakeLists.txt
```

---

## Planned Architecture

```
MainWindow
│
├── FinanceManager
│   ├── Add Transaction
│   ├── Remove Transaction
│   ├── Update Transaction
│   ├── Calculate Balance
│   └── Generate Statistics
│
├── Transaction
│   ├── Date
│   ├── Type
│   ├── Category
│   ├── Description
│   └── Amount
│
└── FileManager
    ├── Save Data
    └── Load Data
```

---

## Roadmap

### Phase 1
- [x] Create project
- [x] Configure Git
- [x] Publish to GitHub
- [ ] Design main window
- [ ] Create Transaction class

### Phase 2
- [ ] Add income
- [ ] Add expenses
- [ ] Display transactions
- [ ] Calculate totals

### Phase 3
- [ ] Edit transactions
- [ ] Delete transactions
- [ ] Search transactions
- [ ] Category filtering

### Phase 4
- [ ] Save data to JSON
- [ ] Load data automatically
- [ ] Charts
- [ ] Dark mode

---

## Skills Demonstrated

This project demonstrates:

- Object-Oriented Programming
- Qt Widgets
- Modern C++
- GUI Development
- File Handling
- Data Structures
- CMake
- Version Control with Git
- Software Design Principles

---

## Future Improvements

- Budget planning
- Spending reports
- Pie charts
- Monthly analytics
- Export to CSV
- Export to PDF
- Password protection
- Multiple user profiles

---

## Author

**Brandon**

GitHub: https://github.com/BranTK-dev
