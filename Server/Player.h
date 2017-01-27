#pragma once
#include <string>
#include <vector>
#include "Packet.h"
#include "Log.h"
#include "MySQL.h"

class Player
{
public:
	Player() {}
	bool InitPlayer(std::string db, unsigned int userid,MySQL* mysql);
	Packet GetPlayerPacket();
private:
	void LoadRevealedMap();
	void SaveRevealedMap();
public:
	std::string db;//user server 
	unsigned int id;
	std::string username;
	std::string IP;
	std::string country;
	bool firstime;
	std::vector<unsigned char> mapRevealed;
	//std::vector<Village> m_villages;
};