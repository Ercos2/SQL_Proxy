#include "DB_class.h"

DB_class::DB_class(std::string& ip, std::string& username, std::string& password, std::string& db_name) {
	conn = mysql_init(NULL);

	if (&conn == NULL) {
		mysql_close(conn);
		throw discriptor_error();
		
	}
	if (!mysql_real_connect(conn, ip.c_str(), username.c_str(), password.c_str(), db_name.c_str(), NULL, NULL, 0)) {
		mysql_close(conn);
		throw connect_error();
	}
	mysql_set_character_set(conn, "utf8");
}

DB_class::~DB_class() {
	mysql_close(conn);
}

std::string DB_class::query_db(std::string command) {
	std::string answer;
	mysql_query(conn, (char*)command.c_str());

	if (res = mysql_store_result(conn)) {
		while (row = mysql_fetch_row(res)) {
			for (int i = 0; i < mysql_num_fields(res); i++) {
				answer += row[i];
				answer += " ";
			}
			answer += "\n";
		}
	}
	else
		answer = mysql_error(conn);

	//std::cout << answer << "\n";
	return answer;
}

void init_db(SOCKET* sock) {
    DB_class* db;
    while (true) {
        std::string ip = get_data(sock);
        std::string user_name = get_data(sock);
        std::string password = get_data(sock);
        std::string db_name = get_data(sock);

        //std::cout << ip.c_str() << "\n";
        //std::cout << user_name.c_str() << "\n";
        //std::cout << password.c_str() << "\n";
        //std::cout << db_name.c_str() << "\n";

        try {
            db = new DB_class(ip, user_name, password, db_name);
            std::cout << "Seccuss\n";
            send_message(sock, "ok");
            break;
        }
        catch (const discriptor_error& x) {
            std::cout << x.what();
            send_message(sock, x.what());
            continue;
        }
        catch (const connect_error& x) {
            std::cout << x.what();
            send_message(sock, x.what());
            continue;
        }
    }
    using_db(sock, db);
}

void using_db(SOCKET* sock, DB_class* db) {
    while (true) {
        std::cout << "awaiting request\n";
        std::string command = get_data(sock);
        if (command == "exit") {
            //delete[] db;
            break;
        }
        std::string db_answer = db->query_db(command);
        if (db_answer.size() < 1) db_answer = " ";
        //std::cout << db_answer << "\n";
        send_message(sock, (char*)db_answer.c_str());
    }
}