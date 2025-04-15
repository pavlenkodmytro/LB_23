#include <iostream>
#include <string>
#include <numeric> // Для std::gcd
#include <cmath>   // Для std::abs
#include <stdexcept> // Для throw invalid_argument

// Використовуємо простір імен std, щоб не писати std:: перед усім
using namespace std;

class Fraction {
private:
    long long numerator;   // Використовуємо long long для гнучкості
    long long denominator;

    void reduce() {
        if (denominator == 0) return; // Захист від ділення на 0 в gcd
        if (numerator == 0) {
            denominator = 1;
            return;
        }
        long long common_divisor = std::gcd(std::abs(numerator), std::abs(denominator));
        // Якщо дріб вже був 0/1, gcd(0, 1) = 1, все ок.
        // Якщо вхідні дані були 0/0, gcd(0, 0) = 0, треба обережність
        if (common_divisor != 0) {
             numerator /= common_divisor;
             denominator /= common_divisor;
        } else if (numerator == 0) { // Обробка випадку 0/0 -> 0/1
            denominator = 1;
        }
        // В будь-якому випадку, якщо знаменник не 0, робимо його додатнім
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public:
    // Конструктор одразу перевіряє знаменник і скорочує
    Fraction(long long num = 0, long long den = 1) : numerator(num), denominator(den) {
        if (den == 0) {
            // Кидаємо помилку, програма впаде без try-catch
            throw std::invalid_argument("Denominator cannot be zero!");
        }
        reduce();
    }

    // Перетворення на текст (з мішаними числами)
    string toString() const {
        if (denominator == 1) return to_string(numerator); // Ціле число
        if (numerator == 0) return "0"; // Нуль

        string sign = "";
        long long num_abs = numerator;
        if (numerator < 0) {
            sign = "-";
            num_abs = -num_abs;
        }

        long long whole_part = num_abs / denominator;
        long long fraction_part = num_abs % denominator;

        string result = sign;
        if (whole_part > 0) {
            result += to_string(whole_part);
            if (fraction_part > 0) {
                result += " " + to_string(fraction_part) + "/" + to_string(denominator);
            }
        } else if (fraction_part > 0) { // Тільки дробова частина
             result += to_string(fraction_part) + "/" + to_string(denominator);
        }
        // Якщо result все ще порожній або "-", то це був 0, що вже оброблено
        return result;
    }

    // Перетворення на double
    double toDouble() const {
        return static_cast<double>(numerator) / denominator;
    }

    // "Сирий" вигляд дробу
    string getRawString() const {
         return std::to_string(numerator) + "/" + std::to_string(denominator);
     }


    // --- Арифметичні операції ---
    // Використовуємо long long для проміжних обчислень, щоб зменшити ризик переповнення
    Fraction plus(const Fraction& that) const {
        long long new_num = numerator * that.denominator + that.numerator * denominator;
        long long new_den = denominator * that.denominator;
        return Fraction(new_num, new_den);
    }

    Fraction minus(const Fraction& that) const {
        long long new_num = numerator * that.denominator - that.numerator * denominator;
        long long new_den = denominator * that.denominator;
        return Fraction(new_num, new_den);
    }

    Fraction times(const Fraction& that) const {
        long long new_num = numerator * that.numerator;
        long long new_den = denominator * that.denominator;
        return Fraction(new_num, new_den);
    }

    Fraction by(const Fraction& that) const {
        if (that.numerator == 0) {
            // Кидаємо помилку, програма впаде без try-catch
            throw std::invalid_argument("Division by zero fraction!");
        }
        long long new_num = numerator * that.denominator;
        long long new_den = denominator * that.numerator;
        // Перевіряємо, чи не став новий знаменник нулем (можливо, якщо this->denominator був 0,
        // але конструктор це заборонив; або якщо that.numerator був 0, що ми перевірили вище)
        // Додаткова перевірка в конструкторі Fraction(new_num, new_den) все одно буде.
        return Fraction(new_num, new_den);
    }
};

// --- Точка входу ---
int main() {
    long long num1, den1, num2, den2;
    char slash;

    // Просто читаємо дані. Жодних перевірок тут.
    cin >> num1 >> slash >> den1;
    cin >> num2 >> slash >> den2;

    // Створюємо дроби. Якщо знаменник 0 - програма вилетить тут.
    Fraction f1(num1, den1);
    Fraction f2(num2, den2);

    // Рахуємо. Якщо f2 == 0, програма вилетить на f1.by(f2).
    Fraction sum = f1.plus(f2);
    Fraction diff = f1.minus(f2);
    Fraction prod = f1.times(f2);
    Fraction quot = f1.by(f2);

    // Виводимо результати
    cout << f1.getRawString() << " + " << f2.getRawString() << " = " << sum.toString() << endl;
    cout << f1.getRawString() << " - " << f2.getRawString() << " = " << diff.toString() << endl;
    cout << f1.getRawString() << " * " << f2.getRawString() << " = " << prod.toString() << endl;
    cout << f1.getRawString() << " / " << f2.getRawString() << " = " << quot.toString() << endl;

    return 0; // Якщо дійшли сюди - все було (начебто) добре
}
