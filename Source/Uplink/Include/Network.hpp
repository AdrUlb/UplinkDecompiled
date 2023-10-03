#pragma once

#include <UplinkObject.hpp>
#include <NetworkScreen.hpp>
#include <DArray.hpp>

typedef void ClientConnection;

class NetworkServer : UplinkObject
{
	int socket;
	int lastSocket;
	bool listening;
	DArray<ClientConnection*> connections;
};

class NetworkClient : UplinkObject
{
	int socket;
	int clientType;
	int currentScreen;
	NetworkScreen* networkScreen;
};

class Network : UplinkObject
{
	NetworkServer server;
	NetworkClient client;
	int status;

public:
	Network();
};
