#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

const int USERS = 2;
const int FIELDS = 3;  // 0 = číslo karty, 1 = PIN, 2 = zůstatek

// Deklarace funkcí
bool verifyAmount(int amount);
bool hasSufficientFunds(int balance, int amount);
void updateBalance(int& balance, int amount);
void printBanknotes(int amount);
void checkBalance(int balance);
void changePin(int& pin);
string formatBalance(int balance);

int main() {
    // users[i][0] = číslo karty, [i][1] = PIN, [i][2] = zůstatek
    int accounts[USERS][FIELDS] = {
        {123456, 9985,   10200},
        {789456, 1145,  230450}
    };

    int attempts = 3;
    int cardInput, pinInput;
    bool authenticated = false;
    int userIndex = -1;

    // Přihlášení – max. 3 pokusy
    while (attempts > 0 && !authenticated) {
        cout << "Zadejte 6místné číslo karty: ";
        cin  >> cardInput;
        cout << "Zadejte 4místný PIN: ";
        cin  >> pinInput;

        for (int i = 0; i < USERS; ++i) {
            if (accounts[i][0] == cardInput && accounts[i][1] == pinInput) {
                authenticated = true;
                userIndex = i;
                break;
            }
        }
        if (!authenticated) {
            --attempts;
            cout << "Neplatné přihlášení. Zbývá pokusů: " << attempts << "\n";
        }
    }
    if (!authenticated) {
        cout << "Vyčerpány všechny pokusy. Program končí.\n";
        return 0;
    }

    // Hlavní menu
    bool exitProgram = false;
    while (!exitProgram) {
        cout << "\n--- MENU ---\n"
             << "1) Vybrat hotovost\n"
             << "2) Dotaz na zůstatek\n"
             << "3) Změna PINu\n"
             << "4) Konec programu\n"
             << "Vyberte volbu: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {  // Výběr hotovosti
                int amount;
                while (true) {
                    cout << "Zadejte částku k výběru: ";
                    cin  >> amount;
                    if (!verifyAmount(amount)) {
                        cout << "Nelze vydat zadanou částku. Zkuste znovu.\n";
                        continue;
                    }
                    if (!hasSufficientFunds(accounts[userIndex][2], amount)) {
                        cout << "Nedostatečný zůstatek na účtu.\n";
                        break;
                    }
                    updateBalance(accounts[userIndex][2], amount);
                    cout << "Vydávám bankovky:\n";
                    printBanknotes(amount);
                    break;
                }
                break;
            }
            case 2:  // Dotaz na zůstatek
                checkBalance(accounts[userIndex][2]);
                break;

            case 3:  // Změna PINu
                changePin(accounts[userIndex][1]);
                break;

            case 4:  // Konec
                exitProgram = true;
                cout << "Děkujeme za použití bankomatu. Nashledanou!\n";
                break;

            default:
                cout << "Neplatná volba. Zkuste znovu.\n";
        }
    }

    return 0;
}

// Ověří, že částka je dělitelná 200 (jen 200, 1000, 2000, 5000)
bool verifyAmount(int amount) {
    return amount > 0 && amount % 200 == 0;
}

// Zkontroluje dostatek prostředků
bool hasSufficientFunds(int balance, int amount) {
    return amount <= balance;
}

// Odečte částku z účtu
void updateBalance(int& balance, int amount) {
    balance -= amount;
}

// Vypíše počet a nominál vydaných bankovek
void printBanknotes(int amount) {
    int denominations[] = {5000, 2000, 1000, 200};
    for (int d : denominations) {
        int cnt = amount / d;
        if (cnt > 0) {
            cout << cnt << " x " << d << " Kč\n";
            amount -= cnt * d;
        }
    }
}

// Formátuje částku tisícovými mezerami
string formatBalance(int balance) {
    string s = to_string(balance), res;
    int count = 0;
    for (int i = s.size() - 1; i >= 0; --i) {
        res.push_back(s[i]);
        if (++count == 3 && i != 0) {
            res.push_back(' ');
            count = 0;
        }
    }
    reverse(res.begin(), res.end());
    return res;
}

// Vypíše aktuální zůstatek uživatele
void checkBalance(int balance) {
    cout << "Váš zůstatek je: " << formatBalance(balance) << " Kč\n";
}

// Umožní uživateli změnit PIN (dvakrát potvrdit)
void changePin(int& pin) {
    int p1, p2;
    while (true) {
        cout << "Zadejte nový 4místný PIN: ";
        cin  >> p1;
        cout << "Znovu zadejte nový PIN: ";
        cin  >> p2;
        if (p1 != p2) {
            cout << "PINy se neshodují. Zkuste to znovu.\n";
        } else if (p1 < 1000 || p1 > 9999) {
            cout << "PIN musí být 4místné číslo. Zkuste to znovu.\n";
        } else {
            pin = p1;
            cout << "PIN byl úspěšně změněn.\n";
            break;
        }
    }
}
