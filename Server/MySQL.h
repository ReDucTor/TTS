#pragma once
#include "LoadIni.h"
#include "mysql_driver.h"
#include "cppconn\resultset.h"
#include "cppconn\exception.h"
#include "cppconn\prepared_statement.h"
#include "cppconn\statement.h"
#include "Log.h"
#include <vector>
#include <string>

class MySQL
{
public:
	MySQL(INISettings& cfg);
	~MySQL();
	bool Connect(std::string database);
	void Disconnect();

	void PstmtQuery(const std::string query);
	void SetString(int id, std::string str);
	void SetInt(int id, int var);
	void ExecutePstmtQuery();
	bool isResult() const { return result->next(); }
	std::string GetString(std::string colname);
	int GetInt(std::string colname);
private:
	INISettings& cfg;
	sql::mysql::MySQL_Driver* driver;
	sql::Connection* con;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
};
