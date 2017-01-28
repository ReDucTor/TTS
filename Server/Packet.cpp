#include "Packet.h"
#include <memory>
Packet::Packet(char * _buffer, int _size) // NOTE: (reductor) Should avoid _ prefix for names, this is reserved for the compiler
{
	// NOTE: (reductor) Should use initializer lists instead
	buffer.insert(buffer.end(), _buffer, _buffer + _size);
}

Packet::Packet(const Packet & p)
{
	// NOTE: (reductor) Should use initializer lists instead
	buffer = p.buffer;
}

Packet::Packet(PacketType type)
{
	AppendInt16(type);
}

Packet::~Packet()
{
	// NOTE: (reductor) You do not need to do anything in the destructor, this happens automatically
	buffer.clear();
}

void Packet::Append(char * data, int _size)
{
	// NOTE: (reductor) data should be 'const char *' not 'char*'
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
