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
    if (this->target != NULL) {
        this->orientation = this->position.getRotationTo((this->target->position + Ogre::Vector3::UNIT_Y * 4) - this->position, Ogre::Vector3::UNIT_Y) * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y);
    }
    
    this->signals.updated(this);

    if (this->timer >= 300)
    {
        this->timer = 0;

        this->signals.fired(this, new Rocket(this->position, this->orientation));
    }
    else
    {
        ++this->timer;
    }
}

void Turret::setTarget(Object *target)
{
    this->target = target;
}
