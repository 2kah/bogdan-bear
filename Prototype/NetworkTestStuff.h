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
#include "Turret.h"

class Object;
class Player;
class Game;


namespace {
struct NetworkSignals {
    //boost::signal<void (Player *player)> playerReplicated;
	boost::signal<void (Player *player, int team)> addPlayer;
    boost::signal<void (Player *player)> removePlayer;
	boost::signal<void (Player *player)> assignLocalPlayer;

    boost::signal<void (std::string message)> chat;
    boost::signal<void (double x, double y, double z, bool isBig)> explosion;
	boost::signal<void (Ogre::Vector3 position, Ogre::Quaternion orientation)> recvRocket;
	boost::signal<void (Platform *p)> recvPlatform;
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
bool isMassive;
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
struct NetPlatform
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
	int turretID;
	bool isInTurret;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TurretInfo
{
	unsigned char typeID;
	int turretID;
	bool turretBusy;
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

struct NetScore
{
	unsigned char typeId;
	int scores[4];
};
#pragma pack(pop)

static const unsigned short SERVER_PORT=12345;

static const unsigned char ID_TEXT = 140;

static const unsigned char ID_NEW_EXPLOSION = 141;
static const unsigned char ID_EXIST_EXPLOSION = 142;
static const unsigned char ID_UPDATE_EXPLOSION = 143;
static const unsigned char ID_DESTROY_EXPLOSION = 144;

static const unsigned char ID_ASSIGN_PLAYER = 151;
static const unsigned char ID_INSERT_PLAYER = 152;
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

static const unsigned char ID_UPDATE_TOWER = 175;
static const unsigned char ID_UPDATE_SCORES = 176;
static const unsigned char ID_START_GAME = 186;
static const unsigned char ID_TURRET_BUSY = 187;
static const unsigned char ID_TURRET_UPDATE = 188;

class NetworkTestStuff : public Updatable
{
public:
	//NetPlayer* myNetPlayer;
    // for spawn positions
	Ogre::Vector3 spawn_points[8];
    int current_spawn;
    //

	char *SERVER_IP_ADDRESS;

    bool hosting;
	Goal* g;
	TowerBuilder* tb;
	Game* game_obj;
	NetTower towerBuffer;
    NetworkSignals signals;
	int teamScores[4];
	Turret* turrets[4];
    NetworkTestStuff(char *hostIP);
    virtual ~NetworkTestStuff();
	void setLocalPlayer(Player * p);
	void broadcastUpdateTower(int low_level, int high_level, int low_layer, int high_layer);
	void sendFullTower(RakNet::Packet *packet);
	void receiveUpdateTower(RakNet::Packet *packet);
	void updateScores();
	virtual void startNetwork(bool asServer);
	virtual void sendChat(std::string message);
	virtual void sendChat(std::string message, RakNet::AddressOrGUID target);
    virtual void update();

	virtual void sendTurretBusy(int turretID, bool busy);
	virtual void recvTurretBusy(RakNet::Packet *packet);
	virtual void sendTurretInfo(TurretInfo ti);

	virtual void sendTurretUpdate(int turretID);
	virtual void recvTurretUpdate(RakNet::Packet *packet);

	virtual void sendStartGame();

    virtual void sendExplosion(Ogre::Vector3 position);
	virtual void sendExplosion(Ogre::Vector3 position, bool isMassive);
	virtual void sendRocket(Ogre::Vector3 position, Ogre::Quaternion orientation);
	virtual void sendNetScores();
	virtual void sendPlatform(Ogre::Vector3 position, Ogre::Quaternion orientation);
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
	void receiveScores(RakNet::Packet *packet);
	void recvStartGame(RakNet::Packet *packet);
	void recvPlatform(RakNet::Packet *packet);

    //
    void sendWorld(RakNet::Packet *packet);
    void sendPlayers(RakNet::Packet *packet);
    void sendRockets() {};
    void sendExplosions() {};
    void sendPlatforms() {};
	void printScores();

	NetPlayer* localPlayer;
	NetPlayer* myNetPlayer;

};

#endif // #ifndef __NetworkTestStuff_h_
