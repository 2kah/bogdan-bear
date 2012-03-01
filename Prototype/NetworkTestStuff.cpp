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

//
#include <vector>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Player.h"

static const char *SERVER_IP_ADDRESS="127.0.0.1";
static const unsigned short SERVER_PORT=12345;

static const unsigned char ID_TEXT = 140;

static const unsigned char ID_NEW_EXPLOSION = 141;
static const unsigned char ID_EXIST_EXPLOSION = 142;
static const unsigned char ID_UPDATE_EXPLOSION = 143;
static const unsigned char ID_DESTROY_EXPLOSION = 144;

static const unsigned char ID_NEW_PLAYER = 151;
static const unsigned char ID_EXIST_PLAYER = 152;
static const unsigned char ID_UPDATE_PLAYER = 153;
static const unsigned char ID_DESTROY_PLAYER = 154;

static const unsigned char ID_NEW_PLATFORM = 161;
static const unsigned char ID_EXIST_PLATFORM = 162;
static const unsigned char ID_UPDATE_PLATFORM = 163;
static const unsigned char ID_DESTROY_PLATFORM = 164;

static const unsigned char ID_NEW_ROCKET = 171;
static const unsigned char ID_EXIST_ROCKET = 172;
static const unsigned char ID_UPDATE_ROCKET = 173;
static const unsigned char ID_DESTROY_ROCKET = 174;


RakNet::RakPeerInterface *rakPeer;
RakNet::SocketDescriptor sd;

NetworkTestStuff::NetworkTestStuff()
	: lastID(0)
{
	std::cout << "Init Network" << std::endl;
}

NetworkTestStuff::~NetworkTestStuff()
{
}

void NetworkTestStuff::startNetwork(bool asServer)
{
	this->hosting = asServer;

	if (asServer)
	{
		std::cout << "Starting Server" << std::endl;
		sd.port=SERVER_PORT;
		rakPeer = RakNet::RakPeerInterface::GetInstance();
		RakNet::StartupResult sr = rakPeer->Startup(32,&sd,1);
		//RakAssert(sr==RAKNET_STARTED);
		rakPeer->SetMaximumIncomingConnections(32);
	}
	else
	{
		std::cout << "Starting Client" << std::endl;
		sd.port=0;
		rakPeer = RakNet::RakPeerInterface::GetInstance();
		RakNet::StartupResult sr = rakPeer->Startup(32,&sd,1);
		//RakAssert(sr==RAKNET_STARTED);
		RakNet::ConnectionAttemptResult car = rakPeer->Connect(SERVER_IP_ADDRESS,SERVER_PORT,0,0);
		//RakAssert(car==CONNECTION_ATTEMPT_STARTED);
	}

}

void NetworkTestStuff::sendChat(std::string message)
{
	int len = message.length() + 2;
	char* sendBuffer = (char*)calloc(1,len);
	sendBuffer[0] = ID_TEXT;
	sendBuffer++;
	strcpy_s(sendBuffer,len,message.c_str());
	sendBuffer--;
	rakPeer->Send(sendBuffer,len, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	std::cout << "Broadcast Message: ";
	std::cout << message << std::endl;
}

void NetworkTestStuff::sendChat(std::string message, RakNet::AddressOrGUID target)
{
	int len = message.length() + 2;
	char* sendBuffer = (char*)calloc(1,len);
	sendBuffer[0] = ID_TEXT;
	sendBuffer++;
	strcpy_s(sendBuffer,len,message.c_str());
	sendBuffer--;
	rakPeer->Send(sendBuffer,len, HIGH_PRIORITY, RELIABLE_ORDERED, 0, target, false);
	std::cout << "Send Message To: ";
	std::cout << target.ToString() << std::endl;
	std::cout << message << std::endl;
}

void NetworkTestStuff::sendExplosion(double x, double y, double z)
{
	if (rakPeer >0)
	{
		Coords3D coord;
		coord.typeId=ID_NEW_EXPLOSION;
		coord.x=x;
		coord.y=y;
		coord.z=z;
		rakPeer->Send((char*)&coord,sizeof(coord) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		std::cout << "Sending Explosion at: ";
		std::cout << "(" << x << "," << y << "," << z << ")"  << std::endl;
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
				std::cout << "Connected To: ";
				std::cout << packet->guid.ToString() << std::endl;
				break;
			case ID_NEW_INCOMING_CONNECTION:
				std::cout << "ID_NEW_INCOMING_CONNECTION" << std::endl;
				std::cout << "From: ";
				std::cout << packet->guid.ToString() << std::endl;
				sendChat("Welcome to the server",packet->guid);
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "ID_DISCONNECTION_NOTIFICATION" << std::endl;
				break;
			case ID_CONNECTION_LOST:
				std::cout << "ID_CONNECTION_LOST" << std::endl;
				std::cout << packet->guid.ToString();
				std::cout << " Disconnected" << std::endl;
				break;
			case ID_TEXT:
				std::cout << "Received Message From: " ;
				std::cout << packet->guid.ToString() << std::endl;
				std::cout << packet->data + 1 << std::endl;
				break;
			case ID_NEW_EXPLOSION:
				receiveNewExplosion(packet);
				break;
			case ID_NEW_PLAYER:
				std::cout << "Received New Player Info\n";
				receiveNewPlayer(packet);
				break;
			case ID_EXIST_PLAYER:
				std::cout << "Received Existing Player Info\n";
				receiveExistingPlayer(packet);
				break;
			case ID_UPDATE_PLAYER:
				std::cout << "Received Update Player Info\n";
				receiveUpdatePlayer(packet);
				break;
			case ID_DESTROY_PLAYER:
				std::cout << "Received Destroy Player Info\n";
				receiveDestroyPlayer(packet);
				break;
			}
		}
	}
}



