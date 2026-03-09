#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

const string DATA_FILE = "inventory.csv";


// ─────────────────────────────────────────────
//  Product Structure
// ─────────────────────────────────────────────
struct Product {
    int    id;
    string name;
    double price;
    int    quantity;
};

// ─────────────────────────────────────────────
//  Inventory Manager Class
// ─────────────────────────────────────────────
class InventoryManager {
private:
    vector<Product> products;
    int nextId;

    // ── helpers ──────────────────────────────
    int findIndexById(int id) const {
        for (size_t i = 0; i < products.size(); ++i)
            if (products[i].id == id) return static_cast<int>(i);
        return -1;
    }

    static void printHeader() {
        cout << "\n"
             << string(62, '-') << "\n"
             << left
             << setw(6)  << "ID"
             << setw(22) << "Name"
             << setw(12) << "Price"
             << setw(10) << "Qty"
             << "Value\n"
             << string(62, '-') << "\n";
    }

    static void printRow(const Product& p) {
        cout << left
             << setw(6)  << p.id
             << setw(22) << p.name
             << "₹" << right << setw(10) << fixed << setprecision(2) << p.price
             << setw(8) << p.quantity
             << "  ₹" << setw(10) << fixed << setprecision(2)
             << (p.price * p.quantity) << "\n";
    }

    // ── input utilities ──────────────────────
    static int readInt(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return value; }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Please enter a valid integer.\n";
        }
    }

    static int readNonNegativeInt(const string& prompt) {
        int v;
        while (true) {
            v = readInt(prompt);
            if (v >= 0) return v;
            cout << "  [!] Value cannot be negative.\n";
        }
    }

    static double readDouble(const string& prompt) {
        double value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= 0) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return value; }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Please enter a valid non-negative number.\n";
        }
    }

    static string readString(const string& prompt) {
        string value;
        while (true) {
            cout << prompt;
            getline(cin, value);
            if (!value.empty()) return value;
            cout << "  [!] Input cannot be empty.\n";
        }
    }

    static int readPositiveInt(const string& prompt) {
        int v;
        while (true) {
            v = readInt(prompt);
            if (v > 0) return v;
            cout << "  [!] Value must be greater than 0.\n";
        }
    }

