#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <numeric>
using namespace std;

// -- Item -------------------------------------------------------------------
struct Item {
    string name;
    int    weight;
    int    value;
    double ratio() const { return (double)value / weight; }
};

// -- 0/1 Knapsack (bottom-up DP) --------------------------------------------
struct KnapsackResult {
    int           maxValue;
    vector<Item>  chosen;
    vector<vector<int>> dp;   // full DP table (for printing)
};

KnapsackResult knapsack01(const vector<Item>& items, int capacity) {
    int n = (int)items.size();
    // dp[i][w] = max value using first i items with capacity w
    vector<vector<int>> dp(n+1, vector<int>(capacity+1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i-1][w];  // don't take item i
            if (items[i-1].weight <= w)
                dp[i][w] = max(dp[i][w], dp[i-1][w - items[i-1].weight] + items[i-1].value);
        }
    }

    // Backtrack to find chosen items
    vector<Item> chosen;
    int w = capacity;
    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            chosen.push_back(items[i-1]);
            w -= items[i-1].weight;
        }
    }
    reverse(chosen.begin(), chosen.end());

    return { dp[n][capacity], chosen, dp };
}

// -- Unbounded Knapsack (items can be used multiple times) ------------------
pair<int, vector<pair<Item,int>>> unboundedKnapsack(const vector<Item>& items, int capacity) {
    vector<int>      dp(capacity+1, 0);
    vector<int>      from(capacity+1, -1);

    for (int w = 1; w <= capacity; w++) {
        for (int i = 0; i < (int)items.size(); i++) {
            if (items[i].weight <= w && dp[w - items[i].weight] + items[i].value > dp[w]) {
                dp[w] = dp[w - items[i].weight] + items[i].value;
                from[w] = i;
            }
        }
    }

    // Reconstruct
    vector<pair<Item,int>> chosen;
    vector<int> counts(items.size(), 0);
    int w = capacity;
    while (w > 0 && from[w] != -1) {
        counts[from[w]]++;
        w -= items[from[w]].weight;
    }
    for (int i = 0; i < (int)items.size(); i++)
        if (counts[i] > 0) chosen.push_back({items[i], counts[i]});

    return { dp[capacity], chosen };
}

// -- Fractional Knapsack (Greedy) -------------------------------------------
double fractionalKnapsack(vector<Item> items, int capacity) {
    sort(items.begin(), items.end(), [](const Item& a, const Item& b){
        return a.ratio() > b.ratio();
    });
    double totalValue = 0;
    int    remaining  = capacity;
    cout << "  Greedy selection:\n";
    for (auto& item : items) {
        if (remaining <= 0) break;
        int take = min(item.weight, remaining);
        double val = take * item.ratio();
        totalValue += val;
        remaining  -= take;
        if (take < item.weight)
            cout << "  [PARTIAL] " << item.name << " (" << take << "/" << item.weight
                 << "kg, value=" << fixed << setprecision(2) << val << ")\n";
        else
            cout << "  [FULL]    " << item.name << " (value=" << item.value << ")\n";
    }
    return totalValue;
}

// -- Print helpers ----------------------------------------------------------
void printItems(const vector<Item>& items) {
    cout << left << setw(18) << "Name" << setw(8) << "Weight" << setw(8) << "Value" << "Ratio\n";
    cout << string(42, '-') << "\n";
    for (const auto& it : items)
        cout << left << setw(18) << it.name
             << setw(8) << it.weight
             << setw(8) << it.value
             << fixed << setprecision(2) << it.ratio() << "\n";
}

void printResult(const KnapsackResult& res, int capacity) {
    int totalW = 0;
    cout << "\nChosen items:\n";
    cout << left << setw(18) << "Item" << setw(8) << "Weight" << "Value\n";
    cout << string(34, '-') << "\n";
    for (const auto& it : res.chosen) {
        cout << left << setw(18) << it.name << setw(8) << it.weight << it.value << "\n";
        totalW += it.weight;
    }
    cout << string(34, '-') << "\n";
    cout << left << setw(18) << "Total" << setw(8) << totalW << res.maxValue << "\n";
    cout << "Capacity used: " << totalW << " / " << capacity << "\n";
}

void printDPTable(const vector<vector<int>>& dp, const vector<Item>& items, int maxW, int maxShow = 12) {
    int W = min(maxW, maxShow);
    cout << "\nDP Table (first " << W << " cols):\n";
    cout << setw(14) << " ";
    for (int w = 0; w <= W; w++) cout << setw(5) << w;
    cout << "\n" << string(14 + (W+1)*5, '-') << "\n";
    for (int i = 0; i <= (int)items.size() && i <= 6; i++) {
        string rowLabel = (i == 0) ? "- (empty)" : items[i-1].name;
        cout << setw(14) << rowLabel.substr(0,13);
        for (int w = 0; w <= W; w++) cout << setw(5) << dp[i][w];
        cout << "\n";
    }
}

