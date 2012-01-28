#include "Turret.h"
#include <math.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Rocket.h"

#define PI 3.14159265

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
		double pitch;
		Ogre::Vector3 d = this->target->position - this->position;
        //this->orientation = this->position.getRotationTo((this->target->position + Ogre::Vector3::UNIT_Y * 4) - this->position, Ogre::Vector3::UNIT_Y) * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y);
        double yaw = (atan (d.x/d.z) * 180 / PI)+90;
		if(d.y < 0) pitch = (atan (d.z/d.y) * 180 / PI)-90;
		else pitch = (atan (d.z/d.y) * 180 / PI) + 90;

		this->orientation = Ogre::Quaternion(Ogre::Degree(yaw), Ogre::Vector3::UNIT_Y);
		this->orientation = this->orientation * Ogre::Quaternion(Ogre::Degree(pitch), Ogre::Vector3::UNIT_Z);
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
