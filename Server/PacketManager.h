#pragma once
#include <queue>
#include <mutex>
#include "Packet.h"

class PacketManager {
public:
	bool HavePackets();
	void Append(Packet p);
	Packet Retrieve();
	void Clear();
private:
	std::queue<Packet> packets;
	std::mutex mtx;
};