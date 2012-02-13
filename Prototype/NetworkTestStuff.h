#ifndef __NetworkTestStuff_h_
#define __NetworkTestStuff_h_

#include <string>
#include <map>

#include <boost/signals.hpp>

#include "Updatable.h"

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
unsigned char typeId; // Your type here
double x;
double y;
double z;
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

    void startServer();
    void startClient();

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
    void receivePlayer(RakNet::Packet *packet);

    //
    void sendWorld();
    void sendPlayers();
    void sendRockets() {};
    void sendExplosions() {};
    void sendPlatforms() {};

    bool hosting;
};

#endif // #ifndef __NetworkTestStuff_h_
