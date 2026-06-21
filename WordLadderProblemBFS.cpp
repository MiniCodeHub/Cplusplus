#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <string>

using namespace std;

int wordLadderLength(
    string beginWord,
    string endWord,
    vector<string>& wordList
) {
    unordered_set<string> words(
        wordList.begin(),
        wordList.end()
    );

    if (
        words.find(endWord)
        == words.end()
    ) {
        return 0;
    }

    queue<pair<string, int>> q;

    q.push({
        beginWord,
        1
    });

    while (!q.empty()) {

        string current =
            q.front().first;

        int steps =
            q.front().second;

        q.pop();

        if (
            current == endWord
        ) {
            return steps;
        }

        for (
            int i = 0;
            i < current.size();
            i++
        ) {

            string temp =
                current;

            for (
                char ch = 'a';
                ch <= 'z';
                ch++
            ) {

                temp[i] = ch;

                if (
                    words.find(temp)
                    != words.end()
                ) {

                    q.push({
                        temp,
                        steps + 1
                    });

                    words.erase(temp);
                }
            }
        }
    }

    return 0;
}

int main() {

    string beginWord = "hit";

    string endWord = "cog";

    vector<string> wordList = {
        "hot",
        "dot",
        "dog",
        "lot",
        "log",
        "cog"
    };

    int result =
        wordLadderLength(
            beginWord,
            endWord,
            wordList
        );

    cout
        << "Begin Word: "
        << beginWord
        << endl;

    cout
        << "End Word: "
        << endWord
        << endl;

    cout
        << "\nShortest Transformation Length: "
        << result
        << endl;

    return 0;
}