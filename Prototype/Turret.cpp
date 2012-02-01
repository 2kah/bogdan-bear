#include "Turret.h"
#include <math.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Rocket.h"

#define PI 3.14159265

Turret::Turret(Ogre::Vector3 position, Ogre::Quaternion orientation)
    : Object(position, orientation)
    , timer(0)
    , target(NULL)
	, nptarget(0,100,0)
	, oldtarget(0,0,0)
	, currenttarget(0,0,0)
{
}

Turret::~Turret()
{
}

void Turret::update()
{
    double pitch;
	double yaw;
	Ogre::Vector3 d;
	
    if (this->target != NULL) {
		d = this->target->position - this->position;
	}
	else {
		this->currenttarget = this->currenttarget + ((this->nptarget - this->oldtarget)/600);
		d = this->currenttarget - this->position;
	}

    //this->orientation = this->position.getRotationTo((this->target->position + Ogre::Vector3::UNIT_Y * 4) - this->position, Ogre::Vector3::UNIT_Y) * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y);
    
	if(this->position.z > 0) {
		yaw = (atan (d.x/d.z) * 180 / PI)+90;
	    if(d.y < 0) pitch = (atan (d.z/d.y) * 180 / PI)-90;
	    else pitch = (atan (d.z/d.y) * 180 / PI) + 90;
	}
	else if(this->position.z < 0) {
		yaw = (atan (d.x/d.z) * 180 / PI)-90;
	    if(d.y < 0) pitch = (atan (-d.z/d.y) * 180 / PI)-90;
	    else pitch = (atan (-d.z/d.y) * 180 / PI) + 90;
	}
	if(this->position.x > 0) {
		yaw = (atan (-d.z/d.x) * 180 / PI)+180;
	    if(d.y < 0) pitch = (atan (d.x/d.y) * 180 / PI)-90;
	    else pitch = (atan (d.x/d.y) * 180 / PI) + 90;
	}
	else if(this->position.x < 0) {
		yaw = (atan (-d.z/d.x) * 180 / PI);//-90;
	    if(d.y < 0) pitch = (atan (-d.x/d.y) * 180 / PI)-90;
	    else pitch = (atan (-d.x/d.y) * 180 / PI) + 90;
	}

	this->orientation = Ogre::Quaternion(Ogre::Degree(yaw), Ogre::Vector3::UNIT_Y);
	this->orientation = this->orientation * Ogre::Quaternion(Ogre::Degree(pitch), Ogre::Vector3::UNIT_Z);

    this->signals.updated(this);

    if (this->timer >= 600)
    {
        this->timer = 0;
		if (this->target == NULL) {
		    this->oldtarget = this->nptarget;
		    this->nptarget = Ogre::Vector3(rand() % 150 - 75, rand() % 500 - 150, rand() % 150 - 75);
		}
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

bool Turret::isOccupied()
{
    return this->occ;
}