#ifndef __GameTestThing_h_
#define __GameTestThing_h_

#include <set>

#include "Updatable.h"

class Game;
class Player;
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

    unsigned timer;
private:
    void playerFired(Player *player, Rocket *rocket);
    void rocketExploded(Rocket *rocket, Explosion *explosion);
    void explosionFinished(Explosion *explosion);

    Rocket *rocket;

    std::set<Updatable *> removeQueue;
};

#endif // #ifndef __GameTestThing_h_
