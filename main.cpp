#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct User {
    short id;
    string login, password;

};

void registerUser(vector<User> &users, short &numberOfUsers);
void updateUsersDatabase(vector<User> &users);
short importUsersDatabase(vector<User> &users, const char &DELIMITER);
User splitLineOfText(vector<User> &users, string stringToSplit, const char &DELIMITER);

int main(){
    system("chcp 1250");
    system("cls");

    char choice = '0';
    vector<User> users;
    short numberOfUsers = 0;
    const char DELIMITER = '|';

    numberOfUsers = importUsersDatabase(users, DELIMITER);

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

short importUsersDatabase(vector<User> &users, const char &DELIMITER) {
    fstream usersDatabase;
    usersDatabase.open("Uzytkownicy.txt", ios::in);
    short lastUserID;

    if (!usersDatabase.good()) {
        system("cls");
        cout << "\nBrak pliku z baz¹ u¿ytkowników! \n\n";
        system("pause");
    }

    //Lines in text file count from 1, NOT from 0!
    unsigned short lineOfTextCount = 1;
    string lineOfText;

    while (getline(usersDatabase, lineOfText)) {
        users.emplace_back(splitLineOfText(users, lineOfText, DELIMITER));
        lineOfTextCount++;
    }

    usersDatabase.close();

    lastUserID = lineOfTextCount - 1;
    return lastUserID;
}

User splitLineOfText(vector<User> &users, string lineOfText, const char& DELIMITER) {
    stringstream ss(lineOfText);
    string singleValueFromLineOfText;
    vector<string> splittedStrings;
    User registeredUser;

    while (getline(ss, singleValueFromLineOfText, DELIMITER)) {
       splittedStrings.emplace_back(singleValueFromLineOfText);
    }

    registeredUser.id = static_cast<short>(stoi(splittedStrings.at(0)));
    registeredUser.login = splittedStrings.at(1);
    registeredUser.password = splittedStrings.at(2);

    return registeredUser;
}
