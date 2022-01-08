#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include<string>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int main() {
	//Структура WSADATA содержит информацию о реализации сокетов Windows.
	WSAData wsaData;

	WORD wVersionRequested;
	//----Запрашиваем и загружаем библиотеку------//
	//Создает значение WORD путем объединения указанных значений.
	wVersionRequested = MAKEWORD(2, 2);
	//Функция WSAStartup инициирует использование Winsock DLL процессом.
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		cout << "Error! Cannot load library!" << endl;
		return 1;
	}

	cout << "___SERVER___" << endl;

	//----Заполняем информацию об адресе сокета---//
	//Создаем сокет, который привязан к определенному поставщику транспортных услуг.
	//AF_INET - The Internet Protocol version 4 (IPv4) address family.
	//SOCK_DGRAM - Тип сокета, который поддерживает дейтаграммы, которые представляют собой 
	//ненадежные буферы без установления соединения с фиксированной (обычно небольшой) максимальной длиной. 
	//Этот тип сокета использует протокол дейтаграмм пользователя (UDP) для семейства интернет-адресов (AF_INET или AF_INET6).
	SOCKET socket1 = socket(AF_INET, SOCK_DGRAM, 0);

	//Создаем локальный адрес
	//Структура SOCKADDR_IN определяет транспортный адрес и порт для семейства адресов AF_INET.
	struct sockaddr_in socketAddress;
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(1024);
	socketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	//связываем локальный адрес с сокетом.
	//Если ошибок не происходит, bind возвращает ноль. В противном случае он возвращает SOCKET_ERROR
	bind(socket1, (struct sockaddr*)&socketAddress, sizeof(socketAddress));

	//создаем локальные переменные
	char buf[100] = {};
	string leftPart, rightPart;
	string output;
	int localAddressSize = sizeof(socketAddress);

	//Получаем дейтаграмму и сохраняем адрес источника.
	recvfrom(socket1, buf, sizeof(buf), 0, (struct sockaddr*)&socketAddress, &localAddressSize); //получили строку

	//преобразуем массив символов в string
	string client_string = buf;
	cout << "String received from client: " << client_string << endl;

	//если длина строки, введенной пользователем кратна 4, то первая часть строки меняется местами со второй
	if (client_string.length() % 4 == 0) {
		cout << "Stroka kratna 4!" << endl;
		leftPart = client_string.substr(0, client_string.length() / 2);
		rightPart = client_string.substr(client_string.length() / 2, client_string.length() / 2);
		output = rightPart.append(leftPart);
		cout << "output: " << output << endl;
	}
	else {
		output = "Stroka ne kratna 4! No need transformations";
		cout << output << endl;
	}

	//отправляем данные на клиент
	sendto(socket1, output.c_str(), output.size(), 0, (struct sockaddr*)&socketAddress, localAddressSize);

	closesocket(socket1);

}