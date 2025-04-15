#include <iostream>  // Підключаємо інструмент для базікання з консоллю (cin, cout)
#include <string>    // Вмикаємо роботу з текстом (рядками)
#include <numeric>   // Тут живе готова магічна штука std::gcd (треба C++17!)
#include <cmath>     // Беремо звідси функцію abs, щоб робити числа добрими (невід'ємними)
#include <stdexcept> // Потрібно, щоб кидатися помилками, якщо щось не так

// Кажемо: "Друже компілятор, не змушуй мене писати std:: перед кожним cout/cin/string/etc."
// У великих проектах так краще не робити, але нам для простоти зійде.
using namespace std;

// --- Креслення для створення супер-об'єктів "Дріб" ---
class Fraction {
private: // Секретна зона: сюди зась стороннім!
    long long numerator;   // Чисельник (те, що НАД рискою). long long - щоб більші числа влазили.
    long long denominator; // Знаменник (те, що ПІД рискою). Теж великий про всяк випадок.

    // Метод "Внутрішній перфекціоніст" - приводить дріб до ладу
    void reduce() {
        // Перше правило клубу дробів: якщо знаменник 0 - це біда!
        // Але ми це ловимо в конструкторі, не тут.

        // Якщо чисельник 0, то дріб - це просто нуль. Робимо його канонічним 0/1.
        if (numerator == 0) {
            denominator = 1; // Знаменник ставимо 1 для краси.
            return;          // Все, нуль прибраний, можна йти.
        }

        // Шукаємо найбільше число, на яке ділиться і верх, і низ.
        // Використовуємо готову магію std::gcd! Передаємо їй числа без знаків (abs).
        long long common_divisor = std::gcd(std::abs(numerator), std::abs(denominator));

        // Якщо НСД не нуль (а він не буде нуль, якщо хоч щось не нуль)
        if (common_divisor != 0) {
             // Ділимо верх і низ на цей НСД. Вжух - і дріб скоротився!
             numerator /= common_divisor;
             denominator /= common_divisor;
        }
        // Маленька перестраховка для дивного випадку gcd(0, 0), хоча ми вже мали вийти раніше.
        // else if (numerator == 0) { denominator = 1; } // Мало б бути непотрібним

        // Правило хорошого тону: мінус має бути зверху, а не знизу.
        if (denominator < 0) {
            numerator = -numerator;     // Перекидаємо мінус наверх
            denominator = -denominator; // А знаменник робимо додатним
        }
    } // Кінець прибирання

public: // Публічна зона: цим можна користуватися ззовні класу

    // "Фабрика дробів" (Конструктор) - створює новий дріб.
    // num - чисельник, den - знаменник. Можна створити і без параметрів (буде 0/1).
    Fraction(long long num = 0, long long den = 1) : numerator(num), denominator(den) {
        // СВЯТА ПЕРЕВІРКА! Знаменник НЕ МОЖЕ бути нулем!
        if (den == 0) {
            // Якщо хтось підсунув нуль - кричимо "ПОМИЛКА!" і кидаємо виняток.
            // Оскільки ми викинули try-catch, програма тут просто впаде. Атата!
            throw std::invalid_argument("Знаменник нуль? Ти серйозно?!");
        }
        // Якщо знаменник не нуль - кличемо "прибиральника", щоб дріб був гарний.
        reduce();
    }

    // "Перекладач на людську мову" - робить з дробу текст (string)
    string toString() const { // const тут означає "обіцяю нічого не міняти всередині дробу"
        // Якщо знаменник 1 - це ж просто ціле число! Його і повертаємо.
        if (denominator == 1) return to_string(numerator);
        // Якщо чисельник 0 - це просто нуль.
        if (numerator == 0) return "0";

        string sign = ""; // Сюди запишемо мінус, якщо він є
        long long num_abs = numerator; // Робоча копія чисельника (без знаку)

        // Якщо дріб від'ємний...
        if (numerator < 0) {
            sign = "-";        // Ставимо мінус на початок
            num_abs = -num_abs; // І працюємо далі з додатним числом
        }

        // Рахуємо цілу частину (скільки разів знаменник влазить у чисельник)
        long long whole_part = num_abs / denominator;
        // Рахуємо залишок - це буде чисельник дробової частини
        long long fraction_part = num_abs % denominator;

        string result = sign; // Починаємо збирати рядок зі знаку (якщо він був)

        // Якщо є ціла частина...
        if (whole_part > 0) {
            result += to_string(whole_part); // Додаємо її до рядка
            // Якщо після цілої частини ще й дробовий хвостик залишився...
            if (fraction_part > 0) {
                // Додаємо пробіл і сам дробовий хвостик (залишок / знаменник)
                result += " " + to_string(fraction_part) + "/" + to_string(denominator);
            }
        } else if (fraction_part > 0) { // Якщо цілої частини не було, але є дробова
             // Просто додаємо дробову частину (чисельник / знаменник)
             result += to_string(fraction_part) + "/" + to_string(denominator);
        }
        // Якщо ми сюди дійшли, і result все ще порожній або "-", значить це був 0,
        // але ми випадок numerator == 0 обробили на самому початку. Отже, все має бути Ок.
        return result; // Віддаємо готовий красивий рядок
    }

