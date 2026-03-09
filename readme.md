# 📦 Inventory Management System

> **Assignment Project — B.Tech Computer Science & Engineering**
> **Karamveer Singh Qaumi** | ITM Skills University

---

## 👤 Student Details

| Field | Details |
|---|---|
| **Name** | Karamveer Singh Qaumi |
| **Program** | B.Tech Computer Science & Engineering |
| **University** | ITM Skills University |
| **Subject** | C++ |
| **Language** | C++ (Standard: C++11) |

---

## 📋 Table of Contents

1. [Project Overview](#-project-overview)
2. [Objectives](#-objectives)
3. [C++ Concepts Used](#-c-concepts-used)
4. [Features](#-features)
5. [System Requirements](#-system-requirements)
6. [Project Structure](#-project-structure)
7. [How to Compile & Run](#-how-to-compile--run)
8. [Menu & Usage Guide](#-menu--usage-guide)
9. [Data Storage & CSV Format](#-data-storage--csv-format)
10. [Input Validation](#-input-validation)
11. [Sample Output](#-sample-output)
12. [Limitations & Future Scope](#-limitations--future-scope)

---

## 📌 Project Overview

The **Inventory Management System** is a console-based C++ application designed to help a user manage a product inventory. It supports full **CRUD operations** (Create, Read, Update, Delete) on product records and persists all data to a local CSV file so that inventory is retained across program sessions.

This project demonstrates practical application of core C++ programming concepts including classes, file I/O, vectors, structs, and robust input handling.

---

## 🎯 Objectives

- Apply **Object-Oriented Programming** principles using classes and structs
- Implement **file handling** in C++ for persistent data storage
- Practice **STL containers** (`std::vector`) for dynamic data management
- Build a clean, user-friendly **menu-driven CLI interface**
- Demonstrate **input validation** and defensive programming techniques
- Perform **string manipulation** and **case-insensitive searching**

---

## 🧠 C++ Concepts Used

| Concept | How It's Used |
|---|---|
| **Struct** | `Product` struct holds id, name, price, and quantity |
| **Class** | `InventoryManager` encapsulates all operations and data |
| **Encapsulation** | Private data members with public method interface |
| **Vectors (`std::vector`)** | Dynamic list of `Product` objects |
| **File I/O (`fstream`)** | Reading and writing `inventory.csv` |
| **String Streams (`sstream`)** | Parsing CSV lines |
| **STL Algorithms** | `std::transform` for lowercase conversion |
| **`iomanip`** | Formatted table output with `setw`, `setprecision`, `fixed` |
| **Input Validation** | Loops with `cin.fail()`, `cin.clear()`, `cin.ignore()` |
| **CSV Parsing** | Manual quote-aware CSV parser for robust name handling |
| **Constructor** | `InventoryManager()` initializes `nextId` and loads file |
| **`const` correctness** | Read-only methods marked `const` |
| **Static methods** | Utility helpers (`printHeader`, `readInt`, etc.) are `static` |

---

## ✨ Features

- ➕ **Add Product** — Register a new product with name, price, and quantity; auto-assigns a unique ID
- 🔄 **Update Stock** — Granular update options: set quantity, add stock, remove stock, change price, change name, or update everything at once
- 📊 **Display Inventory** — Formatted table showing ID, Name, Price, Quantity, and per-product Value; totals shown at the bottom
- 🔍 **Search Product** — Search by exact ID or by name keyword (case-insensitive partial match)
- 🗑️ **Remove Product** — Delete a product by ID with a confirmation prompt to prevent accidents
- 💾 **CSV Persistence** — All changes are immediately saved to `inventory.csv`; data loads automatically on startup

---

## 🖥️ System Requirements

| Requirement | Detail |
|---|---|
| **Compiler** | `g++` or `clang++` with C++11 support |
| **OS** | Windows / Linux / macOS |
| **Dependencies** | None (standard library only) |
| **Disk Space** | Minimal (~KB for source + data file) |

---

## 📁 Project Structure

```
inventory-management/
│
├── main.cpp           # Complete source code (single-file project)
├── inventory.csv      # Auto-generated data file (created on first save)
└── README.md          # Project documentation (this file)
```

**Inside `main.cpp`:**

```
main.cpp
│
├── struct Product               # Data model: id, name, price, quantity
│
├── class InventoryManager
│   ├── Private
│   │   ├── vector<Product> products
│   │   ├── int nextId
│   │   ├── findIndexById()      # Linear search by ID
│   │   ├── printHeader()        # Table header formatter
│   │   ├── printRow()           # Single product row formatter
│   │   ├── readInt()            # Validated integer input
│   │   ├── readPositiveInt()    # Validates > 0
│   │   ├── readNonNegativeInt() # Validates >= 0
│   │   ├── readDouble()         # Validated decimal input
│   │   └── readString()         # Validated non-empty string input
│   │
│   └── Public
│       ├── InventoryManager()   # Constructor — loads CSV on startup
│       ├── saveToFile()         # Writes products to inventory.csv
│       ├── loadFromFile()       # Reads and parses inventory.csv
│       ├── addProduct()         # Menu option 1
│       ├── updateStock()        # Menu option 2
│       ├── displayInventory()   # Menu option 3
│       ├── searchProduct()      # Menu option 4
│       └── removeProduct()      # Menu option 5
│
└── main()                       # Entry point — menu loop
```

---

## ⚙️ How to Compile & Run

### Step 1 — Compile

```bash
g++ -std=c++11 -o inventory main.cpp
```

Or with warnings enabled (recommended):

```bash
g++ -std=c++11 -Wall -Wextra -o inventory main.cpp
```

### Step 2 — Run

**Linux / macOS:**
```bash
./inventory
```

**Windows:**
```cmd
inventory.exe
```

> On first run, no CSV file is needed — the program starts with an empty inventory and creates `inventory.csv` automatically when you add the first product.

---

## 📖 Menu & Usage Guide

```
╔══════════════════════════════════╗
║   INVENTORY MANAGEMENT SYSTEM    ║
╠══════════════════════════════════╣
║  1. Add Product                  ║
║  2. Update Stock                 ║
║  3. Display Inventory            ║
║  4. Search Product               ║
║  5. Remove Product               ║
║  0. Exit                         ║
╚══════════════════════════════════╝
```

---

### Option 1 — Add Product

Prompts for:
- Product name (non-empty string)
- Price in ₹ (non-negative decimal)
- Quantity (positive integer)

An auto-incremented ID is assigned. The product is immediately saved to `inventory.csv`.

---

### Option 2 — Update Stock

Displays the inventory table, then prompts for a product ID. Once found, a sub-menu offers:

| Sub-option | Action |
|---|---|
| 1 | Set quantity to a new value |
| 2 | Add a given amount to current stock |
| 3 | Remove a given amount from current stock |
| 4 | Update price |
| 5 | Update name |
| 6 | Update all fields (name, price, quantity) |
| 0 | Cancel — no changes saved |

---

### Option 3 — Display Inventory

Prints a formatted table:

```
--------------------------------------------------------------
ID    Name                  Price          Qty       Value
--------------------------------------------------------------
1     Wireless Mouse        ₹    899.00      50  ₹  44950.00
2     USB-C Cable           ₹    299.00     120  ₹  35880.00
--------------------------------------------------------------
  Total products: 2    |    Total value: ₹80830.00
```

---

### Option 4 — Search Product

- **By ID** — enters an exact integer ID and displays that product's row
- **By Name** — enters a keyword; all products whose names contain the keyword (case-insensitive) are displayed

---

### Option 5 — Remove Product

Prompts for a product ID, displays the product name, and asks for confirmation (`y/n`) before permanently deleting it and saving the updated file.

---

### Option 0 — Exit

Cleanly exits the program. All data is already saved automatically — no additional save step is needed.

---

## 💾 Data Storage & CSV Format

All data is stored in `inventory.csv` in the same directory as the executable.

**File format:**
```csv
id,name,price,quantity
1,Wireless Mouse,899.00,50
2,USB-C Cable,299.00,120
3,"Bolt, Large Size",5.50,300
```

**CSV handling details:**
- The first line is always a header row (`id,name,price,quantity`) and is skipped on load
- Product names containing a comma or double-quote are automatically wrapped in double-quotes
- Double-quotes inside names are escaped by doubling them (standard CSV escaping)
- `nextId` is recalculated from the maximum ID found in the file on each load, preventing ID collisions

> ⚠️ **Warning:** Do not edit `inventory.csv` while the program is running — changes will be overwritten on the next save operation.

---

## ✅ Input Validation

The program never crashes on bad input. Every input field is validated in a loop:

| Scenario | Behaviour |
|---|---|
| Non-numeric input for integers | Prompts again with `[!] Please enter a valid integer.` |
| Negative price | Prompts again with `[!] Please enter a valid non-negative number.` |
| Zero or negative quantity when adding | Prompts again with `[!] Value must be greater than 0.` |
| Removing more stock than available | Prints error, cancels the operation |
| Empty product name | Prompts again with `[!] Input cannot be empty.` |
| Invalid menu option | Prints `[!] Invalid choice. Try again.` and re-shows menu |

---

## 🖨️ Sample Output

**Adding a product:**
```
===  Add New Product  ===
  Product name : Mechanical Keyboard
  Price (₹)    : 3499
  Quantity     : 25
  [+] Product "Mechanical Keyboard" added  (ID 1)
```

**Displaying inventory:**
```
--------------------------------------------------------------
ID    Name                  Price          Qty       Value
--------------------------------------------------------------
1     Mechanical Keyboard   ₹   3499.00      25  ₹  87475.00
2     USB Hub               ₹    799.00      40  ₹  31960.00
--------------------------------------------------------------
  Total products: 2    |    Total value: ₹119435.00
```

**Searching by name keyword `"hub"`:**
```
===  Search Product  ===
  1) Search by ID
  2) Search by name
  Choice: 2
  Enter name keyword: hub
--------------------------------------------------------------
ID    Name                  Price          Qty       Value
--------------------------------------------------------------
2     USB Hub               ₹    799.00      40  ₹  31960.00
```

---

## 🚀 Limitations & Future Scope

### Current Limitations
- Single-user, single-file — no multi-user or network support
- No sorting or filtering in the display view
- No low-stock alerts or threshold warnings
- IDs are never reused after deletion

### Possible Future Enhancements
- Sort inventory by name, price, or quantity
- Low-stock warning system (e.g., alert when quantity < 10)
- Category/tag support for products
- Export reports as formatted text files
- Replace CSV with a lightweight database (e.g., SQLite)
- Graphical User Interface (GUI) using Qt or a web front-end

---

*Developed by **Karamveer Singh Qaumi** — B.Tech Computer Science & Engineering, ITM Skills University*