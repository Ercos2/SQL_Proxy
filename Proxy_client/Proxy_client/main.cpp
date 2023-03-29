#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#pragma warning(dasable:4996)
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

void send_data(SOCKET* sock, std::string data_type) {
	std::string data;
	//std::cin.ignore();
	std::cout << "Input " << data_type << ": ";
	std::getline(std::cin, data);
	send(*sock, std::to_string(data.size()).c_str(), 4, 0);
	send(*sock, (char*)data.c_str(), data.size(), 0);
	if (data == "exit") exit(0);
}

std::string get_data(SOCKET* sock) {
	std::string return_str;
	std::string temp_size;
	recv(*sock, (char*)temp_size.c_str(), 4, 0);

	char* data = new char[std::stoi(temp_size.c_str())];
	int len;

	len = recv(*sock, data, std::stoi(temp_size.c_str()), 0);
	for (int a = 0; a < len; ++a)
		return_str += data[a];

	delete[] data;
	return return_str;
}

void using_db(SOCKET* sock) {
	std::cout << "Input \"exit\" for exit\n";
	while (true) {
		send_data(sock, "request");
		std::string db_answer = get_data(sock);
		std::cout << db_answer << "\n";
	}
}

void init_db(SOCKET* sock) {
	while (true) {
		send_data(sock, "ip");
		send_data(sock, "username");
		send_data(sock, "password");
		send_data(sock, "db name");

		std::string db_answer = get_data(sock);
		//std::cout << db_answer << "\n";
		if (db_answer == "ok") {
			using_db(sock);
		}
	}
	
}

int main() {
	std::string ip;
	int port;

	std::cout << "Input ip: ";
	std::cin >> ip;
	//ip = "127.0.0.1";
	std::cout << "Input port: ";
	std::cin >> port;
	std::cin.ignore();
	//port = 4321;

	WORD dllVer = MAKEWORD(2, 1);
	WSAData wsad;

	WSAStartup(dllVer, &wsad);

	SOCKADDR_IN addr_info;
	memset(&addr_info, 0, sizeof(SOCKADDR_IN));

	addr_info.sin_addr.S_un.S_addr = inet_addr((char*)ip.c_str());
	addr_info.sin_port = htons(port);
	addr_info.sin_family = AF_INET;

	SOCKET s_client = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(s_client, (sockaddr*)&addr_info, sizeof(addr_info)) == 0) {
		std::cout << "CONNECT\n";
		init_db(&s_client);
	}
	else std::cout << "NOT CONNECT\n";

	system("pause");

	return 0;
}