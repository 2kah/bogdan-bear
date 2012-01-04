#include "Rocket.h"

#include "Explosion.h"

Rocket::Rocket(Ogre::Vector3 position)
{
    this->position = position;

    this->timer = 0;
}

Rocket::~Rocket()
{
}

void Rocket::update()
{
    if (this->timer >= 300)
    {
        this->timer = 0;

        this->signals.exploded(this, new Explosion(this->position));
    }
    else
    {
        ++this->timer;
    }
}
