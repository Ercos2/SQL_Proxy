#include "project_source.h"
#include "DB_class.h"
#include "TCP_class.h"

const int connection_num = 100;         //задаём максимальное количество подключений

SOCKET connections[connection_num];     //массив для хранения сокетов. Вектор почему-то не работал

int main() { 
    std::string ip_address;
    int port;

    std::cout << "Input ip: ";
    std::cin >> ip_address;
    //ip_address = "127.0.0.1";
    std::cout << "Input port: ";
    std::cin >> port;
    //port = 4321;
    
    TCP_class server_sock(ip_address, port);        //скрываем всю настройку сокета внутри класса, чтобы не засорять мэйн. Там же слушаем порт
    SOCKET s_for_connect;                           //сокет для связи с клиентов

    boost::asio::io_service ioService;              //используем бустовский threadpool потому что это удобно и мы можем
    boost::thread_group threadpool;
    boost::asio::io_service::work work(ioService);

    for (int a = 0; a < connection_num; ++a) {
        s_for_connect = accept(server_sock.s_listen, (sockaddr*)&server_sock.addr_info, &server_sock.size_addr);        //подключаем клиента
        if (s_for_connect != 0) {
            std::cout << "CONNECT\n";

            connections[a] = s_for_connect;         //сохраняем сокет на будущее, чтобы поддерживать подключение

            threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));       //создаём поток в пуле
            ioService.post(boost::bind(init_db, &connections[a]));          //направляем туда функцию для работы с базой и сокет, как аргумент
        }
    }
    return 0;
}