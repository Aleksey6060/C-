#define WIN32_LEAN_AND_MEAN  // Исключаем редко используемые компоненты из заголовочных файлов Windows.h и WinSock2.h, чтобы ускорить компиляцию.

#include <iostream>          
#include <Windows.h>         
#include <WS2tcpip.h>        
#include <WinSock2.h>        

using namespace std;       

int main() {               

    WSADATA wsaData;         // Структура, которая будет хранить информацию о использованной версии Winsock.

    ADDRINFOA hints;         // Структура с подсказками для функции getaddrinfo.
    ADDRINFO* addrResult;    // Результат вызова getaddrinfo - информация об адресе.

    char recvBuffer[512];    // Буфер для принимаемых данных.

    SOCKET ListenSocket = INVALID_SOCKET;  // Сокет для прослушивания входящих соединений.
    SOCKET ClientSocket = INVALID_SOCKET;  // Сокет для общения с клиентом.

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);  // Инициализация Winsock.
    if (result != 0) {                                  // Проверка результата инициализации.
        cout << "WSAStartup failed" << endl;            // Вывод сообщения об ошибке.
        return 1;                                       // Возвращаем код ошибки.
    }

    ZeroMemory(&hints, sizeof(hints));   // Обнуляем структуру hints.
    hints.ai_family = AF_INET;            // Устанавливаем семейство адресов IPv4.
    hints.ai_socktype = SOCK_STREAM;      // Устанавливаем тип сокета TCP.
    hints.ai_protocol = IPPROTO_TCP;      // Устанавливаем протокол TCP.
    hints.ai_flags = AI_PASSIVE;          // Устанавливаем флаг для привязки к любому доступному адресу.

    result = getaddrinfo(NULL, "6565", &hints, &addrResult);  // Получаем информацию об адресе.
    if (result != 0) {                                         // Проверка результата getaddrinfo.
        cout << "getaddrinfo failed" << endl;                  // Вывод сообщения об ошибке.
        WSACleanup();                                          // Освобождаем Winsock.
        return 1;                                              // Возвращаем код ошибки.
    }

    ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);  // Создаем сокет.
    if (ListenSocket == INVALID_SOCKET) {                                                              // Проверка успешности создания сокета.
        cout << "Error at socket()" << endl;                                                           // Вывод сообщения об ошибке.
        freeaddrinfo(addrResult);                                                                       // Освобождаем ресурсы, занятые getaddrinfo.
        WSACleanup();                                                                                    // Освобождаем Winsock.
        return 1;                                                                                        // Возвращаем код ошибки.
    }

    result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);  // Привязываем сокет к адресу.
    if (result == SOCKET_ERROR) {                                                    // Проверка успешности привязки сокета.
        cout << "bind failed with error" << endl;                                     // Вывод сообщения об ошибке.
        freeaddrinfo(addrResult);                                                     // Освобождаем ресурсы, занятые getaddrinfo.
        closesocket(ListenSocket);                                                     // Закрываем сокет.
        WSACleanup();                                                                  // Освобождаем Winsock.
        return 1;                                                                      // Возвращаем код ошибки.
    }

    freeaddrinfo(addrResult);         // Освобождаем ресурсы, занятые getaddrinfo.

    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {  // Прослушиваем входящие соединения.
        cout << "Listen failed with error" << endl;         // Вывод сообщения об ошибке.
        closesocket(ListenSocket);                          // Закрываем сокет.
        WSACleanup();                                       // Освобождаем Winsock.
        return 1;                                           // Возвращаем код ошибки.
    }

    ClientSocket = accept(ListenSocket, NULL, NULL);  // Принимаем входящее соединение.
    if (ClientSocket == INVALID_SOCKET) {            // Проверка успешности принятия соединения.
        cout << "accept failed" << endl;             // Вывод сообщения об ошибке.
        closesocket(ListenSocket);                   // Закрываем сокет прослушивания.
        WSACleanup();                                // Освобождаем Winsock.
        return 1;                                    // Возвращаем код ошибки.
    }

    closesocket(ListenSocket);  // Закрываем сокет прослушивания, так как он больше не нужен.

    do {
        result = recv(ClientSocket, recvBuffer, 512, 0);  // Принимаем данные от клиента.
        if (result > 0) {                                 // Проверяем успешность приема данных.
            cout << "Bytes received: " << result << endl;        // Выводим количество принятых байт.
            cout << "Received data: " << recvBuffer << endl;     // Выводим принятые данные.
        }
        else if (result == 0) {                          // Если результат равен 0, клиент закрыл соединение.
            cout << "Connection closing..." << endl;      // Выводим сообщение о закрытии соединения.
        }
        else {                                           // В случае ошибки приема данных.
            cout << "recv failed" << endl;               // Выводим сообщение об ошибке.
            closesocket(ClientSocket);                   // Закрываем сокет клиента.
            WSACleanup();                                // Освобождаем Winsock.
            return 1;                                    // Возвращаем код ошибки.
        }
    } while (result > 0);  // Повторяем до тех пор, пока результат приема больше 0.

    result = shutdown(ClientSocket, SD_SEND);  // Отправляем сигнал закрытия отправки данных клиенту.
    if (result == SOCKET_ERROR) {              // Проверяем успешность отправки сигнала.
        cout << "shutdown failed with error: " << WSAGetLastError() << endl;  // Выводим сообщение об ошибке.
        closesocket(ClientSocket);             // Закрываем сокет клиента.
        WSACleanup();                          // Освобождаем Winsock.
        return 1;                              // Возвращаем код ошибки.
    }

    closesocket(ClientSocket);  // Закрываем сокет клиента.
    WSACleanup();               // Освобождаем Winsock.

    return 0;  // Возвращаем успешный код завершения программы.
}
