#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <map>
#include <stdexcept>
using namespace std;

// -- Item -------------------------------------------------------------------
struct Item {
    int    id;
    string name;
    string category;
    int    quantity;
    double price;

    string toCSV() const {
        ostringstream oss;
        oss << id << "," << name << "," << category << ","
            << quantity << "," << fixed << setprecision(2) << price;
        return oss.str();
    }

    static Item fromCSV(const string& line) {
        istringstream ss(line);
        string tok;
        Item item;
        getline(ss, tok, ','); item.id       = stoi(tok);
        getline(ss, tok, ','); item.name     = tok;
        getline(ss, tok, ','); item.category = tok;
        getline(ss, tok, ','); item.quantity  = stoi(tok);
        getline(ss, tok, ','); item.price    = stod(tok);
        return item;
    }
};

// -- Inventory --------------------------------------------------------------
class Inventory {
private:
    vector<Item> items;
    string       filename;
    int          nextId;

    void recalcNextId() {
        nextId = 1;
        for (const auto& it : items)
            if (it.id >= nextId) nextId = it.id + 1;
    }

public:
    explicit Inventory(const string& file = "inventory.csv")
        : filename(file), nextId(1) { load(); }

    // -- File I/O --
    void load() {
        items.clear();
        ifstream f(filename);
        if (!f.is_open()) { cout << "[Info] No existing file -- starting fresh.\n"; return; }
        string line;
        bool first = true;
        while (getline(f, line)) {
            if (first) { first = false; continue; } // skip header
            if (line.empty()) continue;
            try { items.push_back(Item::fromCSV(line)); }
            catch (...) { cerr << "[Warn] Skipping malformed line: " << line << "\n"; }
        }
        recalcNextId();
        cout << "[OK] Loaded " << items.size() << " item(s) from " << filename << "\n";
    }

    void save() const {
        ofstream f(filename);
        if (!f.is_open()) throw runtime_error("Cannot open file for writing: " + filename);
        f << "id,name,category,quantity,price\n";
        for (const auto& it : items) f << it.toCSV() << "\n";
        cout << "[OK] Saved " << items.size() << " item(s) to " << filename << "\n";
    }

    // -- CRUD --
    Item& add(const string& name, const string& cat, int qty, double price) {
        if (qty < 0)   throw invalid_argument("Quantity cannot be negative.");
        if (price < 0) throw invalid_argument("Price cannot be negative.");
        items.push_back({nextId++, name, cat, qty, price});
        return items.back();
    }

    Item* findById(int id) {
        for (auto& it : items) if (it.id == id) return &it;
        return nullptr;
    }

    bool remove(int id) {
        auto it = find_if(items.begin(), items.end(), [id](const Item& i){ return i.id == id; });
        if (it == items.end()) return false;
        items.erase(it);
        return true;
    }

    bool updateQuantity(int id, int delta) {
        Item* it = findById(id);
        if (!it) return false;
        if (it->quantity + delta < 0) throw runtime_error("Stock would go negative.");
        it->quantity += delta;
        return true;
    }

    bool updatePrice(int id, double newPrice) {
        Item* it = findById(id);
        if (!it) return false;
        if (newPrice < 0) throw invalid_argument("Price cannot be negative.");
        it->price = newPrice;
        return true;
    }

    // -- Search --
    vector<Item*> searchByName(const string& q) {
        vector<Item*> res;
        string ql = q;
        transform(ql.begin(), ql.end(), ql.begin(), ::tolower);
        for (auto& it : items) {
            string nl = it.name;
            transform(nl.begin(), nl.end(), nl.begin(), ::tolower);
            if (nl.find(ql) != string::npos) res.push_back(&it);
        }
        return res;
    }

    vector<Item*> searchByCategory(const string& cat) {
        vector<Item*> res;
        for (auto& it : items)
            if (it.category == cat) res.push_back(&it);
        return res;
    }

    vector<Item*> lowStock(int threshold = 5) {
        vector<Item*> res;
        for (auto& it : items)
            if (it.quantity <= threshold) res.push_back(&it);
        return res;
    }

