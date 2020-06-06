#include <iostream>
#include <cstdlib> // exit()
#include <cstring> // strcpy(), strlen()
#include <unistd.h> // close()
#include <netinet/in.h> // sockaddr_in and include <sys/socket.h>
#include <arpa/inet.h> // inet_addr()
using namespace std;
void errHandler(const char * why, const int exitCode){
	cout << endl << why << endl;
	exit(exitCode);
}

int main(int argc, char **argv)
{
	// Подготовка структуры с адресом программы (клиента)
	sockaddr_in * selfAddr = new (sockaddr_in);
	selfAddr -> sin_family = AF_INET; // интернет протокол IPv4
	selfAddr -> sin_port = 0; // любой порт на усмотрение ОС
	selfAddr -> sin_addr.s_addr = 0; // все адреса компьютера (минимум 2)
	
	// Подготовка структуры с адресом сервера daytime
	sockaddr_in * remoteAddr = new (sockaddr_in);
	remoteAddr -> sin_family = AF_INET; // интернет протокол IPv4
	remoteAddr -> sin_port = htons(13); // порт 13
	remoteAddr -> sin_addr.s_addr = inet_addr("82.179.90.12"); // локальный адрес 82.179.90.12

	// Сокет
	int mySocket = socket(AF_INET, SOCK_STREAM, 0); // TCP
	if(mySocket == -1)
		errHandler("Error open socket", 11);
	
	// Привязка сокета к указанному адресу
	int rc = bind(mySocket, (const sockaddr*)selfAddr, sizeof(sockaddr_in));
	if(rc == -1) {
		close(mySocket);
		errHandler("Error bind socket with local adress", 12);
	}	
	// Установка соединения
	rc = connect(mySocket, (const sockaddr*)remoteAddr, sizeof(sockaddr_in));
	if(rc == -1) {
		close(mySocket);
		errHandler("Error connect socket with remote server", 13);
	}
	
	// Сообщение
	char * buf = new char[256];
	strcpy(buf, "Вызов принят\n"); // копируем строку
	int length = strlen(buf); // вычисляем длину строки
	
	// Отправление
	rc = send(mySocket, buf, length, 0);
	if(rc == -1) {
		close(mySocket);
		errHandler("Error send message", 14);
	}
	cout << "We send: " << buf << endl;
	
	// Получение данных
	rc = recv(mySocket, (char *)buf, 256, 0);
	if(rc == -1) {
		close(mySocket);
		errHandler("Error recevie answer", 15);
	}
	buf[rc] = '\0';
	cout << "We receive: " << buf << endl;
	
	close(mySocket);
	
	delete selfAddr;
	delete remoteAddr;
	delete[] buf;
	
	return 0;
}
