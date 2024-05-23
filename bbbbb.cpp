#define WIN32_LEAN_AND_MEAN  // Исключаем редко используемые компоненты из заголовочных файлов Windows.h и WinSock2.h, чтобы ускорить компиляцию.

#include <iostream>           
#include <Windows.h>         
#include <WS2tcpip.h>         
#include <WinSock2.h>        

using namespace std;         

int main() {                 

    WSADATA wsaData;          // Структура, которая будет хранить информацию о использованной версии Winsock.

    ADDRINFOA hints;          // Структура с подсказками для функции getaddrinfo.
    ADDRINFO* addrResult;     // Результат вызова getaddrinfo - информация об адресе.

    SOCKET ConnectSocket = INVALID_SOCKET;  // Сокет для соединения с сервером.

    const char* sendBuffer1 = "Hello from client";                // Первое сообщение, которое будет отправлено серверу.
    const char* sendBuffer2 = "Second message from client";       // Второе сообщение, которое будет отправлено серверу.

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);  // Инициализация Winsock.
    if (result != 0) {                                  // Проверка результата инициализации.
        cout << "WSAStartup failed" << endl;            // Вывод сообщения об ошибке.
        return 1;                                       // Возвращаем код ошибки.
    }

    ZeroMemory(&hints, sizeof(hints));   // Обнуляем структуру hints.
    hints.ai_family = AF_INET;            // Устанавливаем семейство адресов IPv4.
    hints.ai_socktype = SOCK_STREAM;      // Устанавливаем тип сокета TCP.
    hints.ai_protocol = IPPROTO_TCP;      // Устанавливаем протокол TCP.

    result = getaddrinfo("localhost", "6565", &hints, &addrResult);  // Получаем информацию об адресе.
    if (result != 0) {                                              // Проверка результата getaddrinfo.
        cout << "getaddrinfo failed" << endl;                       // Вывод сообщения об ошибке.
        WSACleanup();                                               // Освобождаем Winsock.
        return 1;                                                   // Возвращаем код ошибки.
    }

    ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);  // Создаем сокет.
    if (ConnectSocket == INVALID_SOCKET) {                                                               // Проверка успешности создания сокета.
        cout << "Error at socket()" << endl;                                                             // Вывод сообщения об ошибке.
        freeaddrinfo(addrResult);                                                                         // Освобождаем ресурсы, занятые getaddrinfo.
        WSACleanup();                                                                                      // Освобождаем Winsock.
        return 1;                                                                                          // Возвращаем код ошибки.
    }

    result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);  // Подключаемся к серверу.
    if (result == SOCKET_ERROR) {                                                      // Проверка успешности подключения.
        cout << "Unable to connect to server" << endl;                                   // Вывод сообщения об ошибке.
        closesocket(ConnectSocket);                                                      // Закрываем сокет.
        ConnectSocket = INVALID_SOCKET;                                                  // Устанавливаем сокет в недопустимое состояние.
        freeaddrinfo(addrResult);                                                         // Освобождаем ресурсы, занятые getaddrinfo.
        WSACleanup();                                                                      // Освобождаем Winsock.
        return 1;                                                                          // Возвращаем код ошибки.
    }

    freeaddrinfo(addrResult);  // Освобождаем ресурсы, занятые getaddrinfo.

    result = send(ConnectSocket, sendBuffer1, (int)strlen(sendBuffer1), 0);  // Отправляем первое сообщение серверу.
    if (result == SOCKET_ERROR) {                                           // Проверка успешности отправки сообщения.
        cout << "Send failed" << endl;                                       // Вывод сообщения об ошибке.
        closesocket(ConnectSocket);                                          // Закрываем сокет.
        WSACleanup();                                                         // Освобождаем Winsock.
        return 1;                                                             // Возвращаем код ошибки.
    }

    cout << "Sent: " << sendBuffer1 << endl;  // Выводим отправленное сообщение.

    result = send(ConnectSocket, sendBuffer2, (int)strlen(sendBuffer2), 0);  // Отправляем второе сообщение серверу.
    if (result == SOCKET_ERROR) {                                           // Проверка успешности отправки сообщения.
        cout << "Send failed" << endl;                                       // Вывод сообщения об ошибке.
        closesocket(ConnectSocket);                                          // Закрываем сокет.
        WSACleanup();                                                         // Освобождаем Winsock.
        return 1;                                                             // Возвращаем код ошибки.
    }

    cout << "Sent: " << sendBuffer2 << endl;  // Выводим отправленное сообщение.

    closesocket(ConnectSocket);  // Закрываем сокет.
    WSACleanup();                // Освобождаем Winsock.

    return 0;  // Возвращаем успешный код завершения программы.
}
