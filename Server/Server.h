#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include "Ws2tcpip.h"
#include <string>
#include "LoadIni.h"
#include "PacketManager.h"
#include "PacketStruct.h"
#include "Log.h"
#include "PacketTypes.h"
#include "Player.h"
#include "MySQL.h"

class Connection {
public:
	Connection(SOCKET _socket)
		:
		Active(true),
		socket(_socket)
		
	{}
	bool Active;
	SOCKET socket;
	PacketManager pmngr;
	int ID = 0;
	Player player;
};


class Server {
public:
	Server();
	bool ListenForNewConnections();
	int PacketSenderThread();
private:
	PacketType CheckForLogin(const char * data, int& id);
	
	bool SendAllBytes(std::shared_ptr<Connection> connection, char * data, int totalbytes);
	bool RecvAllBytes(std::shared_ptr<Connection> connection, char * data, int totalbytes);
	//receive methods
	bool Recv32Bits(std::shared_ptr<Connection> connection, int32_t& int32);
	bool Recv16Bits(std::shared_ptr<Connection> connection, int16_t& int16);
	bool Recv8Bits(std::shared_ptr<Connection> connection, int8_t& int8);
	bool RecvPacketType(std::shared_ptr<Connection> connection, PacketType & _packettype);
	bool RecvString(std::shared_ptr<Connection> connection, std::string & msg);
	//Get methods
	void Get32Bits(int32_t& var, int& pos, const char* data);
	void GetString(std::string& str, int& pos, const char* data);

	bool ProcessPacket(std::shared_ptr<Connection> connection, PacketType packettype);
	int UserMsgHandler(std::shared_ptr<Connection> connection);
	void DisconnectClient(std::shared_ptr<Connection> connection);
private:
	std::vector<std::shared_ptr<Connection>> ConnHolder; // client connections sockets
	std::mutex _mutex;
	unsigned int IDCounter = 0;
	INISettings cfg;
	MySQL mysql;
private:
	SOCKADDR_IN addr;
	SOCKET ConnSock;// server connection socket
	int addrLen = sizeof(addr);
};