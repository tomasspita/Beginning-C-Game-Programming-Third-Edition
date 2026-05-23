#include <iostream>

using namespace std;

int main () {

    enum class daysOfWeek {MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY,
    SATURDAY, SUNDAY};

    daysOfWeek day = daysOfWeek::SATURDAY;
    
    switch (day) {
        case daysOfWeek::MONDAY:
            cout << "It's Monday";
            break;

        case daysOfWeek::TUESDAY:
            cout << "It's Tuesday";
            break;

        case daysOfWeek::WEDNESDAY:
            cout << "It's Wednesday";
            break;

        case daysOfWeek::THURSDAY:
            cout << "It's Thursday";
            break;

        case daysOfWeek::FRIDAY:
            cout << "It's Friday";
            break;

        case daysOfWeek::SATURDAY:
            cout << "It's Saturday";
            break;

        case daysOfWeek::SUNDAY:
            cout << "It's Sunday";
            break;
    } 

    return 0;


    int addAToB(int a, int b)
{
    int answer = a + b;
    return answer;
}

}