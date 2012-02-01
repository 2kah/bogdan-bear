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
static const unsigned char ID_TEXT = 140;


RakNet::RakPeerInterface *rakPeer;
RakNet::SocketDescriptor sd;

NetworkTestStuff::NetworkTestStuff()
{
	std::cout << "Init Network" << std::endl;
}

NetworkTestStuff::~NetworkTestStuff()
{
}

void NetworkTestStuff::startNetwork(bool asServer)
{
	if (asServer)
	{
		std::cout << "Starting Server" << std::endl;
		sd.port=SERVER_PORT;
		rakPeer = RakNet::RakPeerInterface::GetInstance();
		RakNet::StartupResult sr = rakPeer->Startup(32,&sd,1);
		RakAssert(sr==RAKNET_STARTED);
		rakPeer->SetMaximumIncomingConnections(32);
	}
	else
	{
		std::cout << "Starting Client" << std::endl;
		sd.port=0;
		rakPeer = RakNet::RakPeerInterface::GetInstance();
		RakNet::StartupResult sr = rakPeer->Startup(32,&sd,1);
		RakAssert(sr==RAKNET_STARTED);
		RakNet::ConnectionAttemptResult car = rakPeer->Connect(SERVER_IP_ADDRESS,SERVER_PORT,0,0);
		RakAssert(car==CONNECTION_ATTEMPT_STARTED);
	}

}

void NetworkTestStuff::sendChat(std::string message)
{
	char mymessage [] = " hello";
	mymessage[0] = ID_TEXT;
	rakPeer->Send(mymessage, (int) strlen(mymessage)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	std::cout << "Sent Message" << std::endl;
}

void NetworkTestStuff::sendExplosion(double x, double y, double z)
{

}

void NetworkTestStuff::update()
{
	//std::cout << "Network Update" << std::endl;
    //this->signals.chat("TESTING!");
	//this->signals.chat("MEH");
	if (rakPeer > 0)
	{
		RakNet::Packet *packet;
			for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive())
			{
				switch (packet->data[0])
				{
				case ID_CONNECTION_ATTEMPT_FAILED:
					std::cout << "ID_CONNECTION_ATTEMPT_FAILED" << std::endl;
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					std::cout << "ID_NO_FREE_INCOMING_CONNECTIONS" << std::endl;
					break;
				case ID_CONNECTION_REQUEST_ACCEPTED:
					std::cout << "ID_CONNECTION_REQUEST_ACCEPTED" << std::endl;
					break;
				case ID_NEW_INCOMING_CONNECTION:
					std::cout << "ID_NEW_INCOMING_CONNECTION" << std::endl;
					break;
				case ID_DISCONNECTION_NOTIFICATION:
					std::cout << "ID_DISCONNECTION_NOTIFICATION" << std::endl;
					break;
				case ID_CONNECTION_LOST:
					std::cout << "ID_CONNECTION_LOST" << std::endl;
					break;
				case ID_TEXT:
					std::cout << "Received Message" << std::endl;
					break;
				}
			}
	}
}