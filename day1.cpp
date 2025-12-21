#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>   // _getch()
#include <windows.h> // Sleep(), Beep()

using namespace std;

void diceFace(int n) {
    system("cls");
    cout << "\n   ---------\n";

    switch (n) {
        case 1:
            cout << "  |         |\n";
            cout << "  |    o    |\n";
            cout << "  |         |\n";
            break;
        case 2:
            cout << "  | o       |\n";
            cout << "  |         |\n";
            cout << "  |       o |\n";
            break;
        case 3:
            cout << "  | o       |\n";
            cout << "  |    o    |\n";
            cout << "  |       o |\n";
            break;
        case 4:
            cout << "  | o     o |\n";
            cout << "  |         |\n";
            cout << "  | o     o |\n";
            break;
        case 5:
            cout << "  | o     o |\n";
            cout << "  |    o    |\n";
            cout << "  | o     o |\n";
            break;
        case 6:
            cout << "  | o     o |\n";
            cout << "  | o     o |\n";
            cout << "  | o     o |\n";
            break;
    }

    cout << "   ---------\n";
}

int main() {
    srand(time(0));

    cout << " DIGITAL DICE ROLLER \n";
    cout << "Press any key to roll the dice...\n";
    cout << "Press ESC to exit.\n";

    while (true) {
        char key = _getch();
        if (key == 27) break; // ESC key

        for (int i = 0; i < 6; i++) {
            diceFace(rand() % 6 + 1);
            Beep(800 + i * 100, 80);
            Sleep(100);
        }

        int dice = rand() % 6 + 1;
        diceFace(dice);
        Beep(1000, 200);

        cout << "\nRolled: " << dice << endl;
        cout << "Press any key to roll again...\n";
    }

    return 0;
}
