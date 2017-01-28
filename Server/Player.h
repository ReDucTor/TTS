#pragma once
#include <string>
#include <vector>
#include "Packet.h"
#include "Log.h"
#include "MySQL.h"

class Player
{
public:
	Player() {} // NOTE: (reductor) This is unnecessary it's implied
	bool InitPlayer(std::string db, unsigned int userid,MySQL* mysql);
	Packet GetPlayerPacket();
private:
	void LoadRevealedMap();
	void SaveRevealedMap();
public: // NOTE: (reductor) Be careful exposing this data to other usage, especially when your working with multiple threads
	std::string db;//user server 
	unsigned int id; // NOTE: (reductor) This is not initalized by default
	std::string username;
	std::string IP;
	std::string country;
	bool firstime; // NOTE: (reductor) This is not initalized by default
	std::vector<unsigned char> mapRevealed;
	//std::vector<Village> m_villages;
};