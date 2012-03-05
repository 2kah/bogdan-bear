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

class Object;
class Player;

namespace {
struct NetworkSignals {
    //boost::signal<void (Player *player)> playerReplicated;
	boost::signal<void (Player *player)> addPlayer;
    boost::signal<void (Player *player)> playerCreated;
    boost::signal<void (Player *player)> playerDestroyed;
    //boost::signal<void (Player *player)> localPlayerAssigned;
	boost::signal<void (Player *player)> assignLocalPlayer;

    boost::signal<void (std::string message)> chat;
    boost::signal<void (double x, double y, double z)> explosion;
};
}

#pragma pack(push, 1)
struct NetExplosion
{
unsigned char typeId;
Ogre::Vector3 vector;
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
	char name [NAME_LENGTH];
	uint64_t GUID;
	Player* player;
};
#pragma pack(pop)



class NetworkTestStuff : public Updatable
{
public:
    NetworkTestStuff();
    virtual ~NetworkTestStuff();

    NetworkSignals signals;

	virtual void startNetwork(bool asServer);
	virtual void sendChat(std::string message);
	virtual void sendChat(std::string message, RakNet::AddressOrGUID target);
    virtual void update();

    virtual void sendExplosion(double x, double y, double z);

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
	void listNetPlayers();
	void sendNetPlayer(NetPlayer* np, unsigned char type, RakNet::AddressOrGUID g);
	void sendNetPlayer(NetPlayer* np, unsigned char type);

    void sendPlayerUpdate(NetPlayer* np);

	void receiveAssignPlayer(RakNet::Packet *packet);
	void receiveInsertPlayer(RakNet::Packet *packet);
	void receiveUpdatePlayer(RakNet::Packet *packet);
	void receiveDestroyPlayer(RakNet::Packet *packet);

	void receiveNewExplosion(RakNet::Packet *packet);
	void receiveChat(RakNet::Packet *packet);

    //
    void sendWorld(RakNet::Packet *packet);
    void sendPlayers(RakNet::Packet *packet);
    void sendRockets() {};
    void sendExplosions() {};
    void sendPlatforms() {};

    bool hosting;
};

#endif // #ifndef __NetworkTestStuff_h_