unsigned NetworkTestStuff::registerObject(Object *object)
{
	++this->lastID;

	unsigned id = this->lastID;

	this->objectsByID[id] = object;
	this->IDsByObject[object] = id;

	return id;
}

void NetworkTestStuff::sendWorld()
{
	this->sendPlayers();
}

void NetworkTestStuff::sendPlayers()
{

}

void NetworkTestStuff::sendPlayer(Player *player, bool existing)
{
	Ogre::Vector3 position = player->position;
	Ogre::Vector3 velocity = player->velocity;
	Ogre::Quaternion rotation = player->orientation;
	Ogre::Quaternion elevation = player->relativeAim;

	// send creation of player
}

void NetworkTestStuff::sendPlayer(unsigned char packetType, int playerID, Ogre::Vector3 position, Ogre::Quaternion orientation, Ogre::Vector3 velocity)
{
	if (rakPeer >0)
	{
		NetPlayer p;

		p.orientation=orientation;
		p.playerID=playerID;
		p.position=position;
		p.typeId=packetType;
		p.velocity=velocity;

		rakPeer->Send((char*)&p,sizeof(p) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		std::cout << "Sending Player Info\n";
		//std::cout << "(" << x << "," << y << "," << z << ")"  << std::endl;
	}
}

void NetworkTestStuff::receiveNewPlayer(RakNet::Packet *packet)
{
	// extract player data
	//

	// create a new player with the data from the packet
	//Player *player = new Player(...);

	// register the player as the given ID (not possible yet)
	//

	// signal player replicated
	//this->signals.playerReplicated(player);
}
void NetworkTestStuff::receiveExistingPlayer(RakNet::Packet *packet)
{
	// extract player data
	//

	// create a new player with the data from the packet
	//Player *player = new Player(...);

	// register the player as the given ID (not possible yet)
	//

	// signal player replicated
	//this->signals.playerReplicated(player);
}
void NetworkTestStuff::receiveUpdatePlayer(RakNet::Packet *packet)
{
	// extract player data
	//

	// create a new player with the data from the packet
	//Player *player = new Player(...);

	// register the player as the given ID (not possible yet)
	//

	// signal player replicated
	//this->signals.playerReplicated(player);
}
void NetworkTestStuff::receiveDestroyPlayer(RakNet::Packet *packet)
{
	// extract player data
	//

	// create a new player with the data from the packet
	//Player *player = new Player(...);

	// register the player as the given ID (not possible yet)
	//

	// signal player replicated
	//this->signals.playerReplicated(player);
}
void NetworkTestStuff::receiveNewExplosion(RakNet::Packet *packet)
{
	std::cout << "Received Explosion at: ";
	Coords3D* coords = (Coords3D*)packet->data;
	std::cout << "(" << coords->x << "," << coords->y << "," << coords->z << ")"  << std::endl;
	this->signals.explosion(coords->x, coords->y, coords->z);
}



void NetworkTestStuff::clientConnected()
{
	// send all the world state

	// create a player
    Player *player = new Player(Ogre::Vector3(0, 0, 0));
    this->signals.playerCreated(player);

	// register the player, to synchronise
    this->registerObject(player);

	// tell the client they are that player

}

void NetworkTestStuff::clientDisconnected()
{
	// destroy that client's player
}
