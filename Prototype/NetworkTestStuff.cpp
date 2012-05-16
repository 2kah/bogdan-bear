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

#include <string>
#include <unordered_map>


#include <vector>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>
#include "Player.h"
#include "Tower.h"
#include "Game.h"
#include "Platform.h"
#include "Rocket.h"
#include "GameTestThing.h"

#include <boost/math/constants/constants.hpp>


std::tr1::unordered_map<uint64_t, Rocket*> RocketByID;
std::tr1::unordered_map<uint64_t, NetPlayer*> NetPlayerByGUID;
std::tr1::unordered_map<int, NetPlayer*> NetPlayerByPlayerID;

//NetPlayer* myNetPlayer = 0;



int playerCounter = 0;
unsigned long rocketCounter = 1;
bool shouldUpload = false;
int updateDelay = 0;

RakNet::RakPeerInterface *rakPeer;
RakNet::SocketDescriptor sd;

NetworkTestStuff::NetworkTestStuff(char *hostIP, GameTestThing* gtt)
	: lastID(0), gamett_obj(gtt)
{
	this->myNetPlayer = 0;
	std::cout << "Init Network" << std::endl;
	teamScores[0] = teamScores[1] = teamScores[2] = teamScores[3] = 0;
	this->SERVER_IP_ADDRESS = hostIP;
	// generate spawn angles

	for (int i = 0; i < 8; ++i) 
	{
        const float radius = 300.0;
		float angle = i * boost::math::constants::two_pi<float>() / 8.0f;
		Ogre::Vector3 spawn = Ogre::Vector3(radius * std::cos(angle), 200, radius * std::sin(angle));
		spawn_points[i] = spawn;
	}

	current_spawn = 0;


}

NetworkTestStuff::~NetworkTestStuff()
{
}

void NetworkTestStuff::insertRocket(unsigned long ID, Rocket* r)
{
	RocketByID.insert(std::pair<uint64_t, Rocket*>(ID, r));
}

void NetworkTestStuff::setLocalPlayer(Player* p)
{
	this->myNetPlayer->player = p;
	p->signals.sendPlatform.connect(boost::bind(&NetworkTestStuff::sendPlatform, this, _1, _2));
}


