#include <iostream>
#include <string>
#include <numeric> // Для std::gcd з C++17 або для власної реалізації
#include <cmath>   // Для std::abs
#include <stdexcept> // Для std::invalid_argument

// Функція для знаходження найбільшого спільного дільника (НСД)
// Використовуємо алгоритм Евкліда
int gcd(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
    // Або якщо компілятор підтримує C++17:
    // return std::gcd(a, b);
}

class Fraction {
private:
    int numerator;
    int denominator;

    // Приватний метод для скорочення дробу
    void reduce() {
        if (denominator == 0) {
            // Залишаємо знаменник 0, щоб вказати на помилку пізніше,
            // або можна викинути виняток прямо тут.
            return;
        }
        if (numerator == 0) {
            denominator = 1; // Канонічна форма для нуля: 0/1
            return;
        }

        int common_divisor = gcd(numerator, denominator);
        numerator /= common_divisor;
        denominator /= common_divisor;

        // Переконуємося, що знак мінуса завжди у чисельнику
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public:
    // Конструктор
    Fraction(int num, int den) : numerator(num), denominator(den) {
        if (den == 0) {
            // Можна викинути виняток або встановити стан помилки
            // В даному випадку просто зберігаємо 0, toString перевірить це
             std::cerr << "Warning: Denominator cannot be zero." << std::endl;
             // Або краще: throw std::invalid_argument("Denominator cannot be zero");
        } else {
            reduce(); // Скорочуємо дріб одразу при створенні
        }
    }

    // Метод для представлення дробу у вигляді рядка
    std::string toString() const {
        if (denominator == 0) {
            return "Error: Division by zero";
        }
        // Створюємо копію для роботи з цілою та дробовою частинами
        int temp_num = numerator;
        int temp_den = denominator;

        // Якщо дріб від'ємний, запам'ятовуємо знак і працюємо з модулями
        std::string sign = "";
        if (temp_num < 0) {
            sign = "-";
            temp_num = -temp_num;
        }

        int whole_part = temp_num / temp_den;
        int fractional_num = temp_num % temp_den;

        std::string result = sign;
        if (whole_part > 0) {
            result += std::to_string(whole_part);
            if (fractional_num > 0) {
                result += " " + std::to_string(fractional_num) + "/" + std::to_string(temp_den);
            }
        } else {
            if (fractional_num > 0) {
                 // Якщо цілої частини немає, але є дробова
                 result += std::to_string(fractional_num) + "/" + std::to_string(temp_den);
            } else if (numerator == 0) {
                // Якщо дріб дорівнює нулю
                result = "0";
            } else {
                 // Якщо дріб є від'ємним цілим числом (наприклад, -2/1)
                 // або якщо є тільки ціла частина (наприклад, 2/1) - це вже оброблено вище
                 // Ця умова може бути зайвою через попередні перевірки
                 // Якщо numerator != 0, а whole_part = 0 і fractional_num = 0, що неможливо
                 // якщо логіка правильна. Залишимо для ясності, що дріб 0 обробляється.
            }
        }
         // Якщо результат порожній (може бути, якщо дріб 0 і не було знака)
        if (result.empty() || result == "-") {
             result = "0";
        }


        return result;
    }

    // Метод для представлення дробу у вигляді десяткового числа
    double toDouble() const {
        if (denominator == 0) {
            return NAN; // Not a Number
        }
        return static_cast<double>(numerator) / denominator;
    }

    // Метод додавання дробів
    Fraction plus(const Fraction& that) const {
        if (this->denominator == 0 || that.denominator == 0) {
             // Повертаємо "невалідний" дріб або кидаємо виняток
             return Fraction(1, 0); // Приклад "невалідного" дробу
        }
        int new_num = this->numerator * that.denominator + that.numerator * this->denominator;
        int new_den = this->denominator * that.denominator;
        return Fraction(new_num, new_den); // Конструктор автоматично скоротить
    }

    // Метод віднімання дробів
    Fraction minus(const Fraction& that) const {
         if (this->denominator == 0 || that.denominator == 0) {
             return Fraction(1, 0);
        }
        int new_num = this->numerator * that.denominator - that.numerator * this->denominator;
        int new_den = this->denominator * that.denominator;
        return Fraction(new_num, new_den);
    }

    // Метод множення дробів
    Fraction times(const Fraction& that) const {
         if (this->denominator == 0 || that.denominator == 0) {
             return Fraction(1, 0);
        }
        int new_num = this->numerator * that.numerator;
        int new_den = this->denominator * that.denominator;
        return Fraction(new_num, new_den);
    }

    // Метод ділення дробів
    Fraction by(const Fraction& that) const {
         if (this->denominator == 0 || that.denominator == 0 || that.numerator == 0) {
             // Не можна ділити на нуль (якщо дріб-дільник дорівнює нулю)
             return Fraction(1, 0);
        }
        int new_num = this->numerator * that.denominator;
        int new_den = this->denominator * that.numerator;
        return Fraction(new_num, new_den);
    }

    // Додамо геттери, щоб мати доступ до чисельника та знаменника для виводу
    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }
};

// Функція для виведення дробу без перетворення на мішане число
std::string fractionToStringRaw(const Fraction& f) {
     if (f.getDenominator() == 0) return "Error";
     return std::to_string(f.getNumerator()) + "/" + std::to_string(f.getDenominator());
}


int main() {
    int num1, den1, num2, den2;
    char slash;

    std::cout << "Enter first fraction (e.g., 3/4): ";
    std::cin >> num1 >> slash >> den1;

    std::cout << "Enter second fraction (e.g., 1/3): ";
    std::cin >> num2 >> slash >> den2;

    try {
        Fraction f1(num1, den1);
        Fraction f2(num2, den2);

        // Перевірка чи дроби валідні після конструктора
        if (f1.getDenominator() == 0 || f2.getDenominator() == 0) {
             std::cerr << "Error: One or both fractions have a zero denominator." << std::endl;
             return 1;
        }


        Fraction sum = f1.plus(f2);
        Fraction diff = f1.minus(f2);
        Fraction prod = f1.times(f2);
        Fraction quot = f1.by(f2);

        std::cout << fractionToStringRaw(f1) << " + " << fractionToStringRaw(f2) << " = " << sum.toString() << std::endl;
        std::cout << fractionToStringRaw(f1) << " - " << fractionToStringRaw(f2) << " = " << diff.toString() << std::endl;
        std::cout << fractionToStringRaw(f1) << " * " << fractionToStringRaw(f2) << " = " << prod.toString() << std::endl;

        // Перевірка перед виведенням результату ділення
        if (quot.getDenominator() == 0) {
             std::cout << fractionToStringRaw(f1) << " / " << fractionToStringRaw(f2) << " = Error: Division by zero" << std::endl;
        } else {
             std::cout << fractionToStringRaw(f1) << " / " << fractionToStringRaw(f2) << " = " << quot.toString() << std::endl;
        }


    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
