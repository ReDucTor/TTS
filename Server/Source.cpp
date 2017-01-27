//Server
#include "Server.h"
int main()
{
	Server myServer;

	while(true)
	{
		myServer.ListenForNewConnections();
		Sleep(10);
	}
	return 0;
}