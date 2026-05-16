#include <string>
#include <iostream>
#include <sstream>

using namespace std;




int main ()
{
    string playerName = "Player1";
    string message = "Welcome to the game, " + playerName + "!";
    cout << message << endl;
    
    int playerNameLength = playerName.length();
    cout << "Player name has " << playerNameLength << " characters."
    <<endl;
    
    string part1 = "hello ";
    string part2 = "world";
    stringstream ss;
    ss << part1 << part2;
    cout << ss.str() << endl; 

    string scoreText = "score = ";
    int score = 0;

    //Later in the code
    score ++;

    stringstream ssscore;
    ssscore << scoreText << score;
    cout << ssscore.str() << endl;
}