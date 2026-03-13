#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <fstream>

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
    double value() const { return price * quantity; }
};

// ─────────────────────────────────────────────
//  Inventory Manager Class
// ─────────────────────────────────────────────
class InventoryManager {
    vector<Product> products;
    int nextId = 1;

    // ── helpers ──────────────────────────────
    int findIndexById(int id) const {
        for (size_t i = 0; i < products.size(); ++i)
            if (products[i].id == id) return static_cast<int>(i);
        return -1;
    }

    static void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    static void printHeader() {
        cout << "\n" << string(62, '-') << "\n"
             << left << setw(6) << "ID" << setw(22) << "Name"
             << setw(12) << "Price" << setw(10) << "Qty" << "Value\n"
             << string(62, '-') << "\n";
    }

    static void printRow(const Product& p) {
        cout << left  << setw(6)  << p.id << setw(22) << p.name
             << "₹" << right << setw(10) << fixed << setprecision(2) << p.price
             << setw(8) << p.quantity
             << "  ₹" << setw(10) << p.value() << "\n";
    }

    // ── input utilities (consolidated) ──────
    static int readInt(const string& prompt, int minVal = INT_MIN) {
        int v;
        while (true) {
            cout << prompt;
            if (cin >> v && v >= minVal) { clearInput(); return v; }
            clearInput();
            if (minVal > 0)      cout << "  [!] Value must be greater than 0.\n";
            else if (minVal == 0) cout << "  [!] Value cannot be negative.\n";
            else                  cout << "  [!] Please enter a valid integer.\n";
        }
    }

    static double readDouble(const string& prompt) {
        double v;
        while (true) {
            cout << prompt;
            if (cin >> v && v >= 0) { clearInput(); return v; }
            clearInput();
            cout << "  [!] Please enter a valid non-negative number.\n";
        }
    }

    static string readString(const string& prompt) {
        string v;
        while (true) {
            cout << prompt;
            getline(cin, v);
            if (!v.empty()) return v;
            cout << "  [!] Input cannot be empty.\n";
        }
    }

    static bool confirm(const string& msg) {
        cout << msg;
        char c; cin >> c; clearInput();
        return c == 'y' || c == 'Y';
    }

    // ── CSV helpers ─────────────────────────
    static string csvEscape(const string& s) {
        if (s.find(',') == string::npos && s.find('"') == string::npos) return s;
        string out = "\"";
        for (char c : s) { if (c == '"') out += '"'; out += c; }
        return out + "\"";
    }

    static string csvParseField(const string& line, size_t& pos) {
        if (pos >= line.size()) return "";
        string result;
        if (line[pos] == '"') {
            pos++; // skip opening quote
            while (pos < line.size()) {
                if (line[pos] == '"') {
                    if (pos + 1 < line.size() && line[pos + 1] == '"') { result += '"'; pos += 2; }
                    else { pos++; break; } // closing quote
                } else { result += line[pos++]; }
            }
            if (pos < line.size() && line[pos] == ',') pos++;
        } else {
            size_t comma = line.find(',', pos);
            if (comma == string::npos) { result = line.substr(pos); pos = line.size(); }
            else { result = line.substr(pos, comma - pos); pos = comma + 1; }
        }
        return result;
    }

public:
    InventoryManager() { loadFromFile(); }

    // ── CSV persistence ──────────────────────
    void saveToFile() const {
        ofstream file(DATA_FILE);
        if (!file) { cout << "  [!] Could not save to " << DATA_FILE << "\n"; return; }
        file << "id,name,price,quantity\n";
        for (const auto& p : products)
            file << p.id << "," << csvEscape(p.name) << ","
                 << fixed << setprecision(2) << p.price << "," << p.quantity << "\n";
    }

    void loadFromFile() {
        ifstream file(DATA_FILE);
        if (!file) return;

        string line;
        if (!getline(file, line)) return; // skip header

        products.clear();
        int maxId = 0;
        while (getline(file, line)) {
            if (line.empty()) continue;
            try {
                size_t pos = 0;
                int id   = stoi(csvParseField(line, pos));
                string n = csvParseField(line, pos);
                double pr = stod(csvParseField(line, pos));
                int qty  = stoi(csvParseField(line, pos));
                products.push_back({id, n, pr, qty});
                if (id > maxId) maxId = id;
            } catch (...) { continue; } // skip malformed lines
        }
        nextId = max(1, maxId + 1);
        if (!products.empty())
            cout << "  [i] Loaded " << products.size() << " product(s) from " << DATA_FILE << "\n";
    }

    // ── 1. Add Product ───────────────────────
    void addProduct() {
        cout << "\n===  Add New Product  ===\n";
        string name  = readString("  Product name : ");
        double price = readDouble("  Price (₹)    : ");
        int    qty   = readInt("  Quantity     : ", 1);

        products.push_back({nextId++, name, price, qty});
        cout << "  [+] Product \"" << name << "\" added  (ID " << products.back().id << ")\n";
        saveToFile();
    }

