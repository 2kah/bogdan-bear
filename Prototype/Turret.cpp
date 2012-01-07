#include "Turret.h"

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Rocket.h"

Turret::Turret(Ogre::Vector3 position, Ogre::Quaternion orientation)
{
    this->position = position;
    this->orientation = orientation;

    this->timer = 0;
}

Turret::~Turret()
{
}

void Turret::update()
{
    this->signals.updated(this);

    if (this->timer >= 500)
    {
        this->timer = 0;

        this->signals.fired(this, new Rocket(this->position, this->orientation));
    }
    else
    {
        ++this->timer;
    }
}
