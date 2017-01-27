#include "MySQL.h"

MySQL::MySQL(INISettings & cfg)
	:
	cfg(cfg),
	driver(nullptr),
	con(nullptr),
	pstmt(nullptr),
	result(nullptr)
{
}

MySQL::~MySQL()
{
	delete result;
	delete pstmt;
	delete con;
	result = nullptr;
	pstmt = nullptr;
	con = nullptr;
}

bool MySQL::Connect(std::string database)
{
	driver = sql::mysql::get_mysql_driver_instance();
	con = driver->connect(cfg.db.host, cfg.db.username, cfg.db.password);
	con->setSchema(database);
	return con->isValid();
}

void MySQL::Disconnect()
{
	delete result;
	delete pstmt;
	delete con;
	result = nullptr;
	pstmt = nullptr;
	con = nullptr;
}

void MySQL::PstmtQuery(const std::string query)
{
	sql::SQLString q(query);
	pstmt = con->prepareStatement(q);
}

void MySQL::SetString(int id, std::string str)
{
	pstmt->setString(id, str);
}

void MySQL::SetInt(int id, int var)
{
	pstmt->setInt(id, var);
}

void MySQL::ExecutePstmtQuery()
{
	pstmt->execute();
	result = pstmt->getResultSet();
}

std::string MySQL::GetString(std::string colname)
{
	sql::SQLString c(colname);
	return result->getString(c).asStdString();
}

int MySQL::GetInt(std::string colname)
{
	sql::SQLString c(colname);
	return result->getInt(c);
}