void NetworkTestStuff::sendTurretBusy(int turretID, bool busy)
{
std:cout << "Sending turret " << turretID << " is " << busy << std::endl;
	TurretInfo ti;
	ti.typeID = ID_TURRET_BUSY;
	ti.turretID = turretID;
	ti.turretBusy = busy;
	rakPeer->Send((char*)&ti,sizeof(TurretInfo), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}
void NetworkTestStuff::recvTurretBusy(RakNet::Packet *packet)
{
	TurretInfo* ti = (TurretInfo*)packet->data;
std:cout << "Receive turret " << ti->turretID << " is " << ti->turretBusy << std::endl;
	this->turrets[ti->turretID]->isBusy=ti->turretBusy;
	if (this->hosting) rakPeer->Send((char*)ti,sizeof(TurretInfo), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkTestStuff::sendTurretInfo(TurretInfo ti){}
void NetworkTestStuff::sendTurretUpdate(int turretID){}
void NetworkTestStuff::recvTurretUpdate(RakNet::Packet *packet){}


void NetworkTestStuff::sendStartGame()
{
	char buf [1];
	buf[0] = ID_START_GAME;
	rakPeer->Send(buf,1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkTestStuff::recvStartGame(RakNet::Packet *packet)
{
	printf("in recvstart\n");
	//this->game_obj->game=true;
	this->game_obj->restartClient();
}

void NetworkTestStuff::startNetwork(bool asServer)
{
	this->hosting = asServer;
	if (rakPeer > 0)
	{
		rakPeer->DestroyInstance(rakPeer);
	}
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

void NetworkTestStuff::insertNetPlayer(NetPlayer* np)
{
	std::cout << "Inserting Player '" << np->name << "'" << std::endl;
	this->signals.addPlayer(np->player, np->team);
	NetPlayerByGUID.insert(std::pair<uint64_t, NetPlayer*>(np->GUID, np));
	NetPlayerByPlayerID.insert(std::pair<int, NetPlayer*>(np->playerID, np));
	listNetPlayers();
}

void NetworkTestStuff::deleteNetPlayer(NetPlayer* np)
{
	//std::cout << "Deleting Player '" << np->name << "'" << std::endl;
	this->signals.removePlayer(np->player);
	np->player->signals.removed(np->player);
	NetPlayerByGUID.erase(np->GUID);
	NetPlayerByPlayerID.erase(np->playerID);
	listNetPlayers();
}

void NetworkTestStuff::listNetPlayers()
{
	std::cout << "----Players----" <<std::endl;

	std::tr1::unordered_map<int, NetPlayer*>::const_iterator It;
	for (It = NetPlayerByPlayerID.begin(); It != NetPlayerByPlayerID.end(); ++It)
	{
		std::cout << It->first << " = " << It->second->name << " GUID: " << It->second->GUID << " Team: " << It->second->team << "Position:" << It->second->player->position.x << ", "<< It->second->player->position.y << ", " << It->second->player->position.z <<std::endl;
	}
	std::cout << "---------------" <<std::endl;
}

void NetworkTestStuff::sendChat(std::string message)
{
	int len = message.length() + 2;
	char* sendBuffer = new char[len];
	sendBuffer[0] = ID_TEXT;
	sendBuffer++;
	//strcpy_s(sendBuffer,len,message.c_str());
	strncpy(sendBuffer, message.c_str(),len);
	sendBuffer--;
	rakPeer->Send(sendBuffer,len, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	std::cout << "Broadcast Message: ";
	std::cout << message << std::endl;
	std::cout << sendBuffer << std::endl;
}

void NetworkTestStuff::sendChat(std::string message, RakNet::AddressOrGUID target)
{
	int len = message.length() + 2;
	char* sendBuffer = new char[len];
	sendBuffer[0] = ID_TEXT;
	sendBuffer++;
	//strcpy_s(sendBuffer,len,message.c_str());
	strncpy(sendBuffer, message.c_str(),len);
	sendBuffer--;
	rakPeer->Send(sendBuffer,len, HIGH_PRIORITY, RELIABLE_ORDERED, 0, target, false);
	std::cout << "Send Message To: ";
	std::cout << target.ToString() << std::endl;
	std::cout << message << std::endl;
}

void NetworkTestStuff::sendExplosion(Ogre::Vector3 position)
{
	if (rakPeer >0)
	{
		NetExplosion exp;
		exp.typeId=ID_NEW_EXPLOSION;
		exp.vector=position;
		exp.isMassive = false;
		exp.rocketID=0;
		rakPeer->Send((char*)&exp,sizeof(exp) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		std::cout << "OLD CODE: Sending Explosion at: " << "(" << position.x << "," << position.y << "," << position.z << ")"  << std::endl;
	}
}

void NetworkTestStuff::sendExplosion(Ogre::Vector3 position, unsigned long rID)
{
	if (rakPeer >0)
	{
		NetExplosion exp;
		exp.typeId=ID_NEW_EXPLOSION;
		exp.vector=position;
		exp.isMassive = false;
		exp.rocketID=rID;
		rakPeer->Send((char*)&exp,sizeof(exp) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		std::cout << "NEW CODE: Rocket "<< rID <<" caused Explosion at: " << "(" << position.x << "," << position.y << "," << position.z << ")"  << std::endl;
	}
}

void NetworkTestStuff::sendExplosion(Ogre::Vector3 position, bool isMassive)
{
	if (rakPeer >0)
	{
		NetExplosion exp;
		exp.typeId=ID_NEW_EXPLOSION;
		exp.vector=position;
		exp.isMassive = isMassive;
		exp.rocketID=0;
		rakPeer->Send((char*)&exp,sizeof(exp) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		std::cout << "Sending Massive Explosion at: " << "(" << position.x << "," << position.y << "," << position.z << ")"  << std::endl;
	}
}

void NetworkTestStuff::sendRocket(Ogre::Vector3 position, Ogre::Quaternion orientation)
{
	if (rakPeer >0)
	{
		NetRocket rocket;
		rocket.typeId=ID_NEW_ROCKET;
		rocket.position = position;
		rocket.orientation = orientation;
		rakPeer->Send((char*)&rocket,sizeof(rocket) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		std::cout << "Sending Rocket at: " << "(" << position.x << "," << position.y << "," << position.z << ")"  << std::endl;
	}
}
void NetworkTestStuff::update()
{
	if (rakPeer > 0)
	{
		if (shouldUpload)
		{
			updateDelay++;
			if (updateDelay > 5)
			{
				updateDelay = 0;
				//if (myNetPlayer->player->isInTurret)
				//{
				//	myNetPlayer->isInTurret=myNetPlayer->player->isInTurret;
				//	myNetPlayer->turretID=myNetPlayer->player->inTurretNum;
				//	std::cout << "Player is in turret " << myNetPlayer->player->inTurretNum <<std::endl;
				//}
				//else
				//{
				//	myNetPlayer->isInTurret=false;
				//}
				sendPlayerUpdate(myNetPlayer);
			}
		}

		RakNet::Packet *packet;
		for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_UPDATE_PLAYER:
				receiveUpdatePlayer(packet);
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				connectedToServer(packet);
				break;
			case ID_NEW_INCOMING_CONNECTION:
				clientConnected(packet);
				break;
			case ID_CONNECTION_LOST:
				if (hosting)
					clientDisconnected(packet);
				else
					disconnectedFromServer(packet);
				break;
			case ID_TEXT:
				receiveChat(packet);
				break;
			case ID_NEW_EXPLOSION:
				receiveNewExplosion(packet);
				break;
			case ID_NEW_ROCKET:
				receiveNewRocket(packet);
				break;
			case ID_ASSIGN_PLAYER:
				receiveAssignPlayer(packet);
				break;
			case ID_INSERT_PLAYER:
				receiveInsertPlayer(packet);
				break;
			case ID_DESTROY_PLAYER:
				receiveDestroyPlayer(packet);
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				std::cout << "ID_CONNECTION_ATTEMPT_FAILED" << std::endl;
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				std::cout << "ID_NO_FREE_INCOMING_CONNECTIONS" << std::endl;
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "ID_DISCONNECTION_NOTIFICATION" << std::endl;
				break;
			case ID_UPDATE_TOWER:
				receiveUpdateTower(packet);
				//std::cout << "Received Tower Update" << std::endl;
				break;
			case ID_UPDATE_SCORES:
				receiveScores(packet);
				//std::cout << "Received Scores Update" << std::endl;
				break;
			case ID_START_GAME:
				recvStartGame(packet);
				//std::cout << "Received Start Game" << std::endl;
				break;
			case ID_NEW_PLATFORM:
				recvPlatform(packet);
				//std::cout << "Received Start Game" << std::endl;
				break;
			case ID_TURRET_BUSY:
				recvTurretBusy(packet);
				//std::cout << "Received Start Game" << std::endl;
				break;
			case ID_TURRET_UPDATE:
				recvTurretUpdate(packet);
				//std::cout << "Received Start Game" << std::endl;
				break;
			}

		}

		if (this->hosting && this->game_obj->game)
		{
			updateDelay++;
			if (updateDelay > 5)
			{
				updateDelay = 0;
				updateScores();
			}
		}


	}
}

void NetworkTestStuff::updateScores()
{
	int teams[4];
	teams[0] = teams[1] = teams[2] = teams[3]= 0;
	std::tr1::unordered_map<int, NetPlayer*>::const_iterator It;
	for (It = NetPlayerByPlayerID.begin(); It != NetPlayerByPlayerID.end(); ++It)
	{
		NetPlayer& np = *(It->second);
		if (g->isPlayerInRange(np.player->position))
		{
			teams[np.team] = 1;
			//teams[np.team]++;
		}
	}
	int maxPlayerCount = 0;
	int bestTeam = -1;
	if(teams[0] + teams[1] + teams[2] + teams[3] == 1) 
	{
		for (int i = 0; i < 4; i++)
		{
			/*if (teams[i] > maxPlayerCount)
			{
			maxPlayerCount = teams[i];
			bestTeam = i;
			}*/
			if(teams[i] == 1) bestTeam = i;
		}
		if (bestTeam > -1)
		{
			this->teamScores[bestTeam]+= 2;
			if ((teamScores[bestTeam] % 100) == 0)
				printScores();
		}
	}
	//printf("Team %d is holding goal\n", bestTeam);
	sendNetScores();
}

void NetworkTestStuff::receiveScores(RakNet::Packet *packet)
{
	//printf("received scores from server\n");
	NetScore* ns = (NetScore*)packet->data;
	this->teamScores[0] = ns->scores[0];
	this->teamScores[1] = ns->scores[1];
	this->teamScores[2] = ns->scores[2];
	this->teamScores[3] = ns->scores[3];
	//printScores();
}


void NetworkTestStuff::sendNetScores()
{
	NetScore ns;
	ns.scores[0] = this->teamScores[0];
	ns.scores[1] = this->teamScores[1];
	ns.scores[2] = this->teamScores[2];
	ns.scores[3] = this->teamScores[3];
	ns.typeId = ID_UPDATE_SCORES;
	rakPeer->Send((char*)&ns,sizeof(ns) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

}

void NetworkTestStuff::sendPlatform(Ogre::Vector3 pos, Ogre::Quaternion orient)
{
	printf("sending platform (%f, %f, %f)\n",pos.x, pos.y, pos.z);
	NetPlatform np;
	np.orientation = orient;
	np.position=pos;
	np.typeId=ID_NEW_PLATFORM;
	rakPeer->Send((char*)&np,sizeof(np) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkTestStuff::recvPlatform(RakNet::Packet *packet)
{
	printf("in recvplatform\n");
	NetPlatform* np = (NetPlatform*)packet->data;

	if (hosting)
	{
		rakPeer->Send((char*)np,sizeof(NetPlatform) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	Platform* p = new Platform(np->position, np->orientation);
	this->signals.recvPlatform(p);
	//this->game_obj->game=true;
	//TODO
}


void NetworkTestStuff::printScores()
{
	std::cout << "Team Scores" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		printf("Team %d: %d\n",i, teamScores[i]);
	}
}

void NetworkTestStuff::receiveNewExplosion(RakNet::Packet *packet)
{
	std::cout << "Received Explosion at: ";
	NetExplosion* exp = (NetExplosion*)packet->data;
	if (exp->isMassive) std::cout << "MAHOOOOSIVE EXPLOOOSION" << std::endl;
	Ogre::Vector3 v = exp->vector;

	std::cout << "(" << v.x << "," << v.y << "," << v.z << ")"  << std::endl;
	if (hosting)
	{
		std::cout << "Broadcasting explosion" << std::endl;
		rakPeer->Send((char*)exp,sizeof(*exp) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	else
	{
		if (exp->rocketID > 0)
		{
			std::cout << "Need to destroy rocket id " << exp->rocketID <<std::endl;

			//find rocket from id

			std::tr1::unordered_map<uint64_t, Rocket*>::iterator it = RocketByID.find(exp->rocketID);
			Rocket* r = it->second;
			
			if (!(it == RocketByID.end()))
			{
				v = r->position;
				std::cout << "Rocket to destroy at (" << v.x << "," << v.y << "," << v.z << ")"  << std::endl;
				r->timer = 2000;
				//r->explode();
				//this->gamett_obj->removeQueue.insert(r);

			}	
			



		}
	}
	this->signals.explosion(v.x, v.y, v.z, exp->isMassive);
}

void NetworkTestStuff::receiveNewRocket(RakNet::Packet *packet)
{
	
	NetRocket* rocket = (NetRocket*)packet->data;
	
	if (hosting)
	{
		rocket->rocketID=rocketCounter;
		rocketCounter++;
		std::cout << "Broadcasting Rocket " << rocket->rocketID << std::endl;
		rakPeer->Send((char*)rocket,sizeof(*rocket) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	else
	{
		std::cout << "Received Rocket ID "<< rocket->rocketID << std::endl;
	}
	this->signals.recvRocket(rocket->position, rocket->orientation, rocket->rocketID);
}

void NetworkTestStuff::connectedToServer(RakNet::Packet *packet)
{
	std::cout << "ID_CONNECTION_REQUEST_ACCEPTED" << std::endl;
	std::cout << "Connected To: ";
	std::cout << packet->guid.ToString() << std::endl;
}

void NetworkTestStuff::clientConnected(RakNet::Packet *packet)
{
	std::cout << "ID_NEW_INCOMING_CONNECTION" << std::endl;
	std::cout << "From: " << packet->guid.ToString() << std::endl;

	sendWorld(packet);
	// send all the world state

	// world state sent - create and assign player
	Player *p = new Player(Ogre::Vector3(0,0,0));
	NetPlayer* np = new NetPlayer;
	sprintf(np->name,"Player %d",playerCounter);
	np->player = p;
	np->playerID = playerCounter;
	//TODO: the % 2 defines how many teams there are, should make it dynamic in some way
	np->team = playerCounter % 2;
	playerCounter++;
	np->GUID=packet->guid.g;
	insertNetPlayer(np);
	sendNetPlayer(np, ID_INSERT_PLAYER); //Creates new player across all clients
	sendNetPlayer(np, ID_ASSIGN_PLAYER, packet->guid); //assigns new player to client
	this->tb->isPaused=false;

	//Notify Of Complete Client Connection
	/*std::stringstream msgstream;
	msgstream << "Welcome to the server, you are ";
	msgstream << np->name;
	sendChat(msgstream.str(),packet->guid);*/
}

void NetworkTestStuff::sendNetPlayer(NetPlayer* np, unsigned char type, RakNet::AddressOrGUID g)
{
	np->typeId=type;
	rakPeer->Send((char*)np,sizeof(*np) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, g, false);
}

void NetworkTestStuff::sendNetPlayer(NetPlayer* np, unsigned char type)
{
	np->typeId=type;
	rakPeer->Send((char*)np,sizeof(*np) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkTestStuff::receiveAssignPlayer(RakNet::Packet *packet)
{
	NetPlayer* inc = (NetPlayer*)packet->data;
	NetPlayer* np = getNetPlayer(inc->playerID);
	myNetPlayer = np;
	Player* p = np->player;
	np->player->spawnID = inc->playerID;
	setLocalPlayer(p);
	std::cout << "Being assigned Player '" << np->name << "'" << std::endl;
	this->signals.assignLocalPlayer(p);
	shouldUpload=true;

	//this->localPlayer = np;
}

void NetworkTestStuff::receiveInsertPlayer(RakNet::Packet *packet)
{
	NetPlayer* inc = (NetPlayer*)packet->data;

	Player* p = new Player(spawn_points[inc->playerID % 8]);
	std::cout<<"-------------------!!!!!!!!!!!!!!!!_---------------" <<std::endl;
	NetPlayer* np = new NetPlayer;
	np->team = inc->team;
	np->GUID = inc->GUID;
	strncpy(np->name,inc->name,NAME_LENGTH);
	np->playerID = inc->playerID;
	np->player=p;
	insertNetPlayer(np);
}

void NetworkTestStuff::clientDisconnected(RakNet::Packet *packet)
{
	NetPlayer* np = getNetPlayer(packet);
	std::cout << np->name << " Disconnected" << std::endl;
	sendNetPlayer(np, ID_DESTROY_PLAYER);
	this->deleteNetPlayer(np);
}

void NetworkTestStuff::disconnectedFromServer(RakNet::Packet *packet)
{
	std::cout << "Disconnected From Server" << std::endl;
}

NetPlayer* NetworkTestStuff::getNetPlayer(RakNet::Packet *packet)
{
	std::tr1::unordered_map<uint64_t, NetPlayer*>::iterator it = NetPlayerByGUID.find(packet->guid.g);
	NetPlayer* np = it->second;
	if (it == NetPlayerByGUID.end())
		return 0;
	return np;
}

NetPlayer* NetworkTestStuff::getNetPlayer(int playerID)
{
	//std::cout << "Searching for " << playerID <<std::endl;
	std::tr1::unordered_map<int, NetPlayer*>::iterator it = NetPlayerByPlayerID.find(playerID);
	NetPlayer* np = it->second;
	if (it == NetPlayerByPlayerID.end()) 
		return 0;
	return np;
}

void NetworkTestStuff::receiveChat(RakNet::Packet *packet)	
{
	std::cout << "Received Message From '" << packet->guid.ToString() << "'" << std::endl;
	std::cout << packet->data + 1 << std::endl;
}

unsigned NetworkTestStuff::registerObject(Object *object)
{
	++this->lastID;

	unsigned id = this->lastID;

	this->objectsByID[id] = object;
	this->IDsByObject[object] = id;

	return id;
}

void NetworkTestStuff::sendWorld(RakNet::Packet *packet)
{
	this->sendPlayers(packet);
	this->sendFullTower(packet);
}

void NetworkTestStuff::sendPlayers(RakNet::Packet *packet)
{
	std::cout << "Begin listing:" <<std::endl;
	std::tr1::unordered_map<int, NetPlayer*>::const_iterator It;
	for (It = NetPlayerByPlayerID.begin(); It != NetPlayerByPlayerID.end(); ++It)
	{
		sendNetPlayer(It->second,ID_INSERT_PLAYER,packet->guid);
	}
	std::cout << "End listing:" <<std::endl;
}

void NetworkTestStuff::sendFullTower(RakNet::Packet *packet)
{
	broadcastUpdateTower(0,this->tb->tower->levels, 0,this->tb->tower->layers);
}

void NetworkTestStuff::broadcastUpdateTower(int low_level, int high_level, int low_layer, int high_layer)
{
	RakNet::BitStream b;
	b.Write(ID_UPDATE_TOWER);
	b.Write(low_layer);
	b.Write(low_level);
	b.Write(high_layer);
	b.Write(high_level);
	if (rakPeer > 0)
	{
		Tower* tower = this->tb->tower;
		int totalCount = 0;
		unsigned long totalActive = 0;
		for (int i =low_level; i < high_level; i++)
		{
			for (int j =low_layer; j < high_layer; j++)
			{
				for (int k =0; k < tower->blocks[i][j].size() ; k++)
				{
					if (tower->blocks[i][j][k])
					{
						b.Write(true);
						totalActive++;
					}
					else
					{
						b.Write(false);
					}

					totalCount++;
				}
			}
		}

		rakPeer->Send(&b , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		//printf("Sending %d blocks, %d active.\n",totalCount,totalActive);
		//printf("(%d, %d, %d, %d)\n",low_level, high_level, low_layer, high_layer);
	}
}

void NetworkTestStuff::receiveUpdateTower(RakNet::Packet *packet)
{
	int low_level, high_level, low_layer, high_layer;
	Tower* tower = this->tb->tower;
	RakNet::BitStream b(packet->data, packet->length, false);
	printf("Incoming tower update: %d bytes\n",packet->length);
	unsigned char head;
	b.Read(head);
	b.Read(low_layer);
	b.Read(low_level);
	b.Read(high_layer);
	b.Read(high_level);
	printf("(%d, %d, %d, %d)\n",low_level, high_level, low_layer, high_layer);
	bool temp;
	unsigned long totalActive = 0;
	int totalCount = 0;
	for (int i =low_level; i < high_level; i++)
	{
		for (int j =low_layer; j < high_layer; j++)
		{
			for (int k =0; k < tower->blocks[i][j].size(); k++)
			{
				b.Read(temp);
				if (temp)
				{
					tower->blocks[i][j][k] = true;
					totalActive ++;
				}
				else
				{
					tower->blocks[i][j][k] = false;
				}
				totalCount++;
			}
		}
	}
	tower->signals.updated(tower, BoundingVolume(low_level,high_level,low_layer,high_layer,0,tower->sectors), -totalCount);
	printf("Updated %d blocks, total active: %d\n", totalCount, totalActive);
}


void NetworkTestStuff::sendPlayerUpdate(NetPlayer* np)
{
	PlayerInfo pi;
	pi.playerID = np->playerID;
	pi.position = np->player->position;
	pi.velocity = np->player->velocity;
	//std::cout << "sending velocity: " << np->player->velocity << std::endl;
	pi.orientation = np->player->orientation;
	pi.relativeAim = np->player->relativeAim;
	pi.typeID = ID_UPDATE_PLAYER;
	pi.turretID=np->player->inTurretNum;
	pi.isInTurret=np->player->isInTurret;
	rakPeer->Send((char*)&pi,sizeof(pi) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	//std::cout << "Sending velocity: ";
	//Ogre::Vector3 v = np->player->velocity;
	//std::cout << "(" << v.x << "," << v.y << "," << v.z << ")"  << std::endl;
}

void NetworkTestStuff::receiveUpdatePlayer(RakNet::Packet *packet)
{
	PlayerInfo* pi = (PlayerInfo*) packet->data;
	//std::cout << "Updating ID " << pi->playerID << std::endl;
	NetPlayer* np = getNetPlayer(pi->playerID);
	if (np != myNetPlayer)
	{
		//std::cout << "Updating '" << np->name << "'" << std::endl;
		Player* p =np->player;
		p->setState(pi->position, pi->velocity, pi->orientation);
		/*p->position = pi->position;
		p->velocity = pi->velocity;
		p->orientation = pi->orientation;
		p->relativeAim = pi->relativeAim;*/

		if (hosting)
		{
			rakPeer->Send((char*)pi,sizeof(*pi) , HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}
		else
		{
			//Update turret if applicable
			if (pi->isInTurret)
			{
				//{printf("Player %d is in turret %d, updating orientation\n",pi->playerID, pi->turretID);
			this->turrets[pi->turretID]->updateOrient(pi->orientation, pi->relativeAim);
			}
		}
	}
}

void NetworkTestStuff::receiveDestroyPlayer(RakNet::Packet *packet)
{
	NetPlayer* inc = (NetPlayer*)packet->data;
	NetPlayer* np = getNetPlayer(inc->playerID);
	std::cout << np->name << " Disconnected" << std::endl;
	this->deleteNetPlayer(np);
}