#include <iostream>
#include <iomanip>
using namespace std;

int main(){
    string TPnumber, firstName, lastName;
    int day, month, year;
    float cgpa;

    cout << "TP Number: ";
    cin >> TPnumber;
    cout << "What is your first name: ";
    cin >> firstName;
    cout << "What is your last name: ";
    cin >> lastName;
    cout << "What is your birth date: ";
    cin >> day;
    cout << "What is your brith month: ";
    cin >> month;
    cout << "Which year were you born in: ";
    cin >> year;
    cout << "What is your cgpa: ";
    cin >> cgpa;

    cout << endl << "TP Number: " << TPnumber << endl;
    cout << "Hello there, " << firstName << " "  << lastName << endl;
    cout << "You were born on " << day << "."<< month << "." << year << endl;
    cout << "You scored: " << fixed << setprecision(2) << cgpa << endl;

    return 0;
}


