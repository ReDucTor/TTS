#pragma once
#include <string>

struct ServerInfo {
	ServerInfo(std::string db, std::string _name) 
		:
		database(db),
		name(_name)
	{}
	std::string database;
	std::string name;
};