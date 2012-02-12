#ifndef __GameTestThing_h_
#define __GameTestThing_h_

#include <string>
#include <vector>
#include <set>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Updatable.h"

class Game;
class NetworkTestStuff;
class Sounds;
class Turret;
class Player;
class Platform;
class Rocket;
class Explosion;
class TowerBuilder;

class GameTestThing : public Updatable
{
public:
    GameTestThing(Game *game);
    ~GameTestThing();

    virtual void startLocal();
    virtual void startServer();
    virtual void startClient();

    virtual void update();

	virtual void netSendChat(std::string message);
	virtual void netSendExplosion(double x, double y, double z);

    virtual void addPlayer(Player *player);
    virtual void addTurret(Turret *turret);
    virtual void addRocket(Rocket *rocket);
    virtual void addExplosion(Explosion *explosion);

protected:
    Game *game;
    NetworkTestStuff *network;
    Player *player;
    Sounds *sounds;
	TowerBuilder *towerBuilder;

    std::set<Turret *> turrets;
private:
    void turretFired(Turret *turret, Rocket *rocket);
    void playerFired(Player *player, Rocket *rocket);
    void rocketExploded(Rocket *rocket, Explosion *explosion);
    void explosionFinished(Explosion *explosion);
    void platformCreated(Player *player, Platform *platform);
    void platformExpired(Platform *platform);

    void chatReceived(std::string message);
    void networkExplosion(double x, double y, double z);

    void playerUsed(Player *player);

    std::set<Updatable *> removeQueue;

// networky stuff
    void setLocalPlayer(Player *player);

    Player *replicatePlayer(Ogre::Vector3 position, Ogre::Vector3 velocity, Ogre::Quaternion orientation) {};

    std::vector<Player *> players;

    Player *localPlayer;
};

#endif // #ifndef __GameTestThing_h_
