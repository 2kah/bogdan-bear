#ifndef __PlayerSound_h_
#define __PlayerSound_h_

#include "include/irrKlang.h"

class Player;
class Explosion;

class PlayerSound
{
public:
    PlayerSound(Player *player);
    virtual ~PlayerSound();
protected:
    virtual void PlayerWalk(Player *player);
    virtual void PlayerStop(Player *player);

    Player *player;
    irrklang::ISoundEngine *soundEngine;
    irrklang::ISound *sound;
	irrklang::ISound *exp;
};

#endif // #ifndef __RocketSound_h_
