#include "Player.h"
#include <fstream>

bool Player::InitPlayer(std::string db, unsigned int userid, MySQL* mysql)
{
	try {
		if(mysql->Connect(db))
		{
			mysql->PstmtQuery("SELECT * FROM user_acc WHERE id=? LIMIT 1");
			mysql->SetInt(1, userid);
			mysql->ExecutePstmtQuery();
			if (mysql->isResult())
			{
				this->db = db;
				id = mysql->GetInt("id");
				username = mysql->GetString("username");
				IP = mysql->GetString("ip");
				country = mysql->GetString("country");
				firstime = mysql->GetInt("firsttimelogin") ? 1 : 0;
				//not sure about firsttime handling probably only do something if its not
				//first time otherwise vectors stays empty 
				//(guild,villages, and ect )
				if (firstime)
				{
					mapRevealed.resize(70000, 0u);// mapwidth * mapheight / bits, fill with 0u
				}
				else
				{
					LoadRevealedMap();
				}
			}
			else
			{
				mysql->Disconnect();
				return false;
			}
			mysql->Disconnect();
		}
	}
	catch (sql::SQLException &e)
	{
		std::string error = "Mysql Error code :" + std::to_string(e.getErrorCode());
		Log(error);
		error = "Mysql State:" + e.getSQLState();
		Log(error);
		error = e.what();
		Log(error);
		mysql->Disconnect();
		return false;
	}
	return true;
}

Packet Player::GetPlayerPacket()
{
	Packet p;
	p.AppendInt16(PacketType::Login_SendUserData);
	Packet temp;
	//construct temp buffer
	temp.AppendInt32((int32_t)username.size());
	temp.Append((char*)username.c_str(), (int)username.size());
	temp.AppendInt32((int32_t)IP.size());
	temp.Append((char*)IP.c_str(), (int)IP.size());
	temp.AppendInt32((int32_t)country.size());
	temp.Append((char*)country.c_str(), (int)country.size());
	temp.AppednInt8((int8_t)firstime);
	temp.AppendInt32((int32_t)mapRevealed.size());
	temp.Append((char*)mapRevealed.data(), (int)mapRevealed.size());
	//adding temp buffer size to p and whole buffer
	p.AppendInt32((int32_t)temp.buffer.size());
	p.Append((char*)&temp.buffer.front(), (int)temp.buffer.size());
	return p;
}

void Player::LoadRevealedMap()
{
	std::ifstream fin("..\\data\\map\\revealed\\" + std::to_string(id) + ".bin", std::ios::binary);
	int size = 800*700 / 8;
	mapRevealed.resize(size);
	fin.read((char*)mapRevealed.data(), size);
	fin.close();
}

void Player::SaveRevealedMap()
{
	std::ofstream fout("..\\data\\map\\revealed\\" + std::to_string(id) + ".bin", std::ios::binary);
	fout.write((char*)mapRevealed.data(), mapRevealed.size());
	fout.close();
}
