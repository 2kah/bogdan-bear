#ifndef __NetworkTestStuff_h_
#define __NetworkTestStuff_h_

#include <string>
#include <map>

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
    boost::signal<void (Player *player)> playerReplicated;
    boost::signal<void (Player *player)> playerCreated;
    boost::signal<void (Player *player)> playerDestroyed;
    boost::signal<void (Player *player)> localPlayerAssigned;

    boost::signal<void (std::string message)> chat;
    boost::signal<void (double x, double y, double z)> explosion;
};
}

#pragma pack(push, 1)
struct Coords3D
{
unsigned char typeId; 
double x;
double y;
double z;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct NetPlayer
{
unsigned char typeId;
int playerID;
Ogre::Vector3 position;
Ogre::Quaternion orientation;
Ogre::Vector3 velocity;
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
    virtual void update();
	virtual void sendPlayer(unsigned char packetType, int playerID, Ogre::Vector3 position, Ogre::Quaternion orientation, Ogre::Vector3 velocity);

    virtual void sendExplosion(double x, double y, double z);

    // object index
    unsigned lastID;
    std::map<unsigned, Object *> objectsByID;
    std::map<Object *, unsigned> IDsByObject;

    unsigned registerObject(Object *);

    //
    void clientConnected();
    void clientDisconnected();

    void sendPlayer(Player *player, bool existing);
    void receiveNewPlayer(RakNet::Packet *packet);
	void receiveExistingPlayer(RakNet::Packet *packet);
	void receiveUpdatePlayer(RakNet::Packet *packet);
	void receiveDestroyPlayer(RakNet::Packet *packet);

	void receiveNewExplosion(RakNet::Packet *packet);

    //
    void sendWorld();
    void sendPlayers();
    void sendRockets() {};
    void sendExplosions() {};
    void sendPlatforms() {};

    bool hosting;
};

#endif // #ifndef __NetworkTestStuff_h_
