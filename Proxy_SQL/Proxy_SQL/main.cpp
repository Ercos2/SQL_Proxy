#include "project_source.h"
#include "DB_class.h"
#include "TCP_class.h"

const int connection_num = 100;         //����� ������������ ���������� �����������

SOCKET connections[connection_num];     //������ ��� �������� �������. ������ ������-�� �� �������

int main() { 
    std::string ip_address;
    int port;

    std::cout << "Input ip: ";
    std::cin >> ip_address;
    //ip_address = "127.0.0.1";
    std::cout << "Input port: ";
    std::cin >> port;
    //port = 4321;
    
    TCP_class server_sock(ip_address, port);        //�������� ��� ��������� ������ ������ ������, ����� �� �������� ����. ��� �� ������� ����
    SOCKET s_for_connect;                           //����� ��� ����� � ��������

    boost::asio::io_service ioService;              //���������� ���������� threadpool ������ ��� ��� ������ � �� �����
    boost::thread_group threadpool;
    boost::asio::io_service::work work(ioService);

    for (int a = 0; a < connection_num; ++a) {
        s_for_connect = accept(server_sock.s_listen, (sockaddr*)&server_sock.addr_info, &server_sock.size_addr);        //���������� �������
        if (s_for_connect != 0) {
            std::cout << "CONNECT\n";

            connections[a] = s_for_connect;         //��������� ����� �� �������, ����� ������������ �����������

            threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));       //������ ����� � ����
            ioService.post(boost::bind(init_db, &connections[a]));          //���������� ���� ������� ��� ������ � ����� � �����, ��� ��������
        }
    }
    return 0;
}