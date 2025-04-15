#include <iostream> // Штука, щоб писати в консоль і читати з неї (типу cout, cin)
#include <string>   // Щоб працювати з текстом (string)
#include <numeric>  // Тут живе крутий std::gcd (але ми напишемо свій, щоб було зрозуміліше)
#include <cmath>    // Треба для модуля числа (щоб -5 стало 5), функція abs()
#include <stdexcept> // Для красивих помилок, якщо хтось дасть 0 в знаменник

// --- Наш власний ручний Шукач Найбільшого Спільного Дільника (НСД) ---
// Ця штука потрібна, щоб скорочувати дроби типу 2/4 до 1/2. Магія!
int findGCD(int a, int b) {
    // Робимо числа добрими (невід'ємними), бо алгоритм любить позитив :)
    a = std::abs(a);
    b = std::abs(b);
    // Алгоритм Евкліда в дії: поки 'b' не нуль, робимо фокуси
    while (b != 0) {
        int temp = b; // Запам'ятовуємо 'b' тимчасово
        b = a % b;    // Нове 'b' - це залишок від ділення 'a' на старе 'b'
        a = temp;     // Нове 'a' - це старе 'b'
    }
    // Коли 'b' стало нулем, то 'a' - це наш НСД! Ура!
    // Якщо обидва були 0, то НСД теж 0 (хоча ділити на 0 не можна!)
    return (a == 0) ? 1 : a; // Повертаємо НСД, але якщо він 0, то краще 1, щоб не ділити на 0 випадково
}

// --- Опис нашого супер-об'єкта "Дріб" (клас Fraction) ---
// Це як креслення для створення дробів.
class Fraction {
private: // Секретні дані, які нікому не показуємо напряму
    int numerator;   // Чисельник (те, що зверху)
    int denominator; // Знаменник (те, що знизу)

