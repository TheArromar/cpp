#include <iostream>
#include <iomanip>
using namespace std;

int main(){
    string name, gender, address;
    int age;
    double walletAmount;

    cout << endl << "Student Name             :";
    getline(cin, name);
    cout << endl << "Student Gender           :";
    getline(cin, gender);
    cout << endl << "Student Age              :";
    cin >> age;
    cout << endl << "Student Home Address     :";
    cin.ignore();
    getline(cin, address);
    cout << endl << "E-wallet Amount          :RM ";
    cin >> walletAmount;

    const int contentWidth = 85; 

    cout << endl << string(contentWidth + 4, '=');
    cout << endl << "| " << setw(contentWidth) << left << "Student details as below:" << " |";
    cout << endl << string(contentWidth + 4, '=');
    cout << endl << "| " << setw(19) << left << "Name" << left << setw(1) << ":" << setw(67) << right << name << " |";
    cout << endl << "| " << setw(19) << left << "Age" << left << setw(1) << ":" << setw(67) << right << age << " |";
    cout << endl << "| " << setw(19) << left << "Gender" << left << setw(1) << ":" << setw(67) << right << gender << " |";
    cout << endl << "| " << setw(19) << left << "Address" << left << setw(1) << ":" << setw(67) << right << address << " |";
    cout << endl << "| " << setw(19) << left << "E-wallet" << left << setw(4) << ":" << setw(64) << right << fixed << setprecision(2) << walletAmount << " |";
    cout << endl << string(contentWidth + 4, '=');

    return 0;
}