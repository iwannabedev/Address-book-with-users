#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>

using namespace std;

struct User {
    short id;
    string login, password;

};

void registerUser(vector<User> &users, short &numberOfUsers);
void updateUsersDatabase(vector<User> &users);

int main(){
    system("chcp 1250");
    char choice = '0';
    vector<User> users;
    short numberOfUsers = 0;

    while (true) {
        system("cls");
        cout << ">>MENU G£ÓWNE<<\n";
        cout << "1. Logowanie\n";
        cout << "2. Rejestracja\n";
        cout << "9. Zamknij program\n\n";

        choice = getch();

        switch (choice) {
            case '1':   break;
            case '2':   registerUser(users, numberOfUsers);
                        updateUsersDatabase(users);
                        break;
            case '9':   exit(0);
                        break;
            default:    cout << '\n' << "Niepoprawny wybór. Spróbuj ponownie." << '\n';
                        system("pause");
        }

    }
}

void registerUser(vector<User> &users, short &numberOfUsers) {
    User registeredUser;
    vector<User>::iterator itr = users.begin(), lastUserPosition = users.end();
    string userName;
    bool userExists = false;
    cout << "Podaj nazwe uzytkownika: ";
    cin >> userName;

    userExists = (numberOfUsers) ? true:false;

    while (userExists) {
        for (itr; itr != lastUserPosition; ++itr) {
            if (userName == itr->login) {
                cout << "Taki uzytkownik istnieje. Wpisz inna nazwe uzytkownika: ";
                cin >> userName;
                itr = users.begin();
            }
        }
        userExists = false;
    }

    registeredUser.id = ++numberOfUsers;
    registeredUser.login = userName;

    cout << "Podaj has³o: ";
    cin >> registeredUser.password;

    users.emplace_back(registeredUser);

    cout << "Rejestracja zakoñczona" << endl;
    system("pause");
}

void updateUsersDatabase(vector<User> &users) {
    vector<User>::iterator itr = users.begin(), lastUserPosition = users.end();
    fstream usersDatabase;
    usersDatabase.open("Uzytkownicy.txt", ios::out);

    for (itr; itr != lastUserPosition; ++itr) {
        usersDatabase << itr->id << "|" << itr->login << "|" << itr->password << "|\n";
    }

    usersDatabase.close();
}
