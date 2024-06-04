#include <iostream>
#include "..\StaticLib\Header.h"
#include <string>
#include <windows.h>

typedef bool (*ContainsAllCharactersFunc)(const char*, const char*);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "Russian");
    std::string inputString, characters;

    
    std::cout << "������� ������: ";
    std::getline(std::cin, inputString);
    std::cout << "������� ��� � ��� ������: ";
    std::getline(std::cin, characters);

    
    HINSTANCE hinstLib = LoadLibrary(L"StringChiken.dll");
    if (hinstLib == NULL) {
        std::cerr << "�� ������� ��������� ���������� DLL!" << std::endl;
        return 1;
    }

    
    ContainsAllCharactersFunc ContainsAllCharacters = (ContainsAllCharactersFunc)GetProcAddress(hinstLib, "ContainsAllCharacters");
    if (ContainsAllCharacters == NULL) {
        std::cerr << "�� ������� ����� ������� � ���������� DLL!" << std::endl;
        FreeLibrary(hinstLib);
        return 1;
    }

    
    bool result = ContainsAllCharacters(inputString.c_str(), characters.c_str());
    std::cout << "�������� �� ������ \"" << inputString << "\" ������� \"" << characters << "\"? ";
    if (result) {
        std::cout << "��" << std::endl;
    }
    else {
        std::cout << "���" << std::endl;
    }

    
    FreeLibrary(hinstLib);

    return 0;
}
