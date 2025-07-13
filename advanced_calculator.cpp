#include <calculator.h> //Это не обязательный компонент для простых проектов
#include <iostream>     // Для ввода/вывода (cout, cin)
#include <cmath>        // Математические функции (sqrt, pow, fmod)
#include <limits>       // Для работы с numeric_limits (очистка буфера)
#include <fstream>      // Для записи истории в файл
#include <ctime>        // Для работы со временем (запись даты операций)
#include <iomanip>      // Для форматированного вывода
#include <string>       // Для работы со строками
#include <sstream>      // Для преобразования строк в числа

using namespace std;

// Перечисление операций
enum class Operation {
    ADD = 1, SUBTRACT, MULTIPLY, DIVIDE, POWER,
    SQUARE_ROOT, MODULUS, PERCENTAGE, EXIT
};

// Константы
const string HISTORY_FILE = "calculator_history.txt";
const string HELP_FILE = "help.txt";

// Функция очистки буфера
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Безопасный ввод числа
double getNumber(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cout << "Ошибка: введите корректное число\n";
            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            return value;
        }
    }
}

// Основная логика вычислений
double calculate(Operation op, double a, double b = 0) {
    switch (op) {
    case Operation::ADD:      return a + b;
    case Operation::SUBTRACT: return a - b;
    case Operation::MULTIPLY: return a * b;
    case Operation::DIVIDE:   return (b == 0) ? NAN : a / b;
    case Operation::POWER:    return pow(a, b);
    case Operation::SQUARE_ROOT: return (a >= 0) ? sqrt(a) : NAN;
    case Operation::MODULUS:  return fmod(a, b);
    case Operation::PERCENTAGE: return a * b / 100.0;
    default: return NAN;
    }
}

// LocalTime
tm* safe_localtime(const time_t* time) {
    static tm tm_snapshot;
    localtime_s(&tm_snapshot, time);
    return &tm_snapshot;
}

// Запись в историю
void saveToHistory(Operation op, double a, double b, double result) {
    ofstream file(HISTORY_FILE, ios::app);
    if (!file) return;

    time_t now = time(0);
    tm* ltm = safe_localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", ltm);

    file << "[" << time_str << "] ";

    switch (op) {
    case Operation::ADD: file << a << " + " << b; break;
    case Operation::SUBTRACT: file << a << " - " << b; break;
    case Operation::MULTIPLY: file << a << " * " << b; break;
    case Operation::DIVIDE: file << a << " / " << b; break;
    case Operation::POWER: file << a << " ^ " << b; break;
    case Operation::SQUARE_ROOT: file << "sqrt(" << a << ")"; break;
    case Operation::MODULUS: file << a << " % " << b; break;
    case Operation::PERCENTAGE: file << b << "% от " << a; break;
    default: return;
    }

    file << " = " << result << "\n";
}

// Показать подсказки
void showHelp() {
    cout << "\nСправка по операциям:\n";
    cout << "1. Сложение (a + b)\n";
    cout << "2. Вычитание (a - b)\n";
    cout << "3. Умножение (a * b)\n";
    cout << "4. Деление (a / b)\n";
    cout << "5. Возведение в степень (a^b)\n";
    cout << "6. Квадратный корень (sqrt(a))\n";
    cout << "7. Остаток от деления (a % b)\n";
    cout << "8. Процент (b% от a)\n";
    cout << "9. Выход\n\n";
    cout << "Для операций 6 и 8 нужно вводить только первое число!\n";
}

// Меню
void displayMenu() {
    cout << "\n=== КОНСОЛЬНЫЙ КАЛЬКУЛЯТОР ===\n";
    cout << "1. Сложение\t\t5. Степень\n";
    cout << "2. Вычитание\t\t6. Корень (sqrt)\n";
    cout << "3. Умножение\t\t7. Остаток\n";
    cout << "4. Деление\t\t8. Процент\n";
    cout << "9. Выход\t\t?. Помощь (введите '?')\n";
    cout << "Выберите операцию: ";
}

// Спрашиваем у пользователя
Operation getUserChoice() {
    string input;
    while (true) {
        cin >> input;
        clearInputBuffer();

        if (input == "?") {
            showHelp();
            displayMenu();
            continue;
        }

        try {
            int choice = stoi(input);
            if (choice >= 1 && choice <= 9) {
                return static_cast<Operation>(choice);
            }
        }
        catch (...) {}

        stringstream ss(input);
        int choice;
        if (ss >> choice && choice >= 1 && choice <= 9) {
            return static_cast<Operation>(choice);
        }

        cout << "Неверный ввод. Выберите 1-9 или '?' для помощи: ";
    }
}

int main() { //Тут все понятно
    setlocale(LC_ALL, "RU"); // Подключение к русскому языку
    cout << "Добро пожаловать в калькулятор!\n"; //Приветствие с пользователем
    cout << "Введите '?' в меню для получения справки\n";

    while (true) {
        displayMenu();
        Operation choice = getUserChoice();

        if (choice == Operation::EXIT) {
            cout << "\nИстория сохранена в файле: " << HISTORY_FILE << endl;
            cout << "До свидания!\n";
            break;
        }

        double num1, num2 = 0;

        if (choice == Operation::SQUARE_ROOT) {
            num1 = getNumber("Введите число: ");
        }
        else if (choice == Operation::PERCENTAGE) {
            num1 = getNumber("Введите число: ");
            num2 = getNumber("Введите процент: ");
        }
        else if (choice != Operation::EXIT) {
            num1 = getNumber("Введите первое число: ");
            num2 = getNumber("Введите второе число: ");
        }

        double result = calculate(choice, num1, num2);

        if (!isnan(result)) {
            cout << "\nРезультат: " << result << "\n\n";
            saveToHistory(choice, num1, num2, result);
        }
        else {
            cout << "\nОшибка вычисления!\n\n";
        }
    }

    return 0;
}
