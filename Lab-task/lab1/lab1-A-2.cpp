#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int main() {
    string name, gender, address;
    int age;
    double walletAmount;

    // Input
    cout << "\nStudent Name             : ";
    getline(cin, name);

    cout << "\nStudent Gender           : ";
    getline(cin, gender);

    cout << "\nStudent Age              : ";
    cin >> age;
    cin.ignore(); // eat leftover '\n' before getline

    cout << "\nStudent Home Address     : ";
    getline(cin, address);

    cout << "\nE-wallet Amount          : RM ";
    cin >> walletAmount;

    // Layout constants
    const int contentWidth = 85;      // width inside the | ... |
    const int labelWidth   = 19;      // width for the label (e.g., "Name")
    const int sepWidth     = 3;       // " : "
    const int valueWidth   = contentWidth - labelWidth - sepWidth;

    // Pre-format wallet as one string so setw applies to the whole thing
    ostringstream os;
    os << "RM " << fixed << setprecision(2) << walletAmount;
    const string walletStr = os.str();

    // Output box
    cout << '\n' << string(contentWidth + 4, '=');
    cout << "\n| " << setw(contentWidth) << left << "Student details as below:" << " |";
    cout << '\n' << string(contentWidth + 4, '=');

    cout << "\n| " << setw(labelWidth) << left << "Name"
        << " : " << setw(valueWidth) << right << name << " |";

    cout << "\n| " << setw(labelWidth) << left << "Age"
        << " : " << setw(valueWidth) << right << age << " |";

    cout << "\n| " << setw(labelWidth) << left << "Gender"
        << " : " << setw(valueWidth) << right << gender << " |";

    cout << "\n| " << setw(labelWidth) << left << "Address"
        << " : " << setw(valueWidth) << right << address << " |";

    cout << "\n| " << setw(labelWidth) << left << "E-wallet"
        << " : " << setw(valueWidth) << right << walletStr << " |";

    cout << '\n' << string(contentWidth + 4, '=') << '\n';

    return 0;
}
