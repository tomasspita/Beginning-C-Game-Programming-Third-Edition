#include <iostream>

void add(int n1, int n2, int a);
void referenceAdd(int n1, int n2, int& a);


int main()
{
    int number1 = 2;
    int number2 = 2;
    int answer = 0;

    add(number1, number2, answer);

    std::cout << answer << std::endl;

    referenceAdd(number1, number2, answer);

    std::cout << answer << std::endl;

}




void add(int n1, int n2, int a)
{
    a = n1 + n2;
}

void referenceAdd(int n1, int n2, int& a)
{
    a = n1 + n2;
}


