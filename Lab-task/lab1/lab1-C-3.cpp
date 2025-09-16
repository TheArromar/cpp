//Currency Exchange

#include <iostream>
#include <iomanip>
using namespace std;

int main(){
    int choice;
    double rate, amount;
    cout << fixed << setprecision(2);

    while (true){
        cout << "\nEnter the exchange rate from dollars to RMB: ";
        cin >> rate;
        cout << "Enter 0 to convert dollars to RMB and 1 vice versa: ";
        cin >> choice;

        if (choice == 0){
            cout << "Enter the dollar ammount: ";
        } else {
            cout << "Enter the RMB ammount: ";
        }
        cin >> amount;


        if (choice == 0){
            cout << "$" <<amount << " is " << amount*rate << " yuan";
        } else {
            cout << amount << " yuan is " <<  amount/rate;
        }   
    }
}