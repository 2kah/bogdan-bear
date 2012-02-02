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
static const unsigned char ID_3D = 141;


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
	std::cout << "TRYING TO SEND EXPLOSION" << std::endl;
	if (rakPeer >0)
	{
		char* xdata = (char*)&x;
		char* ydata = (char*)&y;
		char* zdata = (char*)&z;
		const int bufsize = 1 + (3 * sizeof(x));


		char sendBuffer[bufsize];
		sendBuffer[0] = ID_3D;
		for (int i = 0; i < 8; i++)
		{
			sendBuffer[i+1] = xdata[i];
			sendBuffer[i+9] = ydata[i];
			sendBuffer[i+17] = zdata[i];
		}

		/* test readout
		double newx = 0.0;
		double newy = 0.0;
		double newz = 0.0;

		char* newxdata = (char*)&newx;
		char* newydata = (char*)&newy;
		char* newzdata = (char*)&newz;

		for (int i = 0; i < 8; i++)
		{
		newxdata[i] = sendBuffer[i+1];
		newydata[i] = sendBuffer[i+9];
		newzdata[i] = sendBuffer[i+17];
		}

		double thingy = newx + newy + newz;
		std::cout << thingy << std::endl;
		*/

		rakPeer->Send(sendBuffer,bufsize , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
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
			case ID_3D:
				std::cout << "Received 3D coords: ";
				double x = 0.0;
				double y = 0.0;
				double z = 0.0;
				char* xdata = (char*)&x;
				char* ydata = (char*)&y;
				char* zdata = (char*)&z;

				for (int i = 0; i < 8; i++)
				{
					xdata[i] = packet->data[i+1];
					ydata[i] = packet->data[i+9];
					zdata[i] = packet->data[i+17];
				}
				std::cout << "(" << x << "," << y << "," << z << ")"  << std::endl;
				break;
			}
		}
	}
}