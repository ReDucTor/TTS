#include "PacketManager.h"

bool PacketManager::HavePackets()
{
	// NOTE: (reductor) Should lock the mutex here also
	return !packets.empty();
}

void PacketManager::Append(Packet p)
{
	std::lock_guard<std::mutex> lock(mtx);
	packets.push(p);
}

Packet PacketManager::Retrieve()
{
	std::lock_guard<std::mutex> lock(mtx);
	Packet p = packets.front();
	packets.pop();
	return p;
}

void PacketManager::Clear()
{
	std::lock_guard<std::mutex> lock(mtx);
	// Can just use packets = std::queue<Packet>{};
	std::queue<Packet> empty;
	std::swap(packets, empty);
}