void sep(const string& t) {
    cout << "\n" << string(54, '=') << "\n " << t << "\n" << string(54, '=') << "\n";
}

int main() {
    cout << "=== Knapsack Problem (Dynamic Programming) ===\n";

    // -- Demo 1: Classic textbook example ----------------------------------
    sep("1. Classic 0/1 Knapsack");
    vector<Item> items1 = {
        {"Laptop",    3, 4},
        {"Phone",     1, 3},
        {"Book",      2, 1},
        {"Headphones",2, 2},
        {"Camera",    4, 5},
        {"Tablet",    3, 3},
    };
    int cap1 = 7;
    cout << "Items:\n"; printItems(items1);
    cout << "\nCapacity: " << cap1 << "\n";
    auto res1 = knapsack01(items1, cap1);
    cout << "Max value: " << res1.maxValue << "\n";
    printResult(res1, cap1);
    printDPTable(res1.dp, items1, cap1);

    // -- Demo 2: Real-world scenario ----------------------------------------
    sep("2. Backpacker's Gear Selection (15kg limit)");
    vector<Item> gear = {
        {"Tent",          4, 10},
        {"Sleeping bag",  3, 8 },
        {"Cooking kit",   2, 5 },
        {"First aid kit", 1, 7 },
        {"Water filter",  1, 9 },
        {"Rain jacket",   2, 6 },
        {"Extra clothes", 2, 4 },
        {"Camera",        3, 7 },
        {"Books",         4, 2 },
        {"Snacks",        3, 8 },
        {"GPS device",    1, 6 },
    };
    int capGear = 15;
    cout << "Items:\n"; printItems(gear);
    cout << "\nCapacity: " << capGear << "kg\n";
    auto resGear = knapsack01(gear, capGear);
    cout << "Max value: " << resGear.maxValue << "\n";
    printResult(resGear, capGear);

    // -- Demo 3: Unbounded Knapsack -----------------------------------------
    sep("3. Unbounded Knapsack (repeat items allowed)");
    vector<Item> coins = {
        {"Gold coin",   5, 30},
        {"Silver coin", 3, 14},
        {"Bronze coin", 1, 4 },
        {"Gem",         7, 50},
    };
    int capU = 20;
    cout << "Items:\n"; printItems(coins);
    cout << "\nCapacity: " << capU << "\n";
    auto [maxU, chosenU] = unboundedKnapsack(coins, capU);
    cout << "Max value: " << maxU << "\nChosen:\n";
    for (auto& [item, cnt] : chosenU)
        cout << "  " << item.name << " x" << cnt << " (total value=" << item.value*cnt << ")\n";

    // -- Demo 4: Fractional Knapsack (Greedy) -------------------------------
    sep("4. Fractional Knapsack (greedy, divisible items)");
    vector<Item> frac = {
        {"Gold dust",  10, 60},
        {"Silver ore", 20, 100},
        {"Bronze ore", 30, 120},
    };
    int capF = 50;
    cout << "Items:\n"; printItems(frac);
    cout << "\nCapacity: " << capF << "\n";
    double maxF = fractionalKnapsack(frac, capF);
    cout << "Max value (fractional): " << fixed << setprecision(2) << maxF << "\n";

    // -- Demo 5: Edge cases -------------------------------------------------
    sep("5. Edge Cases");
    // Single item that fits
    vector<Item> e1 = {{"Widget", 5, 100}};
    auto r1 = knapsack01(e1, 5);
    cout << "Single item that fits: value=" << r1.maxValue << " (expected 100)\n";

    // Single item too heavy
    auto r2 = knapsack01(e1, 4);
    cout << "Single item too heavy: value=" << r2.maxValue << " (expected 0)\n";

    // All items too heavy
    vector<Item> e3 = {{"Heavy", 100, 999}, {"Heavier", 200, 1999}};
    auto r3 = knapsack01(e3, 50);
    cout << "All too heavy: value=" << r3.maxValue << " (expected 0)\n";

    // -- Demo 6: Complexity -------------------------------------------------
    sep("6. Complexity Summary");
    cout << left << setw(22) << "Variant"
                 << setw(18) << "Time"
                 << setw(12) << "Space" << "Notes\n";
    cout << string(64, '-') << "\n";
    for (auto& row : vector<vector<string>>{
        {"0/1 Knapsack",   "O(n * W)",    "O(n * W)", "Bottom-up DP"},
        {"0/1 (optimised)","O(n * W)",    "O(W)",     "1D rolling array"},
        {"Unbounded",      "O(n * W)",    "O(W)",     "Repeat items OK"},
        {"Fractional",     "O(n log n)",  "O(1)",     "Greedy on ratio"},
    }) cout << setw(22)<<row[0]<<setw(18)<<row[1]<<setw(12)<<row[2]<<row[3]<<"\n";

    return 0;
}