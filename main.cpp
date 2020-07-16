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

struct PhoneBook {
    short contactID, userID;
    string firstName, lastName, phoneNo, email, address;
};

short logInUser(vector<User> &users);
void registerUser(vector<User> &users, short &numberOfUsers);
void updateUsersDatabase(vector<User> &users);
short importUsersDatabase(vector<User> &users, const char &DELIMITER);
User splitLineOfText(vector<User> &users, string stringToSplit, const char &DELIMITER);
unsigned short importContactsDatabaseOfLoggedOnUser(vector<PhoneBook> &contacts, short &idOfLoggedOnUser, const char &DELIMITER);
void addContact(vector<PhoneBook> &contacts, unsigned short &lastContactIDinDB, short &idOfLoggedOnUser);
void updateContactsDatabase(vector<PhoneBook> &contacts, const char &DELIMITER);
void findContacts(vector<PhoneBook> &contacts, unsigned short &lastContactIDinDB, bool &searchModeSwitch);
void displayContacts(vector<PhoneBook> &contacts);
void editContact(vector<PhoneBook> &contacts, unsigned short &lastContactIDinDB);
void changeUsersPassword(vector<User> &users, short &idOfLoggedOnUser);

int main(){
    system("chcp 1250");
    system("cls");

    char choice = '0';
    vector<User> users;
    vector<PhoneBook> contacts;
    short numberOfUsers = 0;
    unsigned short lastContactIDinDB = 0;
    const char DELIMITER = '|';
    short idOfLoggedOnUser = 0;

    numberOfUsers = importUsersDatabase(users, DELIMITER);

    while (true) {
        if (idOfLoggedOnUser == 0) {
            system("cls");
            cout << ">>MENU GLÓWNE<<\n";
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
            lastContactIDinDB = importContactsDatabaseOfLoggedOnUser(contacts, idOfLoggedOnUser, DELIMITER);
            bool searchModeSwitch = false, FirstNameSearch = true, LastNameSearch = false;

            system("cls");
            cout << ">>KSIY—KA ADRESOWA<<\n";
            cout << "1. Dodaj adresata\n";
            cout << "2. Wyszukaj po imieniu\n";
            cout << "3. Wyszukaj po nazwisku\n";
            cout << "4. Wyswietl wszystkich adresatow\n";
            cout << "5. Usun adresata\n";
            cout << "6. Edytuj adresata\n";
            cout << "7. Zmien has3o\n";
            cout << "9. Wyloguj sie\n";

            choice = getch();

            switch (choice) {
                case '1':   addContact(contacts, lastContactIDinDB, idOfLoggedOnUser);
                            updateContactsDatabase(contacts, DELIMITER);
                            break;
                case '2':   searchModeSwitch = FirstNameSearch;
                            findContacts(contacts, lastContactIDinDB, searchModeSwitch);
                            break;
                case '3':   searchModeSwitch = LastNameSearch;
                            findContacts(contacts, lastContactIDinDB, searchModeSwitch);
                            break;
                case '4':   displayContacts(contacts);
                            system("pause");
                            break;
                case '5':   break;
                case '6':   editContact(contacts, lastContactIDinDB);
                            break;
                case '7':   changeUsersPassword(users, idOfLoggedOnUser);
                            break;
                case '9':   idOfLoggedOnUser = 0;
                            break;
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
                cout << "Podaj haslo (ilooa pozosta3ych prób: " << 3-numberOfAttempts << "): " ;
                cin >> typedInPassword;

                if (itr->password == typedInPassword) {
                    cout << "Zalogowa3eo sie. Otwieram Twoj1 Ksi1?ke Adresow1..." << endl;
                    Sleep (1000);
                    return itr->id;
                }
            }

            cout << "Poda3es 3 razy b3edne has3o. Poczekaj 5 sekund przed kolejn1 prób1.\n\n";
            Sleep(5000);
            return 0;
        }
    }

    cout << "U?ytkownik " << typedInLogin << " nie istnieje!\n";
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

    cout << "Podaj has3o: ";
    cin >> registeredUser.password;

    users.emplace_back(registeredUser);

    cout << "Rejestracja zakonczona" << endl;
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
        cout << "\nBrak pliku z baz1 u?ytkowników! \n\n";
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

unsigned short importContactsDatabaseOfLoggedOnUser(vector<PhoneBook> &contacts, short &idOfLoggedOnUser, const char &DELIMITER) {
    fstream dbFile;
    dbFile.open("Adresaci.txt", ios::in);
    string singleValueFromLineOfText;
    vector<string> splittedStrings;
    string lineOfText;
    PhoneBook person;
    unsigned short lastContactIDinDB;

    if (!dbFile.good()) {
        system("cls");
        cout << "\nBrak pliku z baz¹ kontaktów!\n\n";
        system("pause");
    }

    contacts.clear();

    while (getline(dbFile, lineOfText)) {
        stringstream ss(lineOfText);

        while (getline(ss, singleValueFromLineOfText, DELIMITER)) {
            splittedStrings.emplace_back(singleValueFromLineOfText);
        }

        //if userID == idOfLoggedOnUser
        if (stoi(splittedStrings.at(1)) == idOfLoggedOnUser) {
            person.contactID = stoi(splittedStrings.at(0));
            person.userID = stoi(splittedStrings.at(1));
            person.firstName = splittedStrings.at(2);
            person.lastName = splittedStrings.at(3);
            person.phoneNo = splittedStrings.at(4);
            person.email = splittedStrings.at(5);
            person.address = splittedStrings.at(6);
            contacts.emplace_back(person);
        }

        lastContactIDinDB = stoi(splittedStrings.at(0));
        splittedStrings.clear();
    }

    dbFile.close();
    return lastContactIDinDB;
}

void addContact(vector<PhoneBook> &contacts, unsigned short &lastContactIDinDB, short &idOfLoggedOnUser) {
    PhoneBook person;

    system("cls");
    person.contactID = lastContactIDinDB + 1;
    lastContactIDinDB++;
    person.userID = idOfLoggedOnUser;
    cout << "Podaj imie: ";
    cin >> person.firstName;
    cout << "Podaj nazwisko: ";
    cin >> person.lastName;
    cout << "Podaj nr telefonu: ";
    cin >> person.phoneNo;
    cout << "Podaj adres email: ";
    cin >> person.email;
    cout << "Podaj pelny adres (ulica [nr], kod pocztowy, miasto): ";
    cin.sync();
    getline(cin, person.address);

    contacts.emplace_back(person);
}

void updateContactsDatabase(vector<PhoneBook> &contacts, const char &DELIMITER) {
    vector<PhoneBook>::iterator itr = contacts.begin(), lastContactPosition = contacts.end();
    fstream dbFile, dbTempFile;
    dbFile.open("Adresaci.txt", ios::in);
    dbTempFile.open("Adresaci_tymczasowy.txt", ios::out);
    string lineOfText;
    string singleValueFromLineOfText;
    vector<string> splittedStrings;
    PhoneBook person;
    lastContactPosition--;

    while (getline(dbFile, lineOfText)) {
        stringstream ss(lineOfText);

        while (getline(ss, singleValueFromLineOfText, DELIMITER)) {
           splittedStrings.emplace_back(singleValueFromLineOfText);
        }

        person.contactID = stoi(splittedStrings.at(0));
        person.userID = stoi(splittedStrings.at(1));
        person.firstName = splittedStrings.at(2);
        person.lastName = splittedStrings.at(3);
        person.phoneNo = splittedStrings.at(4);
        person.email = splittedStrings.at(5);
        person.address = splittedStrings.at(6);

        if (person.contactID == lastContactPosition->contactID) {
            dbTempFile << lastContactPosition->contactID << '|' << lastContactPosition->userID << '|' << lastContactPosition->firstName
                << '|' << lastContactPosition->lastName << '|' << lastContactPosition->phoneNo << '|' << lastContactPosition->email
                << '|' << lastContactPosition->address << '|' << '\n';
        } else {
            dbTempFile << lineOfText << '\n';
        }
    }

    if (person.contactID < lastContactPosition->contactID) {
        dbTempFile << lastContactPosition->contactID << '|' << lastContactPosition->userID << '|' << lastContactPosition->firstName
        << '|' << lastContactPosition->lastName << '|' << lastContactPosition->phoneNo << '|' << lastContactPosition->email
        << '|' << lastContactPosition->address << '|' << '\n';
    }

    dbFile.close();
    remove("Adresaci.txt");
    dbTempFile.close();
    char oldName[] = "Adresaci_tymczasowy.txt";
    char newName[] = "Adresaci.txt";
    rename(oldName, newName);
}

void findContacts(vector<PhoneBook> &contacts, unsigned short &lastContactIDinDB, bool &searchModeSwitch) {
    bool contactFound = false;
    vector<PhoneBook>::iterator itr = contacts.begin(), lastContactPosition = contacts.end();
    string newValue = "", oldValue = "";

    system("cls");

    //search by firstName
    if (searchModeSwitch == true) {
        cout << "Podaj imie do wyszukania: ";
    }
    //search by lastName
    else {
        cout << "Podaj nazwisko do wyszukania: ";
    }

    cin >> newValue;

    for (itr; itr != lastContactPosition; ++itr) {
        oldValue = (searchModeSwitch) ? itr->firstName : itr->lastName;

        if (oldValue == newValue) {
            cout << itr->contactID << " | " << itr->userID << " | " << itr->firstName << " | " << itr->lastName << " | "
                 << itr->phoneNo << " | " << itr->email <<  " | " << itr->address << " | " << '\n';
            contactFound = true;
        }
    }

    if (!contactFound) {
        cout << '\n' << "Zaden kontakt nie spelnia kryterium wyszukiwania." << '\n' << '\n';
        system("pause");
    } else {
        cout << '\n';
        system("pause");
    }
}

void displayContacts(vector<PhoneBook> &contacts) {
    vector<PhoneBook>::iterator itr = contacts.begin(), lastContactPosition = contacts.end();

    system("cls");

    for (itr; itr != lastContactPosition; ++itr ) {
        cout << itr->contactID << " | " << itr->userID << " | "  << itr->firstName << " | " << itr->lastName << " | "
             << itr->phoneNo << " | " << itr->email <<  " | " << itr->address << " | " << '\n';
    }
    cout << '\n';
}

void editContact(vector<PhoneBook> &contacts, unsigned short &lastContactIDinDB) {
    char choice = ' ';
    short contactIDToEdit = 0;
    bool IDSearchSuccessful = false;
    vector<PhoneBook>::iterator itr = contacts.begin(), lastContactPosition = contacts.end();

    displayContacts(contacts);

    cout << "Podaj ID kontaktu, ktory chcesz edytowac: ";
    cin >> contactIDToEdit;

    system("cls");
    cout << "Ktora informacje chcesz zmienic?: \n";
    cout << "1 - imie\n";
    cout << "2 - nazwisko\n";
    cout << "3 - numer telefonu\n";
    cout << "4 - email\n";
    cout << "5 - adres\n";
    cout << "6 - powrot do menu\n\n";
    choice = getch();
    cout << "Twoj wybor: " << choice << '\n';

    for (itr; itr != lastContactPosition; ++itr) {
        if (itr->contactID == contactIDToEdit) {
            IDSearchSuccessful = true;
            switch (choice) {
                case '1':   cout << "Podaj nowe imie: ";
                            cin >> itr->firstName;

                            break;
                case '2':   cout << "Podaj nowe nazwisko: ";
                            cin >> itr->lastName;
                            break;
                case '3':   cout << "Podaj nowy numer telefonu: ";
                            cin >> itr->phoneNo;
                            break;
                case '4':   cout << "Podaj nowy email: ";
                            cin >> itr->email;
                            break;
                case '5':   cout << "Podaj nowe adres: ";
                            cin >> itr->address;
                            break;
                case '6':   break;
                default:    cout << "\nNiepoprawny wybor. Spróbuj ponownie.\n";
                            system("pause");
            }
        }
    }

    if (!IDSearchSuccessful) {
        cout << "\nBrak kontaktu o podanym ID.\n";
        system("pause");
    }

//    updateContactsDatabase
}

void changeUsersPassword(vector<User> &users, short &idOfLoggedOnUser){
    string password1, password2;
    short indexOfLoggedOnUser = idOfLoggedOnUser - 1;
    vector<User>::iterator itr = users.begin(), lastUserPosition = users.end();
    fstream dbFile;
    dbFile.open("Uzytkownicy.txt", ios::out);

    cout << "\nPodaj nowe has³o: ";
    cin >> password1;
    cout << "Podaj ponownie nowe has³o: ";
    cin >> password2;

    if (password1 == password2) {
        users.at(indexOfLoggedOnUser).password = password1;

        for (itr; itr != lastUserPosition; ++itr) {
            dbFile << itr->id << '|' << itr->login << '|' << itr->password<< '|' << '\n';
        }

        cout << "\nHas³o zosta³o pomyœlnie zmienione!\n";
        system("pause");
    } else {
        cout << "Wprowadzone has³a s¹ od siebie ró¿ne! Zmiana has³a zakoñczona niepowodzeniem.\n";
        system("pause");
    }
    dbFile.close();
}
