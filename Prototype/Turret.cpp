#include "Turret.h"
#include <math.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Rocket.h"
#include "Player.h"
#include "NetworkTestStuff.h"

#define PI 3.14159265

Turret::Turret(Ogre::Vector3 position, Ogre::Quaternion orientation, NetworkTestStuff* net_stuff)
    : Object(position, orientation)
    , timer(0)
    //, target(NULL)
	, nptarget(0,100,0)
	, oldtarget(0,0,0)
	, currenttarget(0,0,0)
	, occ(false)
	, rockets(0)
	, playerTimer(0)
	, network_obj(net_stuff)
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
	if(!isClientSide && !occ)
	{
        //if (this->target != NULL) {
	    //	d = this->target - this->position;
	    //}
	   // else {
	    	this->currenttarget = this->currenttarget + ((this->nptarget - this->oldtarget)/600);
	    	d = this->currenttarget - this->position;
	    //}
	    
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
	    	//if (this->target == NULL) {
	    	    this->oldtarget = this->nptarget;
	    	    this->nptarget = Ogre::Vector3(rand() % 150 - 75, rand() % 500 - 150, rand() % 150 - 75);
	    	//}
            this->signals.fired(this, new Rocket(this->position, this->orientation));
        }
        else
        {
            ++this->timer;
        }
		if(this->playerTimer > 0)
			this->playerTimer--;
	}
	else
	{
		Ogre::Quaternion orientation = occupantOrientation * occupantRelativeAim;
		orientation = orientation * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);
		this->orientation = orientation;
		this->signals.updated(this);
	}
}

void Turret::setTarget(Ogre::Vector3 target)
{
    this->currenttarget = target;
}

bool Turret::setOccupied(bool set, Player *player)
{
	//if someone is trying to enter the turret, but it's already inhabited, return false
	if(set && this->occ)
		return false;
	//if the same player is trying to enter, but the timer isn't finished, return false
	if(player == this->player && this->playerTimer != 0)
		return false;
	//player is getting out of the turret, set the timer for 10 seconds
	if(!set)
		this->playerTimer = 1000;
	this->occ = set;
	this->player = player;
	//TODO: tweak
	rockets = 10;
	return true;
}

bool Turret::isOccupied()
{
    return this->occ;
}

void Turret::fireTurret() 
{
	if (this->isClientSide)
	{
		
		if (this->network_obj > 0)
		{
			printf("SENDING NETWORKED TURRET ROCKET (sorry for caps)\n");
		this->network_obj->sendRocket(this->position, this->orientation);
		}
		else
		{
			printf("network fail\n");
		}
		
	}
	else
	{
		this->signals.fired(this, new Rocket(this->position, this->orientation));
	}
	//
	
	this->rockets--;
	//if the player has run out of rockets, eject them from the turret
	if(this->rockets == 0)
		this->player->use(true);
}

void Turret::setOccupant(Ogre::Quaternion newOO, Ogre::Quaternion newRA)
{
	occupantOrientation = newOO;
	occupantRelativeAim = newRA;
}