    // ── 2. Update Stock ──────────────────────
    void updateStock() {
        if (products.empty()) { cout << "\n  [!] Inventory is empty.\n"; return; }

        cout << "\n===  Update Stock  ===\n";
        displayInventory();
        int idx = findIndexById(readInt("  Enter product ID to update: "));
        if (idx == -1) { cout << "  [!] Product not found.\n"; return; }

        Product& p = products[idx];
        cout << "\n  Selected: \"" << p.name << "\"  |  ₹"
             << fixed << setprecision(2) << p.price << "  |  Qty: " << p.quantity << "\n\n"
             << "  1) Set quantity    2) Add stock      3) Remove stock\n"
             << "  4) Update price   5) Update name    6) Update all\n"
             << "  0) Cancel\n";

        switch (readInt("  Choice: ")) {
            case 1: { int q = readInt("  New quantity: ", 0);
                      p.quantity = q;
                      cout << "  [+] Quantity set to " << q << "\n"; break; }
            case 2: { int a = readInt("  Amount to add: ", 1);
                      p.quantity += a;
                      cout << "  [+] Added " << a << ". New quantity: " << p.quantity << "\n"; break; }
            case 3: { int r = readInt("  Amount to remove: ", 1);
                      if (r > p.quantity) { cout << "  [!] Cannot remove " << r << " — only " << p.quantity << " in stock.\n"; return; }
                      p.quantity -= r;
                      cout << "  [+] Removed " << r << ". New quantity: " << p.quantity << "\n"; break; }
            case 4: { double np = readDouble("  New price (₹): ");
                      p.price = np;
                      cout << "  [+] Price updated to ₹" << fixed << setprecision(2) << np << "\n"; break; }
            case 5: { string nn = readString("  New name: ");
                      cout << "  [+] Name changed from \"" << p.name << "\" to \"" << nn << "\"\n";
                      p.name = nn; break; }
            case 6: { p.name = readString("  New name [" + p.name + "]: ");
                      p.price = readDouble("  New price (₹): ");
                      p.quantity = readInt("  New quantity: ", 0);
                      cout << "  [+] Product fully updated.\n"; break; }
            case 0:  cout << "  Cancelled.\n"; return;
            default: cout << "  [!] Invalid choice.\n"; return;
        }
        saveToFile();
    }

    // ── 3. Display Inventory ─────────────────
    void displayInventory() const {
        if (products.empty()) { cout << "\n  [!] Inventory is empty.\n"; return; }
        printHeader();
        double total = 0;
        for (const auto& p : products) { printRow(p); total += p.value(); }
        cout << string(62, '-') << "\n  Total products: " << products.size()
             << "    |    Total value: ₹" << fixed << setprecision(2) << total << "\n";
    }

    // ── 4. Search Product ────────────────────
    void searchProduct() const {
        if (products.empty()) { cout << "\n  [!] Inventory is empty.\n"; return; }

        cout << "\n===  Search Product  ===\n  1) Search by ID\n  2) Search by name\n";
        int choice = readInt("  Choice: ");

        if (choice == 1) {
            int idx = findIndexById(readInt("  Enter product ID: "));
            if (idx == -1) { cout << "  [!] Not found.\n"; return; }
            printHeader(); printRow(products[idx]);
        } else if (choice == 2) {
            string kw = readString("  Enter name keyword: ");
            transform(kw.begin(), kw.end(), kw.begin(), ::tolower);
            bool found = false;
            printHeader();
            for (const auto& p : products) {
                string low = p.name;
                transform(low.begin(), low.end(), low.begin(), ::tolower);
                if (low.find(kw) != string::npos) { printRow(p); found = true; }
            }
            if (!found) cout << "  [!] No products match \"" << kw << "\".\n";
        } else { cout << "  [!] Invalid choice.\n"; }
    }

    // ── 5. Remove Product ────────────────────
    void removeProduct() {
        if (products.empty()) { cout << "\n  [!] Inventory is empty.\n"; return; }

        cout << "\n===  Remove Product  ===\n";
        int idx = findIndexById(readInt("  Enter product ID: "));
        if (idx == -1) { cout << "  [!] Product not found.\n"; return; }

        if (confirm("  Remove \"" + products[idx].name + "\"? (y/n): ")) {
            cout << "  [-] \"" << products[idx].name << "\" removed.\n";
            products.erase(products.begin() + idx);
            saveToFile();
        } else { cout << "  Cancelled.\n"; }
    }
};

// ─────────────────────────────────────────────
//  Main – Menu Loop
// ─────────────────────────────────────────────
int main() {
    InventoryManager mgr;

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
             << "╚══════════════════════════════════╝\n"
             << "  Enter choice: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Invalid input.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: mgr.addProduct();       break;
            case 2: mgr.updateStock();      break;
            case 3: mgr.displayInventory(); break;
            case 4: mgr.searchProduct();    break;
            case 5: mgr.removeProduct();    break;
            case 0: cout << "\n  Goodbye!\n"; return 0;
            default: cout << "  [!] Invalid choice. Try again.\n";
        }
    }
}
