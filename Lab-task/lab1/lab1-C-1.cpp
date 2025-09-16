//Calendar system using Sakamoto algorithm

#include <iostream>
#include <iomanip>
using namespace std;

// Sakamoto's algorithm: 0 = Sunday, 1 = Monday, ... 6 = Saturday
int dayOfWeek(int y, int m, int d) {
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (m < 3) y -= 1;
    return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

bool isLeap(int y) {
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

int daysInMonth(int y, int m) {
    static int dm[] = {0,31,28,31,30,31,30,31,31,30,31,30,31}; // 1..12
    if (m == 2) return dm[2] + (isLeap(y) ? 1 : 0); //using ternary operator to return 29 days for leap february.
    return dm[m];
}

string monthName(int m) {
    static const char* names[] = {
        "", "January","February","March","April","May","June",
        "July","August","September","October","November","December"
    };
    return names[m];
}

void printCalendar(int year, int month) {
    const int width = 6;

    cout << "\n\n"
        << setw(36) << ("Calendar Title : " + monthName(month) + " - " + to_string(year)) << "\n";
    cout << string(50, '-') << "\n";

    // Header row
    const char* wk[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    for (int i = 0; i < 7; ++i)
        cout << setw(width) << wk[i];
    cout << "\n" << string(50, '-') << "\n";


    int first = dayOfWeek(year, month, 1);
    int days  = daysInMonth(year, month);

    for (int i = 0; i < first; ++i) cout << setw(width) << "";

    int dow = first;
    for (int d = 1; d <= days; ++d) {
        cout << setw(width) << d;
        dow = (dow + 1) % 7;
        if (dow == 0) cout << "\n";
    }

    if (dow != 0) cout << "\n";
    cout << string(50, '-') << "\n";
}

int main() {
    while (true) {
        int year, month, again;
        cout << "Enter calendar's year: ";
        cin  >> year;

        // month input with simple validation
        do {
            cout << "Enter calendar's month (1-12): ";
            cin >> month;
        } while (month < 1 || month > 12);

        printCalendar(year, month);

        cout << "\nDo you want to see other month? 1 = Yes, others = No: ";
        cin >> again;
        if (again != 1){
            break;
        }
    }
    return 0;
}
