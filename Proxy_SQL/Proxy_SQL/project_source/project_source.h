#pragma once
#pragma comment(lib,"mysqlclient.lib")

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <mysql.h>

#pragma warning(dasable:4996)
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

class discriptor_error : public std::exception {
public:
	const char* what() const noexcept override {
		return "Error: can't create MySQL-descriptor\n";
	}
};

class connect_error : public std::exception {
public:
	const char* what() const noexcept override {
		return "Error: can't connect to MySQL. Incorrect data.\n";
	}
};
