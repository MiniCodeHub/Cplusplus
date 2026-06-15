#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Job {
    char id;
    int deadline;
    int profit;
};

bool compare(Job a, Job b) {
    return a.profit > b.profit;
}

int main() {

    vector<Job> jobs = {
        {'A', 2, 100},
        {'B', 1, 19},
        {'C', 2, 27},
        {'D', 1, 25},
        {'E', 3, 15}
    };

    sort(
        jobs.begin(),
        jobs.end(),
        compare
    );

    int maxDeadline = 0;

    for (auto &job : jobs) {
        maxDeadline =
            max(
                maxDeadline,
                job.deadline
            );
    }

    vector<char> schedule(
        maxDeadline,
        '-'
    );

    vector<bool> slot(
        maxDeadline,
        false
    );

    int totalProfit = 0;
    int jobsDone = 0;

    for (auto &job : jobs) {

        for (
            int j = job.deadline - 1;
            j >= 0;
            j--
        ) {

            if (!slot[j]) {

                slot[j] = true;

                schedule[j] = job.id;

                totalProfit += job.profit;

                jobsDone++;

                break;
            }
        }
    }

    cout << "Scheduled Jobs:\n";

    for (char job : schedule) {

        if (job != '-') {
            cout << job << " ";
        }
    }

    cout << "\n\n";

    cout << "Jobs Completed: "
         << jobsDone
         << endl;

    cout << "Maximum Profit: "
         << totalProfit
         << endl;

    return 0;
}