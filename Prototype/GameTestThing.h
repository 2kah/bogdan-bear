#ifndef __GameTestThing_h_
#define __GameTestThing_h_

#include <string>
#include <vector>
#include <set>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Updatable.h"
#include "Goal.h"

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

	virtual void buildScene();
	virtual void destroyScene();

    virtual void startLocal();
    virtual void startServer();
    virtual void startClient();

    virtual void update();

	virtual void netSendChat(std::string message);
	virtual void netSendExplosion(Ogre::Vector3 position);
	virtual void netSendRocket(Ogre::Vector3 position, Ogre::Quaternion orientation);

    virtual void addPlayer(Player *player);
    virtual void removePlayer(Player *player);

    virtual void addTurret(Turret *turret);
    virtual void addRocket(Rocket *rocket);
    virtual void addExplosion(Explosion *explosion);

	Goal *goal;
    Player *player;

protected:
    Game *game;
    NetworkTestStuff *network;
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
	void networkRocket(Ogre::Vector3 position, Ogre::Quaternion orientation);

    void playerUsed(Player *player);

    std::set<Updatable *> removeQueue;


// networky stuff
    void setLocalPlayer(Player *player);

    Player *replicatePlayer(Ogre::Vector3 position, Ogre::Vector3 velocity, Ogre::Quaternion orientation) {};

    std::vector<Player *> players;

    Player *localPlayer;
};

#endif // #ifndef __GameTestThing_h_
