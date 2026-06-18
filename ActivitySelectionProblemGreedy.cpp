#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Activity {
    int start;
    int finish;
};

bool compare(
    Activity a,
    Activity b
) {
    return a.finish < b.finish;
}

int main() {

    vector<Activity> activities = {
        {1, 4},
        {3, 5},
        {0, 6},
        {5, 7},
        {8, 9},
        {5, 9}
    };

    sort(
        activities.begin(),
        activities.end(),
        compare
    );

    vector<Activity> selected;

    selected.push_back(
        activities[0]
    );

    int lastFinish =
        activities[0].finish;

    for (
        int i = 1;
        i < activities.size();
        i++
    ) {

        if (
            activities[i].start >=
            lastFinish
        ) {

            selected.push_back(
                activities[i]
            );

            lastFinish =
                activities[i].finish;
        }
    }

    cout
        << "Selected Activities:\n\n";

    for (
        const auto& activity :
        selected
    ) {

        cout
            << "("
            << activity.start
            << ", "
            << activity.finish
            << ")\n";
    }

    cout
        << "\nMaximum Activities = "
        << selected.size()
        << endl;

    return 0;
}