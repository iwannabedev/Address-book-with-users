#include <iostream>
#include <conio.h>

using namespace std;

int main(){
    system("chcp 1250");
    char choice = '0';

    while (true) {
        system("cls");
        cout << ">>MENU G��WNE<<\n";
        cout << "1. Logowanie\n";
        cout << "2. Rejestracja\n";
        cout << "9. Zamknij program\n\n";

        choice = getch();

        switch (choice) {
            case '1':   break;
            case '2':   break;
            case '9':   exit(0);
                        break;
            default:    cout << '\n' << "Niepoprawny wyb�r. Spr�buj ponownie." << '\n';
                        system("pause");
        }

    }
}
