#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int main(){
    srand(time(0));
    int lowerLimit = 1, upperLimit = 100;
    int guess;  
    int secret = rand() % (upperLimit - lowerLimit + 1) + lowerLimit;

    while (true){
        cout << "\nGuess Number Game started Now!\n";
        cout << string(45, '-');
        cout << "\nGuess Number between " << lowerLimit << " and " << upperLimit << endl;
        cout << "Your answer: ";
        cin >> guess;

        if (guess < lowerLimit){
            cout << "\nHey, that is a smaller number than the current lower limit";
            continue;
        } else if (guess > upperLimit){
            cout << "\nHey, that is a bigger number than the current Upper limit";
            continue;
        }

        if (guess == secret){
            cout << "\nCongratulation! You won!";
            break;
        } else if (guess < secret){
            lowerLimit = guess;
        } else if (guess > secret){
            upperLimit = guess;
        }
    }
    
    return 0;
}