    // "Перекладач на комп'ютерну мову чисел" (double)
    double toDouble() const { // const - знову обіцянка нічого не псувати
        // Просто ділимо верх на низ, але так, щоб вийшло десяткове число
        return static_cast<double>(numerator) / denominator;
    }

    // "Сирий вигляд" - просто показує дріб як "число/число"
    string getRawString() const { // const - обіцянка... ну ви зрозуміли
         return std::to_string(numerator) + "/" + std::to_string(denominator);
     }


    // --- Арифметичні Суперсили Дробу ---
    // Важливо: ці функції створюють НОВИЙ дріб як результат, старі не чіпають!

    // Сила Плюса! Додаємо інший дріб 'that' до нашого ('this')
    Fraction plus(const Fraction& that) const { // const Fraction& - беремо інший дріб ефективно і не ламаємо його
        // Рахуємо за шкільною формулою: a/b + c/d = (ad + bc) / bd
        long long new_num = numerator * that.denominator + that.numerator * denominator;
        long long new_den = denominator * that.denominator;
        // Створюємо абсолютно новий дріб з отриманим результатом (він сам себе скоротить)
        return Fraction(new_num, new_den);
    }

    // Сила Мінуса! Віднімаємо дріб 'that' від нашого ('this')
    Fraction minus(const Fraction& that) const {
        // Формула: a/b - c/d = (ad - bc) / bd
        long long new_num = numerator * that.denominator - that.numerator * denominator;
        long long new_den = denominator * that.denominator;
        // Знову створюємо новий дріб-результат
        return Fraction(new_num, new_den);
    }

    // Сила Множення! Множимо наш дріб ('this') на інший ('that')
    Fraction times(const Fraction& that) const {
        // Формула: (a/b) * (c/d) = (ac) / (bd) - найпростіша!
        long long new_num = numerator * that.numerator;
        long long new_den = denominator * that.denominator;
        // І знову народжується новий дріб!
        return Fraction(new_num, new_den);
    }

    // Сила Ділення! Ділимо наш дріб ('this') на інший ('that')
    Fraction by(const Fraction& that) const {
        // НАЙГОЛОВНІША ПЕРЕВІРКА: Чи не намагаємось ми ділити на НУЛЬ?
        if (that.numerator == 0) { // Якщо чисельник другого дробу нуль - це ділення на нуль!
            // Кричимо "ПОМИЛКА!" і кидаємо виняток. Програма впаде. Не можна так!
            throw std::invalid_argument("Ділити на нуль?! Геть звідси!");
        }
        // Якщо все ок, рахуємо за формулою: (a/b) / (c/d) = (a*d) / (b*c)
        long long new_num = numerator * that.denominator;
        long long new_den = denominator * that.numerator; // Тут новий знаменник може стати 0, якщо this->denominator був 0 (неможливо) або that.numerator був 0 (перевірили вище)
        // Все одно, конструктор Fraction ще раз перевірить новий знаменник new_den.
        // Створюємо новий дріб-результат
        return Fraction(new_num, new_den);
    }
}; // Кінець креслення класу Fraction

// --- Головний Режисер Шоу (функція main) ---
// Саме звідси починається виконання програми
int main() {
    long long num1, den1, num2, den2; // Коробки для чисел, які введе користувач
    char slash; // Маленька коробка для символу '/'

    // Кажемо "Привіт!" і просимо перший дріб
    cout << "Введи перший дріб (типу 5/8): ";
    // Читаємо два числа і з'їдаємо слеш між ними
    cin >> num1 >> slash >> den1;

    // Просимо другий дріб
    cout << "Тепер другий дріб (типу 1/4): ";
    // Читаємо другий так само
    cin >> num2 >> slash >> den2;

    // --- Момент істини! Створюємо дроби ---
    // Використовуємо нашу "фабрику" (конструктор).
    // УВАГА! Якщо користувач ввів знаменник 0, програма КРАШНЕТЬСЯ прямо тут!
    Fraction f1(num1, den1); // Створюємо перший дріб
    Fraction f2(num2, den2); // Створюємо другий дріб

    cout << "\nТи ввів: " << f1.getRawString() << " та " << f2.getRawString() << endl;
    cout << "Зараз магія почнеться...\n" << endl;

    // --- Використовуємо Суперсили! ---
    // Кажемо дробам порахувати результати.
    // УВАГА! Якщо f2 виявився нулем, то на діленні програма КРАШНЕТЬСЯ!
    Fraction sum = f1.plus(f2);    // f1 + f2
    Fraction diff = f1.minus(f2);   // f1 - f2
    Fraction prod = f1.times(f2);   // f1 * f2
    Fraction quot = f1.by(f2);    // f1 / f2 (небезпечно!)

    // --- Показ результатів ---
    // Виводимо все на екран, використовуючи "сирий вигляд" для умови
    // і "перекладач на людську" (toString) для результату.
    cout << f1.getRawString() << " + " << f2.getRawString() << " = " << sum.toString() << endl;
    cout << f1.getRawString() << " - " << f2.getRawString() << " = " << diff.toString() << endl;
    cout << f1.getRawString() << " * " << f2.getRawString() << " = " << prod.toString() << endl;
    cout << f1.getRawString() << " / " << f2.getRawString() << " = " << quot.toString() << endl;

    // Якщо ми дійшли сюди, значить, все пройшло без падінь. Ура!
    return 0; // Кажемо операційній системі: "Все чотко, завдання виконано!"
}
