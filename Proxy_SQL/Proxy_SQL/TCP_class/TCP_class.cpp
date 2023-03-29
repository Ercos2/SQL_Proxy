#include "TCP_class.h"

TCP_class::TCP_class(std::string& ip_address, int& port) {
    WSAData wsad;
    if (WSAStartup(MAKEWORD(2, 1), &wsad) != 0)
    {
        std::cout << "WSAStartup error.\n";
        exit(1);
    }
    memset(&addr_info, 0, sizeof(SOCKADDR_IN));
    size_addr = sizeof(addr_info);
    addr_info.sin_addr.s_addr = inet_addr((char*)ip_address.c_str());
    addr_info.sin_port = htons(port);
    addr_info.sin_family = AF_INET;
    s_listen = socket(AF_INET, SOCK_STREAM, 0);
    _WINSOCK2API_::bind(s_listen, (sockaddr*)&addr_info, sizeof(addr_info));
    listen(s_listen, SOMAXCONN);
}

std::string get_data(SOCKET* sock) {
    std::string return_str;
    std::string temp_size;

    recv(*sock, (char*)temp_size.c_str(), 4, 0);
    char* data = new char[std::stoi(temp_size.c_str())];
    int len = recv(*sock, data, std::stoi(temp_size), 0);

    for (int a = 0; a < len; ++a)
        return_str += data[a];

    delete[] data;
    return return_str;
}

void send_message(SOCKET* sock, const char* message) {
    //std::cout << message << "\n";
    send(*sock, std::to_string(strlen(message)).c_str(), 4, 0);
    send(*sock, message, strlen(message), 0);
}