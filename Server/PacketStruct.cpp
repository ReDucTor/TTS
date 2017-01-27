#include "PacketStruct.h"
#include "DataStruct\ServerInfo.h"
#include "Server.h"
#include <fstream>

PS::Send::LoadServerInfo::LoadServerInfo()
{
	std::vector<ServerInfo> list;
	std::string database;
	char name[20];
	char version[15];
	std::string filename = "serverlist.txt";
	std::fstream fin(filename.c_str());
	GetPrivateProfileStringA("VERSION", "Version", "NONE", version, sizeof(version), ".\\config.ini");
	while (!fin.eof()) {
		fin >> database;
		fin >> std::ws;
		fin.getline(name, '\n');
		std::string nm = name;
		list.emplace_back(database, nm);
	}
	std::string ver = version;
	p.AppendInt16(PacketType::General_RequestGameInfo);
	Packet temp;//temp buffer
	temp.AppendInt32((int32_t)ver.size());
	temp.Append((char*)ver.c_str(), (int)ver.size());
	temp.AppendInt32((int32_t)list.size());
	for (size_t i = 0; i < list.size(); i++) {
		std::string db = list[i].database;
		std::string nm = list[i].name;

		temp.AppendInt32((int32_t)db.size());
		temp.Append((char*)db.c_str(), (int)db.size());
		temp.AppendInt32((int32_t)nm.size());
		temp.Append((char*)nm.c_str(), (int)nm.size());
	}
	p.AppendInt32((int32_t)temp.buffer.size());
	p.Append((char*)&temp.buffer.front(), (int)temp.buffer.size());
}
