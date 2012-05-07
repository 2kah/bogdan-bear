#ifndef __NetworkTestStuff_h_
#define __NetworkTestStuff_h_

#define NAME_LENGTH 32
#include <string>
#include <map>
#include <iostream>

#include <boost/signals.hpp>

#include "RakNetTypes.h"

#include "Updatable.h"
#include <vector>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>
#include "TowerBuilder.h"
#include "Goal.h"

class Object;
class Player;


namespace {
struct NetworkSignals {
    //boost::signal<void (Player *player)> playerReplicated;
	boost::signal<void (Player *player, int team)> addPlayer;
    boost::signal<void (Player *player)> removePlayer;
	boost::signal<void (Player *player)> assignLocalPlayer;

    boost::signal<void (std::string message)> chat;
    boost::signal<void (double x, double y, double z)> explosion;
	boost::signal<void (Ogre::Vector3 position, Ogre::Quaternion orientation)> recvRocket;
};
}

#pragma pack(push, 1)
struct NetTower
{
	unsigned char typeId;
	int low_level;
	int high_level;
	int low_layer;
	int high_layer;
	bool data [840000];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct NetExplosion
{
unsigned char typeId;
Ogre::Vector3 vector;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct NetRocket
{
unsigned char typeId;
Ogre::Vector3 position;
Ogre::Quaternion orientation;	
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PlayerInfo
{
	unsigned char typeID;
	int playerID;
	Ogre::Vector3 position;
	Ogre::Vector3 velocity;
	Ogre::Quaternion orientation;
	Ogre::Quaternion relativeAim;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct NetPlayer
{
	unsigned char typeId;
	int playerID;
	int team;
	char name [NAME_LENGTH];
	uint64_t GUID;
	Player* player;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct NetPlatform
{
	unsigned char typeId;
	int playerID;
	char name [NAME_LENGTH];
	uint64_t GUID;
	Player* player;
};
#pragma pack(pop)



class NetworkTestStuff : public Updatable
{
public:
    // for spawn positions
	float spawn_angles[16];
    int current_spawn;
    //

    bool hosting;
	Goal* g;
	TowerBuilder* tb;
	NetTower towerBuffer;
    NetworkSignals signals;
	int teamScores[4];

    NetworkTestStuff();
    virtual ~NetworkTestStuff();
	
	void broadcastUpdateTower(int low_level, int high_level, int low_layer, int high_layer);
	void sendFullTower(RakNet::Packet *packet);
	void receiveUpdateTower(RakNet::Packet *packet);
	void updateScores();
	virtual void startNetwork(bool asServer);
	virtual void sendChat(std::string message);
	virtual void sendChat(std::string message, RakNet::AddressOrGUID target);
    virtual void update();

    virtual void sendExplosion(Ogre::Vector3 position);
	virtual void sendRocket(Ogre::Vector3 position, Ogre::Quaternion orientation);

    // object index
    unsigned lastID;
    std::map<unsigned, Object *> objectsByID;
    std::map<Object *, unsigned> IDsByObject;

    unsigned registerObject(Object *);

    //
    void clientConnected(RakNet::Packet *packet);
    void clientDisconnected(RakNet::Packet *packet);
	void connectedToServer(RakNet::Packet *packet);
	void disconnectedFromServer(RakNet::Packet *packet);

	NetPlayer* getNetPlayer(RakNet::Packet *packet);
	NetPlayer* getNetPlayer(int playerID);

	void insertNetPlayer(NetPlayer* np);
	void deleteNetPlayer(NetPlayer* np);
	void listNetPlayers();
	void sendNetPlayer(NetPlayer* np, unsigned char type, RakNet::AddressOrGUID g);
	void sendNetPlayer(NetPlayer* np, unsigned char type);

    void sendPlayerUpdate(NetPlayer* np);

	void receiveAssignPlayer(RakNet::Packet *packet);
	void receiveInsertPlayer(RakNet::Packet *packet);
	void receiveUpdatePlayer(RakNet::Packet *packet);
	void receiveDestroyPlayer(RakNet::Packet *packet);

	void receiveNewExplosion(RakNet::Packet *packet);
	void receiveNewRocket(RakNet::Packet *packet);
	void receiveChat(RakNet::Packet *packet);

    //
    void sendWorld(RakNet::Packet *packet);
    void sendPlayers(RakNet::Packet *packet);
    void sendRockets() {};
    void sendExplosions() {};
    void sendPlatforms() {};
	void printScores();

};

#endif // #ifndef __NetworkTestStuff_h_
