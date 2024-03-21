#include <Network.hpp>
#include <Util.hpp>

NetworkClient::NetworkClient() : socket(-1), type(0), currentScreen(0), screen(nullptr) {}

NetworkClient::~NetworkClient()
{
	UplinkAbort("TODO: implement NetworkClient::~NetworkClient()");
}

void NetworkClient::Print()
{
	printf("NetworkClient : SOCKET:%d\n", socket);
	printf("\tcurrentscreen:%d\n", currentScreen);
}

void NetworkClient::Update()
{
	UplinkAbort("TODO: implement NetworkClient::Update()");
}

const char* NetworkClient::GetID()
{
	return "CLIENT";
}

NetworkServer::NetworkServer()
{
	UplinkAbort("TODO: implement NetworkServer::NetworkServer()");
}

NetworkServer::~NetworkServer()
{
	UplinkAbort("TODO: implement NetworkServer::~NetworkServer()");
}

void NetworkServer::Print()
{
	UplinkAbort("TODO: implement NetworkServer::Print()");
	//printf("NetworkServer : listensocket:%d, lastlisten:%zu\n", listenSocket, lastListen);
	//PrintDArray(&connections);
}

void NetworkServer::Update()
{
	UplinkAbort("TODO: implement NetworkServer::Update()");
}

const char* NetworkServer::GetID()
{
	return "SERVER";
}
