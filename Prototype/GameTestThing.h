#ifndef __GameTestThing_h_
#define __GameTestThing_h_

#include <set>

#include "Updatable.h"

class Game;
class Player;
class Platform;
class Rocket;
class Explosion;

class GameTestThing : public Updatable
{
public:
    GameTestThing(Game *game);
    ~GameTestThing();

    virtual void update();
protected:
    Game *game;
private:
    void playerFired(Player *player, Rocket *rocket);
    void rocketExploded(Rocket *rocket, Explosion *explosion);
    void explosionFinished(Explosion *explosion);
    void platformCreated(Player *player, Platform *platform);
    void platformExpired(Platform *platform);

    std::set<Updatable *> removeQueue;
};

#endif // #ifndef __GameTestThing_h_
