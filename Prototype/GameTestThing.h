#ifndef __GameTestThing_h_
#define __GameTestThing_h_

#include <string>
#include <set>

#include "Updatable.h"

class Game;
class NetworkTestStuff;
class Turret;
class Player;
class Platform;
class Rocket;
class Explosion;
#include "include/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

class GameTestThing : public Updatable
{
public:
    GameTestThing(Game *game);
    ~GameTestThing();
	irrklang::ISoundEngine* se;
	irrklang::ISound *sound;
    irrklang::ISound *sound2;

    virtual void update();
protected:
    Game *game;
    NetworkTestStuff *network;
    Player *player;
private:
    void turretFired(Turret *turret, Rocket *rocket);
    void playerFired(Player *player, Rocket *rocket);
    void rocketExploded(Rocket *rocket, Explosion *explosion);
    void explosionFinished(Explosion *explosion);
    void platformCreated(Player *player, Platform *platform);
    void platformExpired(Platform *platform);

    void chatReceived(std::string message);

    std::set<Updatable *> removeQueue;
};

#endif // #ifndef __GameTestThing_h_
