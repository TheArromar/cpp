#include <iostream>
#include <iomanip>
using namespace std;

int main(){
    string name, gender, address;
    int age;
    double walletAmount;

    cout << endl << "Student Name:             :";
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

    cout << endl << "Student details as below: ";
    cout << endl << "===========================================================================================";
    cout << endl << "| " << setw(15) << left << "Name";
    cout << "| " << setw(15) << left << "Age";
    cout << "| " << setw(15) << left << "Gender";
    cout << "| " << setw(15) << left << "Address";
    cout << "| " << setw(20) << left << "E-wallet Amount" << "|";
    cout << endl << "===========================================================================================";
    cout << endl << "| " << setw(15) << left << name;
    cout << "| " << setw(15) << left << age;
    cout << "| " << setw(15) << left << gender;
    cout << "| " << setw(15) << left << address;
    cout << "| " << setw(20) << left << fixed << setprecision(2) << walletAmount << "|";
    cout << endl << "===========================================================================================";

    return 0;
}