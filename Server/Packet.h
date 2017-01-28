#pragma once
#include <vector>
#include "PacketTypes.h"
class Packet {
public:
	Packet() {}
	Packet(char* _buffer, int _size);
	Packet(const Packet& p);
	Packet(PacketType type);
	~Packet();
	void Append(char* data, int _size);
	void AppendInt32(int32_t value);
	void AppendInt16(int16_t value);
	void AppednInt8(int8_t value);
	std::vector<uint8_t> buffer; // NOTE: (reductor) You should avoid exposing data
};