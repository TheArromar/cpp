//Calendar system using Sakamoto algorithm

#include <iostream>
#include <string>
using namespace std;

string monthChecker(int month);

int main(){
    int year, month, totalLength = 120;
    string monthName;

    cout << "\nEnter calendar's year: ";
    cin >> year;
    cout << "\nEnter calendar's month: ";
    cin >> month;
    string currentMonth = monthChecker(month);
    cout << "The month is " << currentMonth << endl;

    return 0;
}

string monthChecker(int month){
    string monthName;
    switch (month){
        case 1:
            monthName = "January";
            break;
        case 2:
            monthName = "February";
            break;
        case 3:
            monthName = "March";
            break;
        case 4:
            monthName = "April";
            break;
        case 5:
            monthName = "May";
            break;
        case 6:
            monthName = "June";
            break;
        case 7:
            monthName = "July";
            break;
        case 8:
            monthName = "August";
            break;
        case 9:
            monthName = "September";
            break;
        case 10:
            monthName = "October";
            break;
        case 11:
            monthName = "November";
            break;
        case 12:
            monthName = "December";
            break;
            default:
                monthName = "invalid month";
    }
    return monthName;
}