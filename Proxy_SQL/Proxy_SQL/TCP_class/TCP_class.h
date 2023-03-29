#pragma once

#include "project_source.h"

class TCP_class {
public:
    SOCKADDR_IN addr_info;
    int size_addr;
    SOCKET s_listen;

    TCP_class(std::string& ip_address, int& port);
};

std::string get_data(SOCKET* sock);
void send_message(SOCKET* sock, const char* message);