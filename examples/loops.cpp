#include <iostream>

int main() {
    using namespace std;
    int z = 0;
    
    while(true)
    {
        z++; // Bigger... Bigger...
        std::cout << z;

        break; // No you're not
        
        // Code doesnt reach here
    }


    int userInput;

    while (true)
    {
        cout << "Enter a positive number to exit: ";
        cin >> userInput;

        if (cin.fail()){

            cin.clear();

            cin.ignore(numeric_limits<streamsize>::max(),'\n');

            cout << "Invalid input. Try again." <<endl;
            continue;
        }

        if (userInput > 0)
        {
            break;
        }
        cout << "Invalid input. try again." << endl;
    }

    int x = 0 ;
    while (x < 100) {

        x++;
    
    }

    int i = 3;
    int j = 10;

    while (i > 0)
    {

        cout << i << endl;
        while (j > 0)
        {
            cout << j << endl;
            j--;
        }
        j = 10;
        i--;

    }

    for (int x = 0; x <= 100; x++)
    {
        cout << x << endl;
    }

    int uselessArray[10];
    
    for (int i = 0; i < 10; i++)
    {
        uselessArray[i] = i;
        cout << uselessArray[i] << endl;
    }

}