public:
    InventoryManager() : nextId(1) {
        loadFromFile();
    }

    // ── CSV persistence ──────────────────────
    void saveToFile() const {
        ofstream file(DATA_FILE);
        if (!file.is_open()) {
            cout << "  [!] Could not save to " << DATA_FILE << "\n";
            return;
        }

        // Write header
        file << "id,name,price,quantity\n";
        
        for (const auto& p : products) {
            // Escape quotes inside the name by doubling them, and wrap in quotes if there's a comma
            string nameStr = p.name;
            bool needsQuotes = nameStr.find(',') != string::npos || nameStr.find('"') != string::npos;
            
            if (needsQuotes) {
                string escaped = "\"";
                for (char c : nameStr) {
                    if (c == '"') escaped += "\"\"";
                    else escaped += c;
                }
                escaped += "\"";
                nameStr = escaped;
            }

            file << p.id << ","
                 << nameStr << ","
                 << fixed << setprecision(2) << p.price << ","
                 << p.quantity << "\n";
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file(DATA_FILE);
        if (!file.is_open()) return; // File doesn't exist yet, start fresh

        string line;
        // Skip header
        if (!getline(file, line)) return;

        products.clear();
        int maxId = 0;

        while (getline(file, line)) {
            if (line.empty()) continue;

            Product p;
            size_t pos = 0;
            
            // ID
            size_t commaPos = line.find(',');
            if (commaPos == string::npos) continue;
            p.id = stoi(line.substr(0, commaPos));
            pos = commaPos + 1;

            // Name (handle CSV quoting)
            if (pos < line.length() && line[pos] == '"') {
                pos++; // skip opening quote
                string namePart = "";
                while (pos < line.length()) {
                    if (line[pos] == '"') {
                        if (pos + 1 < line.length() && line[pos+1] == '"') {
                            namePart += '"';
                            pos += 2;
                        } else {
                            pos++; // skip closing quote
                            break;
                        }
                    } else {
                        namePart += line[pos++];
                    }
                }
                p.name = namePart;
                if (pos < line.length() && line[pos] == ',') pos++;
            } else {
                commaPos = line.find(',', pos);
                if (commaPos == string::npos) continue;
                p.name = line.substr(pos, commaPos - pos);
                pos = commaPos + 1;
            }

            // Price
            commaPos = line.find(',', pos);
            if (commaPos == string::npos) continue;
            p.price = stod(line.substr(pos, commaPos - pos));
            pos = commaPos + 1;

            // Quantity
            p.quantity = stoi(line.substr(pos));

            products.push_back(p);
            if (p.id > maxId) maxId = p.id;
        }

        nextId = maxId + 1;
        if (nextId < 1) nextId = 1;
        
        file.close();
        
        if (!products.empty()) {
            cout << "  [i] Loaded " << products.size() << " product(s) from " << DATA_FILE << "\n";
        }
    }

    // ── 1. Add Product ───────────────────────
    void addProduct() {
        cout << "\n===  Add New Product  ===\n";
        string name   = readString("  Product name : ");
        double price  = readDouble("  Price (₹)    : ");
        int    qty    = readPositiveInt("  Quantity     : ");

        products.push_back({nextId++, name, price, qty});
        cout << "  [+] Product \"" << name << "\" added  (ID " << products.back().id << ")\n";
        saveToFile();
    }

    // ── 2. Update Stock (fixed) ──────────────
    void updateStock() {
        if (products.empty()) { cout << "\n  [!] Inventory is empty.\n"; return; }

        cout << "\n===  Update Stock  ===\n";
        displayInventory();
        int id = readInt("  Enter product ID to update: ");
        int idx = findIndexById(id);
        if (idx == -1) { cout << "  [!] Product ID " << id << " not found.\n"; return; }

        Product& p = products[idx];

        cout << "\n  Selected: \"" << p.name << "\"  |  ₹"
             << fixed << setprecision(2) << p.price
             << "  |  Qty: " << p.quantity << "\n\n";
        cout << "  1) Update quantity (set new value)\n";
        cout << "  2) Add to stock\n";
        cout << "  3) Remove from stock\n";
        cout << "  4) Update price\n";
        cout << "  5) Update name\n";
        cout << "  6) Update all fields\n";
        cout << "  0) Cancel\n";
        int choice = readInt("  Choice: ");

        switch (choice) {
            case 1: {
                int qty = readNonNegativeInt("  New quantity: ");
                p.quantity = qty;
                cout << "  [+] Quantity set to " << qty << "\n";
                break;
            }
            case 2: {
                int add = readPositiveInt("  Amount to add: ");
                p.quantity += add;
                cout << "  [+] Added " << add << ". New quantity: " << p.quantity << "\n";
                break;
            }
            case 3: {
                int rem = readPositiveInt("  Amount to remove: ");
                if (rem > p.quantity) {
                    cout << "  [!] Cannot remove " << rem << " — only " << p.quantity << " in stock.\n";
                    return;
                }
                p.quantity -= rem;
                cout << "  [+] Removed " << rem << ". New quantity: " << p.quantity << "\n";
                break;
            }
            case 4: {
                double newPrice = readDouble("  New price (₹): ");
                p.price = newPrice;
                cout << "  [+] Price updated to ₹" << fixed << setprecision(2) << newPrice << "\n";
                break;
            }
            case 5: {
                string newName = readString("  New name: ");
                cout << "  [+] Name changed from \"" << p.name << "\" to \"" << newName << "\"\n";
                p.name = newName;
                break;
            }
            case 6: {
                string newName = readString("  New name [" + p.name + "]: ");
                double newPrice = readDouble("  New price (₹): ");
                int newQty = readNonNegativeInt("  New quantity: ");
                p.name = newName;
                p.price = newPrice;
                p.quantity = newQty;
                cout << "  [+] Product fully updated.\n";
                break;
            }
            case 0:
                cout << "  Cancelled.\n";
                return;
            default:
                cout << "  [!] Invalid choice.\n";
                return;
        }
        saveToFile();
    }

    // ── 3. Display Inventory ─────────────────
    void displayInventory() const {
        if (products.empty()) { cout << "\n  [!] Inventory is empty.\n"; return; }

        printHeader();
        double totalValue = 0;
        for (const auto& p : products) {
            printRow(p);
            totalValue += p.price * p.quantity;
        }
        cout << string(62, '-') << "\n"
             << "  Total products: " << products.size()
             << "    |    Total value: ₹" << fixed << setprecision(2) << totalValue << "\n";
    }

    // ── 4. Search Product ────────────────────
    void searchProduct() const {
        if (products.empty()) { cout << "\n  [!] Inventory is empty.\n"; return; }

        cout << "\n===  Search Product  ===\n";
        cout << "  1) Search by ID\n";
        cout << "  2) Search by name\n";
        int choice = readInt("  Choice: ");

        if (choice == 1) {
            int id = readInt("  Enter product ID: ");
            int idx = findIndexById(id);
            if (idx == -1) { cout << "  [!] Not found.\n"; return; }
            printHeader();
            printRow(products[idx]);
        } else if (choice == 2) {
            string keyword = readString("  Enter name keyword: ");
            string kwLower = keyword;
            transform(kwLower.begin(), kwLower.end(), kwLower.begin(), ::tolower);

            bool found = false;
            printHeader();
            for (const auto& p : products) {
                string nameLower = p.name;
                transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
                if (nameLower.find(kwLower) != string::npos) {
                    printRow(p);
                    found = true;
                }
            }
            if (!found) cout << "  [!] No products match \"" << keyword << "\".\n";
        } else {
            cout << "  [!] Invalid choice.\n";
        }
    }

    // ── 5. Remove Product ────────────────────
    void removeProduct() {
        if (products.empty()) { cout << "\n  [!] Inventory is empty.\n"; return; }

        cout << "\n===  Remove Product  ===\n";
        int id  = readInt("  Enter product ID: ");
        int idx = findIndexById(id);
        if (idx == -1) { cout << "  [!] Product ID " << id << " not found.\n"; return; }

        cout << "  Remove \"" << products[idx].name << "\"? (y/n): ";
        char confirm;
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (confirm == 'y' || confirm == 'Y') {
            cout << "  [-] \"" << products[idx].name << "\" removed.\n";
            products.erase(products.begin() + idx);
            saveToFile();
        } else {
            cout << "  Cancelled.\n";
        }
    }
};

// ─────────────────────────────────────────────
//  Main – Menu Loop
// ─────────────────────────────────────────────
int main() {
    InventoryManager mgr;
    int choice;

    while (true) {
        cout << "\n"
             << "╔══════════════════════════════════╗\n"
             << "║   INVENTORY MANAGEMENT SYSTEM    ║\n"
             << "╠══════════════════════════════════╣\n"
             << "║  1. Add Product                  ║\n"
             << "║  2. Update Stock                 ║\n"
             << "║  3. Display Inventory            ║\n"
             << "║  4. Search Product               ║\n"
             << "║  5. Remove Product               ║\n"
             << "║  0. Exit                         ║\n"
             << "╚══════════════════════════════════╝\n";

        cout << "  Enter choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Invalid input.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: mgr.addProduct();        break;
            case 2: mgr.updateStock();       break;
            case 3: mgr.displayInventory();  break;
            case 4: mgr.searchProduct();     break;
            case 5: mgr.removeProduct();     break;
            case 0:
                cout << "\n  Goodbye!\n";
                return 0;
            default:
                cout << "  [!] Invalid choice. Try again.\n";
        }
    }
}
