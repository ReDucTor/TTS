#include "Server.h"

Server::Server()
	:
	mysql(cfg)
{
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "WinSock failed to startup", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	/*if (BroadCastPublic) {
		addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	}
	else {
		inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	}*/

	inet_pton(AF_INET,  cfg.server.host.c_str(), &addr.sin_addr);
	addr.sin_port = htons(cfg.server.port);
	addr.sin_family = AF_INET;

	ConnSock = socket(addr.sin_family, SOCK_STREAM,0);
	if (bind(ConnSock, (SOCKADDR*)&addr, addrLen) == SOCKET_ERROR) {
		std::string errormsg = "Failed to bind address to listing socket.Error" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, errormsg.c_str(), "ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(ConnSock, SOMAXCONN) == SOCKET_ERROR) {
		std::string errormsg = "Failed to listen socket.Error" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, errormsg.c_str(), "ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}
	std::thread ph([this] { PacketSenderThread(); });
	ph.detach();
}

bool Server::ListenForNewConnections()
{
	SOCKET newConnection = INVALID_SOCKET;
	newConnection = accept(ConnSock, (SOCKADDR*)&addr, &addrLen);
	if (newConnection == INVALID_SOCKET) {
		printf("Failed to accept connection. ERROR: %d\n", WSAGetLastError());
		return false;
	}
	else {
		std::lock_guard<std::mutex> lock(_mutex);
		ConnHolder.emplace_back(std::make_shared<Connection>(newConnection));
		ConnHolder.back()->ID = IDCounter;
		IDCounter += 1;
		std::cout << "Client Connected ID : " << IDCounter-1 << std::endl;
		std::thread ph([this] { UserMsgHandler(ConnHolder.back()); });
		ph.detach();
		return true;
	}
}

bool Server::ProcessPacket(std::shared_ptr<Connection> connection, PacketType packettype)
{
	switch (packettype)
	{
	case Chat_ChatMessage:
	{
		std::string message;
		if (!RecvString(connection, message)) {
			return false;
		}
		Packet p;
		p.AppendInt16(Chat_ChatMessage);
		p.AppendInt32((int32_t)message.size());
		p.Append((char*)message.c_str(), (int)message.size());
		for (auto& i : ConnHolder)
		{
			if (i == connection)
				continue;
			connection->pmngr.Append(p);
		}
		break;
	}
	case Login_AttemptToLogin:
	{
		int32_t buffersize;
		if (!Recv32Bits(connection, buffersize))
			return false;
		char* data = new char[buffersize];
		if (!RecvAllBytes(connection, data, buffersize))
		{
			delete[]data;
			return false;
		}
		int id = 0;
		Packet p;
		switch (CheckForLogin(data ,id)) {
			case Login_LoginSuccessful:
				p.AppendInt16(Login_LoginSuccessful);
				p.AppendInt32(id);
				connection->pmngr.Append(p);
				break;
			case Login_WrongPassword:
				p.AppendInt16(Login_WrongPassword);
				connection->pmngr.Append(p);
				break;
			case Login_NoSuchUser:
				p.AppendInt16(Login_NoSuchUser);
				connection->pmngr.Append(p);
				break;
			case Mysql_Error:
				p.AppendInt16(Mysql_Error);
				connection->pmngr.Append(p);
				break;
		}
		delete[]data;
		break;
	}
	case Login_RequestUserData:
	{
		int32_t buffersize = 0;
		Recv32Bits(connection, buffersize);
		char * data = new char[buffersize];
		RecvAllBytes(connection, data, buffersize);
		int pos = 0;
		int32_t userid = 0;
		std::string database;
		Get32Bits(userid, pos, data);
		GetString(database, pos, data);
		connection->player.InitPlayer(database, userid,&mysql);
		Packet p = connection->player.GetPlayerPacket();
		connection->pmngr.Append(p);
		break;
	}
	case General_RequestGameInfo:
	{
		PS::Send::LoadServerInfo g;
		connection->pmngr.Append(g.GetList());
		break;
	}
	default:
		std::cout << "Unknown packet received." << packettype << std::endl;
		break;
	}
	return true;
}

int Server::UserMsgHandler(std::shared_ptr<Connection> connection)
{
	PacketType ptype;
	while (true) {
		if (!RecvPacketType(connection, ptype)) {
			break;
		}
		if (!ProcessPacket(connection, ptype)) {
			break;
		}
		Sleep(10);
	}
	std::cout << "Lost connection to client ID:" << connection->ID << std::endl;
	DisconnectClient(connection);
	return 0;
}

int Server::PacketSenderThread()
{
	while (true) {
		std::lock_guard<std::mutex> lock(_mutex);
		for (auto& i : ConnHolder) {
			if (i->pmngr.HavePackets()) {
				Packet p = i->pmngr.Retrieve();
				if (!SendAllBytes(i, (char*)&p.buffer.front(), (int)p.buffer.size())) {
					std::cout << "Failed to send packet to client " << i << std::endl;
				}
			}
		}
		Sleep(10);
	}
	return 0;
}

void Server::DisconnectClient(std::shared_ptr<Connection> connection)
{
	std::lock_guard<std::mutex> lock(_mutex);
	connection->pmngr.Clear();
	closesocket(connection->socket);
	// need to stop thread
	ConnHolder.erase(std::remove(ConnHolder.begin(), ConnHolder.end(), connection));
	std::cout << "Client Disconected : " << connection->ID << std::endl;
}

PacketType Server::CheckForLogin(const char * data,int& id)
{
	try {
		int pos = 0;
		std::string server;
		std::string username;
		std::string password;
		GetString(server, pos, data);
		GetString(username, pos, data);
		GetString(password, pos, data);
		if (mysql.Connect(server))
		{
			mysql.PstmtQuery("SELECT password FROM user_acc WHERE username=? LIMIT 1");
			mysql.SetString(1, username);
			mysql.ExecutePstmtQuery();
			if (mysql.isResult())
			{
				if (mysql.GetString("password") == password)
				{
					mysql.PstmtQuery("SELECT firsttimelogin,id FROM user_acc WHERE username= ? LIMIT 1");
					mysql.SetString(1, username);
					mysql.ExecutePstmtQuery();
					if (mysql.isResult())
					{
						id = mysql.GetInt("id");
						mysql.Disconnect();
						return Login_LoginSuccessful;
					}
				}
				else
				{
					mysql.Disconnect();
					return Login_WrongPassword;
				}
			}
			else
			{
				mysql.Disconnect();
				return Login_NoSuchUser;
			}
			mysql.Disconnect();
		}
		else
		{
			return Mysql_Error;
		}
	}catch (sql::SQLException &e) 
	{
		std::string error = "Mysql Error code :" + std::to_string(e.getErrorCode());
		Log(error);
		error = "Mysql State:" + e.getSQLState();
		Log(error);
		error = e.what();
		Log(error);
		mysql.Disconnect();
		return Mysql_Error;
	}
}

/////////////////
//SEND METHODS///
/////////////////

bool Server::SendAllBytes(std::shared_ptr<Connection> connection, char * data, int totalbytes)
{
	int bytessent = 0;
	while (bytessent < totalbytes) {
		int ReturnCheck = send(connection->socket, data + bytessent, totalbytes - bytessent, NULL);
		if (ReturnCheck == SOCKET_ERROR) {
			return false;
		}
		bytessent += ReturnCheck;
	}
	return true;
}

bool Server::RecvAllBytes(std::shared_ptr<Connection> connection, char * data, int totalbytes)
{
	int bytesreceived = 0;
	while (bytesreceived < totalbytes) {
		int ReturnCheck = recv(connection->socket, data + bytesreceived, totalbytes - bytesreceived, NULL);
		if (ReturnCheck == SOCKET_ERROR) {
			return false;
		}
		bytesreceived += ReturnCheck;
	}
	return true;
}

bool Server::Recv32Bits(std::shared_ptr<Connection> connection, int32_t& int32) {
	return RecvAllBytes(connection, (char*)&int32, sizeof(int32_t));
}

bool Server::Recv16Bits(std::shared_ptr<Connection> connection, int16_t & int16)
{
	return RecvAllBytes(connection, (char*)&int16, sizeof(int16_t));
}

bool Server::Recv8Bits(std::shared_ptr<Connection> connection, int8_t & int8)
{
	return RecvAllBytes(connection, (char*)&int8, sizeof(int8_t));
}

bool Server::RecvPacketType(std::shared_ptr<Connection> connection,PacketType & type) {
	int16_t packettype;
	if (!Recv16Bits(connection,packettype)) {
		return false;
	}
	type = (PacketType)packettype;
	return true;
}

bool Server::RecvString(std::shared_ptr<Connection> connection,std::string & msg) {
	int32_t msglen;
	if (!Recv32Bits(connection,msglen)) {
		return false;
	}
	msg.resize(msglen);
	if (!RecvAllBytes(connection,&msg[0], msglen)) {
		return false;
	}
	return true;
}

void Server::Get32Bits(int32_t & var, int & pos, const char * data)
{
	memcpy(&var, data + pos, sizeof(var));
	pos += sizeof(var);
}

void Server::GetString(std::string & str, int & pos, const char * data)
{
	int32_t len = 0;
	Get32Bits(len, pos, data);
	str = std::string(data + pos, len);
	pos += len;

}
