#ifndef __RocketSound_h_
#define __RocketSound_h_

#include "include/irrKlang.h"

class Rocket;
class Explosion;

class RocketSound
{
public:
    RocketSound(Rocket *rocket, irrklang::ISoundEngine *soundEngine);
    virtual ~RocketSound();
protected:
    virtual void rocketUpdated(Rocket *rocket);
    virtual void rocketExploded(Rocket *rocket, Explosion *explosion);

    Rocket *rocket;
    irrklang::ISoundEngine *soundEngine;
    irrklang::ISound *sound;
};

#endif // #ifndef __RocketSound_h_
