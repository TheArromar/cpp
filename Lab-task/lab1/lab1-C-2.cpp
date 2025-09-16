//weather forecast input - input weather for days in a month, and then display total of each type.

#include <iostream>
using namespace std;

int main(){
    char weather;
    int totalHot = 0, totalRainy = 0, totalCloudy = 0, year, totalDays;
    string month, emptystuff;

    while (true){

        {
            /* code */
        }
        
        
        cout << "\nEnter Your Month (e.g. August 2019): ";
        cin >> month >> year;
        for (char &c : month) {  //lowercases the entire thing
            c = tolower(c);
        }
        
        if (month == "january"){
            totalDays = 31;
        } else if (month == "february"){
            if (year % 400 == 0 || (year % 4 ==0 && year % 100 !=0)){
                totalDays = 29;
            } else {
                totalDays = 28;
            }
        } else if (month == "march"){
            totalDays = 31;
        } else if (month == "april"){
            totalDays = 30;
        } else if (month == "may"){
            totalDays = 31;
        } else if (month == "june"){
            totalDays = 30;
        } else if (month == "july"){
            totalDays = 31;
        } else if (month == "august"){
            totalDays = 31;
        } else if (month == "september"){
            totalDays = 30;
        } else if (month == "october"){
            totalDays = 31;
        } else if (month == "november"){
            totalDays = 30;
        } else if (month == "december"){
            totalDays = 31;
        }
        
        for (int i=1; i <= totalDays; i++){
            cout << "\nDay " << i << " : Enter \"H\" for Hot, \"R\" for Rainy, \"C\" for Cloudy, (\"E\" to end):";
            cin >> weather;
            weather = tolower(weather);

            if (weather == 'h'){
                totalHot ++;
            } else if (weather == 'r'){
                totalRainy ++;
            } else if (weather == 'c'){
                totalCloudy ++;
            } else if (weather == 'e'){
                break; 
            }
        }
        cout << "\nNumber of hot days this month: " << totalHot;
        cout << "\nNumber of hot days this month: " << totalRainy;
        cout << "\nNumber of hot days this month: " << totalCloudy;
        cout << "\nPress any key to continue . . . ";
        cin.ignore();
        cin.get();
    }
    return 0;
}