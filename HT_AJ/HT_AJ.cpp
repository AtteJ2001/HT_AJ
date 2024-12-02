//Atte Jaakkola Harjoitustyo
//Ohjelmassa ei toiminut setlocale joten on jouduttu menem‰‰n ‰=a jne tyylill‰
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

// Maaritellaan vakioita huonehintoihin ja huonemaariin
const int HuoneMAX = 300;
const int huoneHinta1 = 100;
const int huoneHinta2 = 150;
const int huoneMin = 40;
const int HuoneMAX_Limit = 300;

// Rakenne huoneen tiedoille
struct huone {
    int number;
    bool isSingle;
    bool isReserved;
    int price;
    int reservationNumber;
    string reserverName;
};

// Funktioiden esittelyt
int generatehuoneCount();
int generatehuonePrice(int basePrice);
int generateReservationNumber();
int calculateDiscount();
bool checkhuoneAvailability(const vector<huone>& huoneet, int huoneNumber);
void reservehuone(vector<huone>& huoneet, int huoneNumber, const string& name, int nights);
void searchReservationByNumber(const vector<huone>& huoneet, int reservationNumber);
void searchReservationByName(const vector<huone>& huoneet, const string& name);
void displayMenu();
int getUserInputInt();

int main() {
    
    srand(static_cast<unsigned int>(time(0))); // Asetetaan satunnaislukugeneraattori

    // Arvotaan hotellin huonemaara
    int huoneCount = generatehuoneCount();

    // Maaritellaan hotellin huoneet
    vector<huone> huoneet(huoneCount);
    for (int i = 0; i < huoneCount; ++i) {
        huoneet[i].number = i + 1;
        huoneet[i].isSingle = i < huoneCount / 2; // Ensimmainen puolikas on 1hh, toinen puolikas 2hh
        huoneet[i].isReserved = false;
        huoneet[i].price = huoneet[i].isSingle ? huoneHinta1 : huoneHinta2;
    }

    bool exitProgram = false;
    while (!exitProgram) {
        displayMenu();
        int choice = getUserInputInt();

        switch (choice) {
        case 1: { // Tee huonevaraus
            cout << "Anna huonetyyppi (1 = yhden hengen huone, 2 = kahden hengen huone): ";
            int huoneType = getUserInputInt();
            cout << "Anna oiden lukumaara: ";
            int nights = getUserInputInt();

            if (huoneType != 1 && huoneType != 2) {
                cout << "Virheellinen huonetyyppi.\n";
                break;
            }

            bool isSingle = (huoneType == 1);
            int availablehuoneNumber = -1;
            for (const auto& huone : huoneet) {
                if (huone.isSingle == isSingle && !huone.isReserved) {
                    availablehuoneNumber = huone.number;
                    break;
                }
            }

            if (availablehuoneNumber == -1) {
                cout << "Valitettavasti kaikki haluamasi huoneet ovat varattuja.\n";
                break;
            }

            cout << "Anna varaajan nimi: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string name;
            getline(cin, name);

            reservehuone(huoneet, availablehuoneNumber, name, nights);
            break;
        }
        case 2: { // Hae varausnumerolla
            cout << "Anna varausnumero: ";
            int reservationNumber = getUserInputInt();
            searchReservationByNumber(huoneet, reservationNumber);
            break;
        }
        case 3: { // Hae varaajan nimella
            cout << "Anna varaajan nimi: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string name;
            getline(cin, name);
            searchReservationByName(huoneet, name);
            break;
        }
        case 4: { // Poistu
            exitProgram = true;
            break;
        }
        default:
            cout << "Virheellinen valinta. Yrita uudelleen.\n";
        }
    }

    return 0;
}

// Funktiot

// Luo satunnainen huonemaara
int generatehuoneCount() {
    return huoneMin + (rand() % ((HuoneMAX_Limit - huoneMin + 1) / 2) * 2); // Parillinen maara
}

// Luo satunnainen hinta, jossa perushinta vaihtelee 80ñ100 valilla
int generatehuonePrice(int basePrice) {
    return basePrice + (rand() % 21); // +0...+20 euroa
}

// Luo satunnainen varausnumero
int generateReservationNumber() {
    return 10000 + rand() % 90000;
}

// Laskee satunnaisen alennuksen (0%, 10%, tai 20%)
int calculateDiscount() {
    int discounts[] = { 0, 10, 20 };
    return discounts[rand() % 3];
}

// Tarkistaa huoneen saatavuuden
bool checkhuoneAvailability(const vector<huone>& huoneet, int huoneNumber) {
    for (const auto& huone : huoneet) {
        if (huone.number == huoneNumber) {
            return !huone.isReserved;
        }
    }
    return false;
}

// Huonevaraus
void reservehuone(vector<huone>& huoneet, int huoneNumber, const string& name, int nights) {
    for (auto& huone : huoneet) {
        if (huone.number == huoneNumber && !huone.isReserved) {
            huone.isReserved = true;
            huone.reserverName = name;
            huone.reservationNumber = generateReservationNumber();

            int discount = calculateDiscount();
            double finalPrice = huone.price * nights * (1 - discount / 100.0);

            cout << "Huone " << huoneNumber << " on varattu " << name << " nimelle.\n";
            cout << "oiden maara: " << nights << ", Hinta per yo: " << huone.price << " Euroa\n";
            cout << "Alennus: " << discount << "%\n";
            cout << "Loppusumma: " << finalPrice << " Euroa\n";
            return;
        }
    }
    cout << "Virhe: Huonetta ei voitu varata.\n";
}

// Hakee varauksen numerolla
void searchReservationByNumber(const vector<huone>& huoneet, int reservationNumber) {
    for (const auto& huone : huoneet) {
        if (huone.isReserved && huone.reservationNumber == reservationNumber) {
            cout << "Varauksen loysi huoneesta: " << huone.number << "\n";
            cout << "Varaajan nimi: " << huone.reserverName << "\n";
            return;
        }
    }
    cout << "Varausta ei loytynyt annetulla numerolla.\n";
}

// Hakee varauksen nimella
void searchReservationByName(const vector<huone>& huoneet, const string& name) {
    for (const auto& huone : huoneet) {
        if (huone.isReserved && huone.reserverName == name) {
            cout << "Varauksen loysi huoneesta: " << huone.number << "\n";
            cout << "Varausnumero: " << huone.reservationNumber << "\n";
            return;
        }
    }
    cout << "Varausta ei loytynyt annetulla nimella.\n";
}

// Nayttaa ohjelman valikon
void displayMenu() {
    cout << "\n=== Hotellivarauksen hallinta  ===\n";
    cout << "1. Tee huonevaraus\n";
    cout << "2. Hae varausnumerolla\n";
    cout << "3. Hae varaajan nimella\n";
    cout << "4. Poistu\n";
    cout << "Valitse toiminto: ";
    cout.flush();
}

// Kayttajan syotteen lukeminen kokonaislukuna ja virheiden kasittely
int getUserInputInt() {
    int input;
    while (!(cin >> input)) {
        cout << "Virheellinen syote. Anna kokonaisluku: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return input;
}
