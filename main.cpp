#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <windows.h>

using namespace std;

struct User {
    short id;
    string login, password;

};

short logInUser(vector<User> &users);
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
    short idOfLoggedOnUser = 0;

    numberOfUsers = importUsersDatabase(users, DELIMITER);

    while (true) {
        if (idOfLoggedOnUser == 0) {
            system("cls");
            cout << ">>MENU G£ÓWNE<<\n";
            cout << "1. Logowanie\n";
            cout << "2. Rejestracja\n";
            cout << "9. Zamknij program\n\n";

            choice = getch();

            switch (choice) {
                case '1':   idOfLoggedOnUser = logInUser(users);
                            break;
                case '2':   registerUser(users, numberOfUsers);
                            updateUsersDatabase(users);
                            break;
                case '9':   exit(0);
                            break;
                default:    cout << "\nNiepoprawny wybór. Spróbuj ponownie.\n";
                            system("pause");
            }
        } else {
            system("cls");
            cout << ">>KSI¥¯KA ADRESOWA<<\n";
            cout << "1. Dodaj adresata\n";
            cout << "2. Wyszukaj po imieniu\n";
            cout << "3. Wyszukaj po nazwisku\n";
            cout << "4. Wyswietl wszystkich adresatow\n";
            cout << "5. Usun adresata\n";
            cout << "6. Edytuj adresata\n";
            cout << "7. Zmieñ has³o\n";
            cout << "9. Wyloguj siê\n";

            choice = getch();

            switch (choice) {
                case '1':   break;
                case '2':   break;
                case '3':   break;
                case '4':   break;
                case '5':   break;
                case '6':   break;
                case '7':   break;
                case '9':   break;
                default:    cout << "\nNiepoprawny wybór. Spróbuj ponownie.\n";
                            system("pause");
            }
        }
    }
    return 0;
}

short logInUser(vector<User> &users){
    vector<User>::iterator itr = users.begin(), lastUserPosition = users.end();
    string typedInLogin, typedInPassword;
    short numberOfAttempts = 3;

    cout << "Podaj login: ";
    cin >> typedInLogin;

    for (itr; itr != lastUserPosition; ++itr) {
        if (itr->login == typedInLogin) {
            for (short numberOfAttempts = 0; numberOfAttempts < 3; numberOfAttempts++) {
                cout << "Podaj haslo (iloœæ pozosta³ych prób: " << 3-numberOfAttempts << "): " ;
                cin >> typedInPassword;

                if (itr->password == typedInPassword) {
                    cout << "Zalogowa³eœ siê. Otwieram Twoj¹ Ksi¹¿kê Adresow¹..." << endl;
                    Sleep (1000);
                    return itr->id;
                }
            }

            cout << "Poda³es 3 razy b³êdne has³o. Poczekaj 5 sekund przed kolejn¹ prób¹.\n\n";
            Sleep(5000);
            return 0;
        }
    }

    cout << "U¿ytkownik " << typedInLogin << " nie istnieje!\n";
    system("pause");
    return 0;
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