    // -- Reports --
    void printTable(const vector<Item*>& list = {}) const {
        const vector<const Item*>* src;
        vector<const Item*> all;
        if (list.empty()) {
            for (const auto& it : items) all.push_back(&it);
            src = &all;
        } else {
            vector<const Item*> tmp;
            for (auto* p : list) tmp.push_back(p);
            // print from tmp
            const int W1=5, W2=22, W3=14, W4=9, W5=10;
            cout << "\n"
                 << left << setw(W1) << "ID"
                 << setw(W2) << "Name"
                 << setw(W3) << "Category"
                 << right << setw(W4) << "Qty"
                 << setw(W5) << "Price" << "\n";
            cout << string(W1+W2+W3+W4+W5, '-') << "\n";
            for (auto* p : tmp) {
                cout << left << setw(W1) << p->id
                     << setw(W2) << p->name
                     << setw(W3) << p->category
                     << right << setw(W4) << p->quantity
                     << setw(W5) << fixed << setprecision(2) << p->price << "\n";
            }
            cout << "\n";
            return;
        }
        const int W1=5, W2=22, W3=14, W4=9, W5=10;
        cout << "\n"
             << left << setw(W1) << "ID"
             << setw(W2) << "Name"
             << setw(W3) << "Category"
             << right << setw(W4) << "Qty"
             << setw(W5) << "Price" << "\n";
        cout << string(W1+W2+W3+W4+W5, '-') << "\n";
        for (auto* p : *src) {
            string rowMark = p->quantity <= 5 ? " (!)" : "";
            cout << left << setw(W1) << p->id
                 << setw(W2) << (p->name + rowMark)
                 << setw(W3) << p->category
                 << right << setw(W4) << p->quantity
                 << setw(W5) << fixed << setprecision(2) << p->price << "\n";
        }
        cout << "\n";
    }

    void printSummary() const {
        double totalValue = 0;
        int    totalItems = 0;
        map<string,int> catCount;
        for (const auto& it : items) {
            totalValue += it.price * it.quantity;
            totalItems += it.quantity;
            catCount[it.category]++;
        }
        cout << "\n--- Inventory Summary ---\n";
        cout << "Total SKUs      : " << items.size()    << "\n";
        cout << "Total units     : " << totalItems       << "\n";
        cout << "Total value     : $" << fixed << setprecision(2) << totalValue << "\n";
        cout << "\nCategories:\n";
        for (auto& [cat, cnt] : catCount)
            cout << "  " << left << setw(16) << cat << cnt << " SKU(s)\n";
        cout << "\n";
    }

    int size() const { return (int)items.size(); }
};

// helper
void sep(const string& title = "") {
    cout << "\n" << string(60, '=');
    if (!title.empty()) cout << "\n " << title;
    cout << "\n" << string(60, '=') << "\n";
}

int main() {
    const string FILE = "inventory_demo.csv";
    Inventory inv(FILE);

    sep("1. Add Items");
    auto& i1 = inv.add("Wireless Mouse",    "Electronics", 45, 29.99);
    auto& i2 = inv.add("USB-C Hub",         "Electronics", 12, 49.99);
    auto& i3 = inv.add("Standing Desk Mat", "Office",      30, 39.99);
    auto& i4 = inv.add("HDMI Cable 2m",     "Electronics", 80, 9.99);
    auto& i5 = inv.add("Notebook A5",       "Stationery",  200, 3.49);
    auto& i6 = inv.add("Ballpoint Pens x5", "Stationery",  4,   2.99);
    auto& i7 = inv.add("Ergonomic Chair",   "Furniture",   3,  299.99);
    auto& i8 = inv.add("Laptop Stand",      "Electronics", 18, 34.99);
    cout << "Added " << inv.size() << " items.\n";

    sep("2. Full Inventory (! = low stock)");
    inv.printTable();

    sep("3. Summary Report");
    inv.printSummary();

    sep("4. Search by Name: 'desk'");
    auto hits = inv.searchByName("desk");
    cout << hits.size() << " result(s)\n";
    inv.printTable(hits);

    sep("5. Search by Category: Electronics");
    auto elec = inv.searchByCategory("Electronics");
    inv.printTable(elec);

    sep("6. Low Stock Alert (qty <= 5)");
    auto low = inv.lowStock(5);
    cout << low.size() << " item(s) with low stock:\n";
    inv.printTable(low);

    sep("7. Restock Ballpoint Pens (+50)");
    inv.updateQuantity(i6.id, 50);
    auto it = inv.findById(i6.id);
    cout << it->name << " -> new qty: " << it->quantity << "\n";

    sep("8. Sell 2 Ergonomic Chairs (-2)");
    inv.updateQuantity(i7.id, -2);
    it = inv.findById(i7.id);
    cout << it->name << " -> new qty: " << it->quantity << "\n";

    sep("9. Update price of USB-C Hub");
    inv.updatePrice(i2.id, 44.99);
    it = inv.findById(i2.id);
    cout << it->name << " -> new price: $" << fixed << setprecision(2) << it->price << "\n";

    sep("10. Remove HDMI Cable");
    bool removed = inv.remove(i4.id);
    cout << "Removed: " << (removed ? "Yes" : "No") << "\n";

    sep("11. Error Handling");
    try { inv.updateQuantity(i7.id, -99); }
    catch (const runtime_error& e) { cout << "Caught: " << e.what() << "\n"; }
    try { inv.add("Test", "X", -1, 9.99); }
    catch (const invalid_argument& e) { cout << "Caught: " << e.what() << "\n"; }

    sep("12. Save to File & Reload");
    inv.save();

    Inventory inv2(FILE);
    cout << "Reloaded inventory has " << inv2.size() << " items.\n";
    inv2.printTable();
    inv2.printSummary();

    return 0;
}