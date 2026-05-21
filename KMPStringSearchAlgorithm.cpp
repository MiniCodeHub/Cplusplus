#include <iostream>
#include <vector>

using namespace std;

class KMPAlgorithm {
public:

    // Build LPS Array
    vector<int> buildLPS(string pattern) {

        int n = pattern.length();

        vector<int> lps(n, 0);

        int length = 0;
        int i = 1;

        while (i < n) {

            if (pattern[i] == pattern[length]) {

                length++;

                lps[i] = length;

                i++;
            }
            else {

                if (length != 0) {

                    length = lps[length - 1];
                }
                else {

                    lps[i] = 0;

                    i++;
                }
            }
        }

        return lps;
    }

    // KMP Search
    void search(string text, string pattern) {

        vector<int> lps = buildLPS(pattern);

        int textIndex = 0;
        int patternIndex = 0;

        cout << "Pattern Found At Indexes:\n\n";

        while (textIndex < text.length()) {

            if (
                pattern[patternIndex]
                == text[textIndex]
            ) {

                textIndex++;
                patternIndex++;
            }

            // Full match found
            if (patternIndex == pattern.length()) {

                cout
                    << textIndex - patternIndex
                    << endl;

                patternIndex =
                    lps[patternIndex - 1];
            }

            else if (
                textIndex < text.length() &&
                pattern[patternIndex]
                != text[textIndex]
            ) {

                if (patternIndex != 0) {

                    patternIndex =
                        lps[patternIndex - 1];
                }
                else {

                    textIndex++;
                }
            }
        }

        cout << endl;

        // Print LPS Array
        cout << "LPS Array:\n\n";

        for (int value : lps) {

            cout << value << " ";
        }

        cout << endl;
    }
};

int main() {

    string text =
        "ABABDABACDABABCABAB";

    string pattern =
        "ABABCABAB";

    cout << "Text:\n";
    cout << text << endl;

    cout << "\nPattern:\n";
    cout << pattern << endl << endl;

    KMPAlgorithm kmp;

    kmp.search(text, pattern);

    return 0;
}