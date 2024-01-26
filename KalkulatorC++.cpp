#include <iostream>
#include <cmath>

using namespace std;

double add(double num1, double num2) {
    return num1 + num2;
}

double subtract(double num1, double num2) {
    return num1 - num2;
}

double multiply(double num1, double num2) {
    return num1 * num2;
}

double divide(double num1, double num2) {
    if (num2 != 0) {
        return num1 / num2;
    }
    else {
        return 0;
    }
}

double power(double num1, double num2) {
    return pow(num1, num2);
}

double sqrt_num(double num) {
    return sqrt(num);
}

double find_percent(double num) {
    return num / 100;
}

long long factorial(int num) {
    long long result = 1;
    for (int i = 1; i <= num; i++) {
        result *= i;
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    double num1, num2;
    char operation;

    while (true) {
        cout << "Введите операцию (+, -, *, /, $, sqrt, %, !, q): ";
        cin >> operation;

        if (operation == 'q') {
            break;
        }

        cout << "Введите два числа: ";
        cin >> num1 >> num2;

        switch (operation) {
        case '+':
            cout << "Результатом будет: " << add(num1, num2) << endl;
            break;

        case '-':
            cout << "Результатом будет: " << subtract(num1, num2) << endl;
            break;

        case '*':
            cout << "Результатом будет: " << multiply(num1, num2) << endl;
            break;

        case '/':
            if (num2 != 0) {
                cout << "Результатом будет: " << divide(num1, num2) << endl;
            }
            else {
                cout << "Ошибка! Деление на ноль недопустимо." << endl;
            }
            break;

        case '$':
            cout << "Результатом будет: " << power(num1, num2) << endl;
            break;

        case 'sqrt':
            cout << "Результатом будет: " << sqrt_num(num1) << endl;
            break;

        case '%':
            cout << "Результатом будет: " << find_percent(num1) << endl;
            break;

        case '!':
            cout << "Результатом будет: " << factorial(num1) << endl;
            break;

        default:
            cout << "Ошибка! Пожалуйста, введите правильный оператор" << endl;
        }
    }

    return 0;
}