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
	delete result; // NOTE: can just call Disconnect() instead
	delete pstmt;
	delete con;
	result = nullptr; // NOTE: (reductor) there is no need to set these to nullptr, no calls can happen after the destructor
	pstmt = nullptr;
	con = nullptr;
}

bool MySQL::Connect(std::string database)
{
	driver = sql::mysql::get_mysql_driver_instance();
	con = driver->connect(cfg.db.host, cfg.db.username, cfg.db.password); // NOTE: (reductor) This does not delete any previous connection
	con->setSchema(database);
	return con->isValid();
}

void MySQL::Disconnect()
{
	delete result; // NOTE: (reductor) can use unique_ptr instead
	delete pstmt;
	delete con;
	result = nullptr;
	pstmt = nullptr;
	con = nullptr;
}

void MySQL::PstmtQuery(const std::string query) // NOTE: (reductor) Should use const & here
{
	sql::SQLString q(query);
	pstmt = con->prepareStatement(q); // NOTE: (reductor) This can overwite and existing pstmt without deleting it
}

void MySQL::SetString(int id, std::string str) // NOTE: (reductor) Should use const & for str here
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
