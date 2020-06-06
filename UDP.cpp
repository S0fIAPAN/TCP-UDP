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
	
	// Подготовка структуры с адресом сервера
	sockaddr_in * echoAddr = new (sockaddr_in);
	echoAddr -> sin_family = AF_INET;
	echoAddr -> sin_port = htons(7);
	echoAddr -> sin_addr.s_addr = inet_addr("82.179.90.12");

	// Сокет
	int echoSocket = socket(AF_INET, SOCK_DGRAM, 0); // UDP
	if(echoSocket == -1)
		errHandler("Error open socket", 11);
	
	// Привязка сокета к указанному адресу
	int rc = bind(echoSocket, (const sockaddr*)selfAddr, sizeof(sockaddr_in));
	if(rc == -1) {
		close(echoSocket);
		errHandler("Error bind socket with local adress", 12);
	}

	// Установка соединения
	rc = connect(echoSocket, (const sockaddr*)echoAddr, sizeof(sockaddr_in));
	if(rc == -1) {
		close(echoSocket);
		errHandler("Error connect socket with remote server", 13);
	}
	
	// Сообщение
	char * buf = new char[256];
	strcpy(buf, "Вызов принят\n");
	int length = strlen(buf);
	
	// Отправление
	rc = send(echoSocket, buf, length, 0);
	if(rc == -1) {
		close(echoSocket);
		errHandler("Error send message", 14);
	}
	cout << "We send: " << buf << endl;
	
	// Получение данных
	rc = recv(echoSocket, (char *)buf, length, 0);
	if(rc == -1) {
		close(echoSocket);
		errHandler("Error recevie answer", 15);
	}
	buf[rc] = '\0';
	cout << "We receive: " << buf << endl;

	close(echoSocket);
	
	delete selfAddr;
	delete echoAddr;
	delete[] buf;
	return 0;
}