#include "NetworkTestStuff.h"
#include "RakPeerInterface.h"
#include <iostream>
#include "GetTime.h"
#include "RakSleep.h"
#include "RakAssert.h"
#include "StringTable.h"
#include "RakPeerInterface.h"

#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "ReplicaManager3.h"
#include "NetworkIDManager.h"
#include "RakSleep.h"
#include "FormatString.h"
#include "StringCompressor.h"
#include "Rand.h"
#include "RakNetTypes.h"

static const char *SERVER_IP_ADDRESS="127.0.0.1";
static const unsigned short SERVER_PORT=12345;



RakNet::RakPeerInterface *rakPeer;
NetworkTestStuff::NetworkTestStuff()
{
	std::cout << "Init Network" << std::endl;
}

NetworkTestStuff::~NetworkTestStuff()
{
}

void NetworkTestStuff::startServer()
{
	std::cout << "Starting server" << std::endl;
}

void NetworkTestStuff::startClient()
{
	std::cout << "Starting client" << std::endl;
	RakNet::ConnectionAttemptResult car = rakPeer->Connect(SERVER_IP_ADDRESS,SERVER_PORT,0,0);
	RakAssert(car==CONNECTION_ATTEMPT_STARTED);
}

void NetworkTestStuff::update()
{
	std::cout << "Network Update" << std::endl;
    this->signals.chat("TESTING!");
	this->signals.chat("MEH");
}