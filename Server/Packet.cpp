#include "Packet.h"
#include <memory>
Packet::Packet(char * _buffer, int _size)
{
	buffer.insert(buffer.end(), _buffer, _buffer + _size);
}

Packet::Packet(const Packet & p)
{
	buffer = p.buffer;
}

Packet::Packet(PacketType type)
{
	AppendInt16(type);
}

Packet::~Packet()
{
	buffer.clear();
}

void Packet::Append(char * data, int _size)
{
	buffer.insert(buffer.end(), data, data+ _size);
}

void Packet::AppendInt32(int32_t value)
{
	Append((char*)&value, sizeof(int32_t));
}

void Packet::AppendInt16(int16_t value)
{
	Append((char*)&value, sizeof(int16_t));
}

void Packet::AppednInt8(int8_t value)
{
	Append((char*)&value, sizeof(int8_t));
}
