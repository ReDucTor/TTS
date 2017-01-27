#pragma once
#include <string>

class INISettings {
public:
	INISettings(){}
private:
	class DatabaseSettings {
	public:
		DatabaseSettings();
		std::string host;
		std::string username;
		std::string password;
	};
	class ServerSettings {
	public:
		ServerSettings();
		std::string host;
		unsigned int port;
	};
public:
	DatabaseSettings db;
	ServerSettings server;
};