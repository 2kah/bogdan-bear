#include "Rocket.h"

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Explosion.h"

const double Rocket::SPEED = 1.0;

Rocket::Rocket(Ogre::Vector3 position, Ogre::Quaternion orientation)
{
    this->position = position;
    this->orientation = orientation;

    this->timer = 0;
}

Rocket::~Rocket()
{
}

void Rocket::update()
{
    this->position = this->position + this->orientation * Ogre::Vector3::UNIT_X * Rocket::SPEED;
    this->signals.updated(this);

    if (this->timer >= 1000)
    {
        this->timer = 0;

        this->explode();
    }
    else
    {
        ++this->timer;
    }
}

void Rocket::explode()
{
    this->signals.exploded(this, new Explosion(this->position));
}