    // Приватний метод "Прибиральник" - скорочує дріб до нормального вигляду
    void reduce() {
        // Якщо знаменник 0 - це біда, але ми це обробимо в конструкторі.
        // Якщо чисельник 0, то дріб - це просто 0/1, так красивіше.
        if (numerator == 0) {
            denominator = 1;
            return; // Прибирання завершено!
        }

        // Знаходимо найбільший спільний дільник (НСД)
        int common_divisor = findGCD(numerator, denominator);

        // Ділимо верх і низ на цей НСД - вжух, і дріб скоротився!
        numerator /= common_divisor;
        denominator /= common_divisor;

        // Якщо знаменник раптом від'ємний (типу 1/-2), робимо його додатним,
        // а мінус переносимо наверх (-1/2). Так прийнято у математиків-зануд ;)
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public: // Публічні штуки, якими можна користуватися ззовні

    // "Свідоцтво про народження" дробу (Конструктор)
    // Створює новий об'єкт-дріб з чисельником num і знаменником den
    Fraction(int num, int den) {
        // ПЕРЕВІРКА! Знаменник не може бути НУЛЕМ! Це святе правило!
        if (den == 0) {
            // Якщо хтось спробує, кидаємо помилку - хай переробляє!
            throw std::invalid_argument("Атата! Знаменник нулем бути не може!");
        }
        // Записуємо дані в наш об'єкт
        numerator = num;
        denominator = den;
        // Одразу кличемо "Прибиральника", щоб дріб був гарненьким
        reduce();
    }

    // "Паспорт дробу" - як перетворити дріб на текст (string)
    std::string toString() const {
        // Робимо копії, щоб не псувати оригінал під час фокусів з виведенням
        int current_num = numerator;
        int current_den = denominator;

        std::string result = ""; // Тут буде наш текстовий результат

        // Якщо дріб від'ємний, ставимо мінус попереду і працюємо з додатним
        if (current_num < 0) {
            result += "-";
            current_num = -current_num; // Робимо чисельник тимчасово додатним
        }

        int whole_part = current_num / current_den; // Ціла частина (скільки разів низ влазить у верх)
        int fraction_part = current_num % current_den; // Залишок (чисельник дробової частини)

        // Визначаємо, як вивести:
        if (whole_part > 0) {
            result += std::to_string(whole_part); // Пишемо цілу частину
            if (fraction_part > 0) {
                // Якщо є і ціла, і дробова частина (мішане число)
                result += " " + std::to_string(fraction_part) + "/" + std::to_string(current_den);
            }
        } else { // Якщо цілої частини немає
            if (fraction_part > 0) {
                // Тільки дробова частина (правильний дріб)
                result += std::to_string(fraction_part) + "/" + std::to_string(current_den);
            } else {
                // Якщо немає ні цілої, ні дробової - значить, це був нуль!
                // Або якщо дріб був від'ємний і ми вже додали мінус, то result = "-"
                if (result == "-") { // Якщо це був від'ємний нуль :)
                     result = "0"; // То просто 0
                } else if (result.empty()){ // Якщо нічого не додали
                     result = "0"; // Значить точно 0
                }
                 // Інакше це від'ємне ціле, мінус вже є, ціла частина = 0, дробова = 0 - нічого більше не треба
            }
        }
        return result; // Віддаємо готовий текст
    }

    // "Перекладач на мову комп'ютерних чисел" (double)
    double toDouble() const {
        // Просто ділимо чисельник на знаменник, але як десятковий дріб
        return static_cast<double>(numerator) / denominator;
    }

    // --- Математичні суперсили (методи для операцій) ---
    // Важливо: ці методи НЕ змінюють існуючі дроби, а створюють НОВИЙ дріб-результат

    // Додавання (+)
    Fraction plus(const Fraction& other) const {
        // Формула: a/b + c/d = (a*d + c*b) / (b*d)
        int new_num = this->numerator * other.denominator + other.numerator * this->denominator;
        int new_den = this->denominator * other.denominator;
        // Створюємо новий дріб з результату (він сам себе скоротить у конструкторі)
        return Fraction(new_num, new_den);
    }

    // Віднімання (-)
    Fraction minus(const Fraction& other) const {
        // Формула: a/b - c/d = (a*d - c*b) / (b*d)
        int new_num = this->numerator * other.denominator - other.numerator * this->denominator;
        int new_den = this->denominator * other.denominator;
        return Fraction(new_num, new_den); // Новий дріб - результат
    }

    // Множення (*)
    Fraction times(const Fraction& other) const {
        // Формула: (a/b) * (c/d) = (a*c) / (b*d)
        int new_num = this->numerator * other.numerator;
        int new_den = this->denominator * other.denominator;
        return Fraction(new_num, new_den); // Новий дріб - результат
    }

    // Ділення (/)
    Fraction by(const Fraction& other) const {
        // ДУЖЕ ВАЖЛИВА ПЕРЕВІРКА: на НУЛЬ ділити не можна!
        // Якщо другий дріб (other) - це нуль (його чисельник = 0)
        if (other.numerator == 0) {
            // Кидаємо помилку! Нехай думають, що роблять!
            throw std::invalid_argument("Ділення на нуль! Ти шо, математику не вчив?");
        }
        // Формула: (a/b) / (c/d) = (a*d) / (b*c)
        int new_num = this->numerator * other.denominator;
        int new_den = this->denominator * other.numerator;
        return Fraction(new_num, new_den); // Новий дріб - результат
    }

     // Допоміжний метод, щоб отримати "сирий" вигляд дробу типу "3/4" для виведення
     std::string getRawString() const {
         return std::to_string(numerator) + "/" + std::to_string(denominator);
     }
}; // Кінець опису класу Fraction

// --- Головний Режисер (функція main) ---
// Тут починається вся двіжуха
int main() {
    int num1, den1, num2, den2;
    char slash; // Символ '/' між чисельником і знаменником

    std::cout << "Хеллоу! Введи перший дріб (типу 3/4): ";
    std::cin >> num1 >> slash >> den1; // Читаємо числа і з'їдаємо слеш

    std::cout << "Окей, тепер другий дріб (типу 1/3): ";
    std::cin >> num2 >> slash >> den2; // Читаємо другий

    // Спробуємо створити дроби і порахувати. А раптом помилка?
    try {
        // Створюємо два об'єкти-дроби за нашим "кресленням"
        Fraction f1(num1, den1);
        Fraction f2(num2, den2);

        // Показуємо, що ввів юзер (використовуємо сирий вигляд)
        std::cout << "\nТи ввів: " << f1.getRawString() << " та " << f2.getRawString() << std::endl;
        std::cout << "Зараз порахуємо...\n" << std::endl;


        // Викликаємо "суперсили" для обчислень
        Fraction sum = f1.plus(f2);    // Додавання
        Fraction diff = f1.minus(f2);   // Віднімання
        Fraction prod = f1.times(f2);   // Множення
        Fraction quot = f1.by(f2);    // Ділення (тут може бути помилка ділення на 0!)

        // Виводимо результати красиво (використовуємо toString для мішаних чисел)
        std::cout << f1.getRawString() << " + " << f2.getRawString() << " = " << sum.toString() << std::endl;
        std::cout << f1.getRawString() << " - " << f2.getRawString() << " = " << diff.toString() << std::endl;
        std::cout << f1.getRawString() << " * " << f2.getRawString() << " = " << prod.toString() << std::endl;
        std::cout << f1.getRawString() << " / " << f2.getRawString() << " = " << quot.toString() << std::endl;

    } catch (const std::invalid_argument& e) {
        // Якщо десь сталася біда (знаменник 0 або ділення на 0), ловимо помилку тут
        std::cerr << "\nОЙ! Сталася помилка: " << e.what() << std::endl;
        // Повертаємо 1, щоб система знала - щось пішло не так
        return 1;
    }

    // Якщо все пройшло гладко, повертаємо 0 - тіпа "все ОК, шеф!"
    return 0;
}
