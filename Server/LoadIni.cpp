#include "LoadIni.h"
#include <Windows.h>

INISettings::DatabaseSettings::DatabaseSettings()
{
	char host[26];
	char dbnamepass[20];
	GetPrivateProfileStringA("DATABASE", "HOST", "127.0.0.1", host, sizeof(host), ".\\config.ini");
	this->host = host;
	GetPrivateProfileStringA("DATABASE", "USER", "NONE", dbnamepass, sizeof(dbnamepass), ".\\config.ini");
	username = dbnamepass;
	GetPrivateProfileStringA("DATABASE", "PASSWORD", "NONE", dbnamepass, sizeof(dbnamepass), ".\\config.ini");
	password = dbnamepass;
}

INISettings::ServerSettings::ServerSettings()
{
	char host[16];
	GetPrivateProfileStringA("SERVER","HOST","127.0.0.1", host, sizeof(host),".\\config.ini");
	this->host = host;
	port = GetPrivateProfileIntA("SERVER", "PORT", 1111, ".\\config.ini");
}
