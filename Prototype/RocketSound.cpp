#include "RocketSound.h"

#include "include/irrKlang.h"

#include "BtOgreExtras.h"

#include "Rocket.h"
#include "Explosion.h"

RocketSound::RocketSound(Rocket *rocket, irrklang::ISoundEngine *soundEngine)
    : rocket(rocket)
    , soundEngine(soundEngine)
{
    this->sound = this->soundEngine->play3D("sounds/play.mp3", BtOgre::Convert::toIrrKlang(rocket->position));

    this->rocket->signals.exploded.connect(boost::bind(&RocketSound::rocketExploded, this, _1, _2));
    this->rocket->signals.updated.connect(boost::bind(&RocketSound::rocketUpdated, this, _1));
}

RocketSound::~RocketSound()
{
    // stop sounds / clear up here
}

void RocketSound::rocketUpdated(Rocket *rocket)
{
    // update sound position here
}

void RocketSound::rocketExploded(Rocket *rocket, Explosion *explosion)
{
    //delete this;
}
