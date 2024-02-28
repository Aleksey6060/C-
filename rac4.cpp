#include <iostream> 
#include <string>
#include <algorithm>
#include <random>
#include <locale>
//#include: Это директива препроцессора, которая подключает необходимые заголовочные 
// файлы. В данном случае, мы подключаем заголовочные файлы для работы с вводом-выводом
//  (iostream), строками (string), алгоритмами (algorithm), генерацией случайных чисел 
// (random) и для поддержки локализации (locale).

void reverse(std::wstring& word) {
    std::reverse(word.begin(), word.end());
}
//Это определение функции reverse, которая переворачивает строку. Она принимает ссылку на 
// строку (std::wstring& word) и использует стандартную функцию std::reverse для перестановки
//  символов в обратном порядке.
void removeVowels(std::wstring& word) {
    std::wstring vowels = L"аеёиоуыэюяАЕЁИОУЫЭЮЯ";
    word.erase(std::remove_if(word.begin(), word.end(), [&](wchar_t c) {
        return vowels.find(c) != std::wstring::npos;
        }), word.end());
}
//Это определение функции removeVowels, которая удаляет гласные буквы из строки.Она принимает 
// ссылку на строку(std::wstring& word), создает строку vowels, содержащую гласные буквы, а затем 
// использует стандартные алгоритмы std::remove_if и std::wstring::erase, чтобы удалить гласные буквы из строки.
void removeConsonants(std::wstring& word) {
    std::wstring consonants = L"бвгджзйклмнпрстфхцчшщБВГДЖЗЙКЛМНПРСТФХЦЧШЩ";
    word.erase(std::remove_if(word.begin(), word.end(), [&](wchar_t c) {
        return consonants.find(c) != std::wstring::npos;
        }), word.end());
}
//Это определение функции removeConsonants, которая удаляет согласные буквы из строки. Она аналогична 
// функции removeVowels, но использует строку consonants, содержащую согласные буквы.

void shuffle(std::wstring& word) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(word.begin(), word.end(), g);
}
//Это определение функции shuffle, которая перемешивает буквы в строке. Она использует стандартные 
// средства C++ для генерации случайных чисел (std::random_device, std::mt19937) и стандартную 
// функцию std::shuffle для перемешивания символов в строке.
int main() {
    setlocale(LC_ALL, "Russian");
    //Функция main() является точкой входа в программу. В этой строке кода устанавливается 
    // русская локализация для корректной работы с русским текстом.(которая почему-то не работает)
    std::wstring word;
    int choice;

    std::wcout << L"Введите слово: ";
    std::wcin >> word;
    //Объявляются переменные word(для хранения введенного слова) и choice(для хранения 
    // выбора пользователя).Затем программа запрашивает у пользователя ввод слова.
    std::wcout << L"Меню:\n";
    std::wcout << L"1. Перевернуть слово\n";
    std::wcout << L"2. Удалить гласные\n";
    std::wcout << L"3. Удалить согласные\n";
    std::wcout << L"4. Перемешать буквы\n";
    std::wcout << L"Введите ваш выбор: ";
    std::wcin >> choice;

    switch (choice) {
    case 1:
        reverse(word);
        break;
    case 2:
        removeVowels(word);
        break;
    case 3:
        removeConsonants(word);
        break;
    case 4:
        shuffle(word);
        break;
    default:
        std::wcout << L"Неверный выбор\n";
        return 1;
    }
    //В зависимости от выбора пользователя, программа вызывает соответствующую 
    // функцию для обработки введенного слова. Если выбор некорректен, программа 
    // выводит сообщение об ошибке и завершает выполнение с кодом возврата 1.
    std::wcout << L"Результат: " << word << std::endl;

    return 0;
}