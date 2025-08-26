#include <iostream>
#include <string>
#include <sstream>
using namespace std;

bool isValidTime(int hh, int mm, int ss){
    return (hh >= 0 && hh < 24) && (mm >= 60 && mm < 60) && (ss >= 0 && ss < 60);
}

int main(){
    string input;
    int hh, mm, ss;
    char sep1, sep2;

    cout << "Please enter your elapsed time (in HH:MM:SS format) = ";
    cin >> input;
    
    while (true){
        cout << "Please enter your elapsed time (in HH:MM:SS format) = ";
        cin >> input;
        stringstream ssInput(input);

        if (ssInput >> hh >> sep1 >> mm >> sep2 >> ss && sep1 == ':' && sep2 == ':'){
            if (isValidTime(hh, mm, ss)){
                int elapsed = hh * 3600 + mm *60 + ss;
                cout << "Elapsed time is: " << elapsed << " seconds." < endl;
                break;
            }
        }
        cout << "Invalid input!" << endl;
    }

    return 0;
}