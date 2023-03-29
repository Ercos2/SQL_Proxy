#pragma once

#include "project_source.h"
#include "TCP_class.h"

class DB_class {
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
public:
	DB_class(std::string& ip, std::string& username, std::string& password, std::string& db_name);
	~DB_class();

	std::string query_db(std::string command);
};

void init_db(SOCKET* sock);
void using_db(SOCKET* sock, DB_class* db);