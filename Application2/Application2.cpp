
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <cstring>

double func(double a, double b, char operation) {
    // Создание команды для вызова внешней программы
    std::string command = "echo " + std::to_string(a) + " " + operation + " " + std::to_string(b) + " | buggy.exe";

   
}

void runTests(func) {
    // Тест сложения
    assert(func(5, 3, '+') == 8);
    assert(func(-5, -3, '+') == -8);
    assert(func(0, 0, '+') == 0);

    // Тест вычитания
    assert(func(5, 3, '-') == 2);
    assert(func(-5, -3, '-') == -2);
    assert(func(3, 5, '-') == -2);

    // Тест умножения
    assert(func(5, 3, '*') == 15);
    assert(func(-5, -3, '*') == 15);
    assert(func(-5, 3, '*') == -15);

    // Тест деления
    assert(func(6, 3, '/') == 2);
    assert(func(5, 2, '/') == 2.5);

    // Тест деления на ноль
    try {
        func(5, 0, '/');
        std::cerr << "Тест деления на ноль не пройден." << std::endl;
    }
    catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "Ошибка: деление на ноль.");
    }

    // Тест неизвестной операции
    try {
        func(5, 3, '^');
        std::cerr << "Тест неизвестной операции не пройден." << std::endl;
    }
    catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "Ошибка: неизвестная операция.");
    }

    std::cout << "Все тесты пройдены успешно!" << std::endl;
}

int main() {
    setlocale(LC_ALL, "ru");
    runTests(func);
    return 0;
}