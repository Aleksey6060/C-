#include <iostream>
#include "..\StaticLib\Header.h"
#include <string>
#include <windows.h>

typedef bool (*ContainsAllCharactersFunc)(const char*, const char*);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "Russian");
    std::string inputString, characters;

    
    std::cout << "Введите строку: ";
    std::getline(std::cin, inputString);
    std::cout << "Введите что в ней искать: ";
    std::getline(std::cin, characters);

    
    HINSTANCE hinstLib = LoadLibrary(L"StringChiken.dll");
    if (hinstLib == NULL) {
        std::cerr << "Не удалось загрузить библиотеку DLL!" << std::endl;
        return 1;
    }

    
    ContainsAllCharactersFunc ContainsAllCharacters = (ContainsAllCharactersFunc)GetProcAddress(hinstLib, "ContainsAllCharacters");
    if (ContainsAllCharacters == NULL) {
        std::cerr << "Не удалось найти функцию в библиотеке DLL!" << std::endl;
        FreeLibrary(hinstLib);
        return 1;
    }

    
    bool result = ContainsAllCharacters(inputString.c_str(), characters.c_str());
    std::cout << "Содержит ли строка \"" << inputString << "\" символы \"" << characters << "\"? ";
    if (result) {
        std::cout << "Да" << std::endl;
    }
    else {
        std::cout << "Нет" << std::endl;
    }

    
    FreeLibrary(hinstLib);

    return 0;
}
