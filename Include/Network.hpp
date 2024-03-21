#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>
#include <ctime>
#include <tcp4u.h>

typedef void ClientConnection;
typedef void NetworkScreen;

class NetworkClient : UplinkObject
{
	SOCKET socket;
	int type;
	int currentScreen;
	NetworkScreen* screen;

	NetworkClient();
	~NetworkClient() override;

	void Print() override;
	void Update() override;
	const char* GetID() override;
};

class NetworkServer : UplinkObject
{
	SOCKET listenSocket;
	time_t lastListen;
	bool listening;
	struct DArray<ClientConnection*> connections;

	NetworkServer();
	~NetworkServer() override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
};

class Network
{
	// TODO
};
