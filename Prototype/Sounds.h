#ifndef __Sounds_h_
#define __Sounds_h_

#include <boost/signals.hpp>

#include "include/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

class Player;
class Turret;
class Rocket;

class Sounds
{
public:
    Sounds(Player *player);
    virtual ~Sounds();

    irrklang::ISoundEngine *engine;
protected:
    virtual void listenerUpdated(Player *player);
};

#endif // #ifndef __Sounds_h_