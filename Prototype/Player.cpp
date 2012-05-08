#include "Player.h"

#include <iostream>
#include <math.h>

#include <boost/bind.hpp>

#include <OGRE/OgreQuaternion.h>

#include "PlayerInput.h"

#include "Platform.h"
#include "Rocket.h"

#include "Tower.h"

#include "Turret.h"

const double Player::MOVEMENT_SPEED = 5 / 16.0;
const double Player::ROTATION_SPEED = 0.05;

Player::Player(Ogre::Vector3 position)
    : Object(position, Ogre::Quaternion())
    , velocity(Ogre::Vector3::ZERO)
    , turret(NULL)
	, needsReload(false)
	, reloadTimer(200)
	, rocketAmmo(4)
	, canFire(true)
	, cooldown(50)
	, platformTimer(0)
{
}

Player::~Player()
{
}

void Player::setState(Ogre::Vector3 position, Ogre::Vector3 velocity, Ogre::Quaternion orientation)
{
    this->position = position;
    this->velocity = velocity;
    this->orientation = orientation;

    //this->update();

    // make sure physics updates too
    this->signals.stateSet(this);
}

void Player::update()
{
	this->orientation = this->orientation * Ogre::Quaternion(Ogre::Radian(rotX), Ogre::Vector3::UNIT_Y);
    this->relativeAim = this->relativeAim * Ogre::Quaternion(Ogre::Radian(rotY), Ogre::Vector3::UNIT_X);

	if(this->relativeAim.getPitch() > Ogre::Radian(Ogre::Math::PI / 2))
		this->relativeAim = Ogre::Quaternion(Ogre::Radian(Ogre::Math::PI / 2), Ogre::Vector3::UNIT_X);
	
	if(this->relativeAim.getPitch() < Ogre::Radian(-Ogre::Math::PI / 2))
		this->relativeAim = Ogre::Quaternion(Ogre::Radian(-Ogre::Math::PI / 2), Ogre::Vector3::UNIT_X);

    rotX = 0;
    rotY = 0;
    
    //this->position = this->position + this->orientation * this->velocity;
    
    this->signals.updated(this);

	if(!this->canFire) {
		this->cooldown--;
		if(this->cooldown <= 0) {
			this->canFire = true;
			this->cooldown = 50;
		}
	}

	if(this->needsReload) {
		this->reloadTimer--;
		if(this->reloadTimer <= 0) {
			this->rocketAmmo++;
			std::cout << "Ammo " << this->rocketAmmo << std::endl;
			this->reloadTimer = 200;
			if(this->rocketAmmo < 4) {
				this->needsReload = true;
			}
			else {
				this->needsReload = false;
			}
		}
	}

	if(this->platformTimer > 0)
		this->platformTimer--;

	if(this->turret != NULL) {
		this->turret->setOccupant(this->orientation, this->relativeAim);
	}
}

void Player::addInput(PlayerInput &input)
{
    //input.signals.move.connect(boost::bind(&Player::movement, this, _1, _2));
    input.signals.look.connect(boost::bind(&Player::look, this, _1, _2));

    input.signals.fire.connect(boost::bind(&Player::fire, this, _1));
    input.signals.create.connect(boost::bind(&Player::create, this, _1));

    //input.signals.jump.connect(boost::bind(&Player::jump, this, _1));
    input.signals.use.connect(boost::bind(&Player::use, this, _1));

	//TODO: remove these 2 (testing purposes only)
	input.signals.deactivate.connect(boost::bind(&Player::enteredTurret, this));
	input.signals.reactivate.connect(boost::bind(&Player::exitedTurret, this));
}

void Player::movement(DIRECTION direction, bool state)
{
	//velocity should be controlled by the physics
    /*if (direction == FORWARD)
    {
        this->velocity.z = -Player::MOVEMENT_SPEED * state;
    }
    else if (direction == BACKWARD)
    {
        this->velocity.z = Player::MOVEMENT_SPEED * state;
    }
    else if (direction == LEFT)
    {
        this->velocity.x = -Player::MOVEMENT_SPEED * state;
    }
    else if (direction == RIGHT)
    {
        this->velocity.x = Player::MOVEMENT_SPEED * state;
    }*/

    if (!this->stepping && this->velocity.length() > 0) {
        this->stepping = true;
        this->signals.startedStepping(this);
    } else if (this->stepping && this->velocity.length() == 0) {
        this->stepping = false;
        this->signals.stoppedStepping(this);
    }
}

void Player::look(int x, int y)
{
    this->rotX += Ogre::Degree(-x * Player::ROTATION_SPEED);
    this->rotY += Ogre::Degree(-y * Player::ROTATION_SPEED);

	this->orientation = this->orientation * Ogre::Quaternion(Ogre::Radian(rotX), Ogre::Vector3::UNIT_Y);
    this->relativeAim = this->relativeAim * Ogre::Quaternion(Ogre::Radian(rotY), Ogre::Vector3::UNIT_X);

	if(this->relativeAim.getPitch() > Ogre::Radian(Ogre::Math::PI / 2))
		this->relativeAim = Ogre::Quaternion(Ogre::Radian(Ogre::Math::PI / 2), Ogre::Vector3::UNIT_X);
	
	if(this->relativeAim.getPitch() < Ogre::Radian(-Ogre::Math::PI / 2))
		this->relativeAim = Ogre::Quaternion(Ogre::Radian(-Ogre::Math::PI / 2), Ogre::Vector3::UNIT_X);

	rotX = 0;
    rotY = 0;

	this->signals.updated(this);
}

void Player::fire(bool state)
{
    if (state) {
		if(this->turret == NULL) {
		    if(this->canFire && this->rocketAmmo > 0) {
                Ogre::Quaternion orientation = this->orientation * this->relativeAim;
                orientation = orientation * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);
		    
                this->signals.fired(this, new Rocket(this->position + Ogre::Vector3::UNIT_Y * 180 / 16.0 + (orientation * Ogre::Vector3::UNIT_X * 10), orientation));
		    	this->rocketAmmo--;
		    	std::cout << "Ammo " << this->rocketAmmo << std::endl;
		    	this->needsReload = true;
		    	this->reloadTimer = 200;
		    	this->canFire = false;
		    }
		}
		else if(this->turret != NULL) {
			this->turret->fireTurret();
		}
    }
}

void Player::create(bool state)
{
    if (state)
	{
		if(this->platformTimer == 0)
		{
			Ogre::Vector3 platformPosition = this->position;
			//make platform spawn beneath you, not inside you
			platformPosition.y -= 6;
			this->signals.platform(this, new Platform(platformPosition, this->orientation));
			//TODO: tweak
			//number of seconds until player can place another platform (* 100)
			this->platformTimer = 500;
		}
    }
}

//TODO: remove this code (redundant)
void Player::jump(bool state)
{
    this->velocity.y = Player::MOVEMENT_SPEED * state;

    this->stepping = false;
    this->signals.stoppedStepping(this);
}

void Player::use(bool state)
{
    this->signals.used(this);
}

//TODO: remove these 2 (testing purposes only)
void Player::enteredTurret()
{
	this->signals.enteredTurret(this);
}

void Player::exitedTurret()
{
	this->signals.exitedTurret(this);
}

void Player::setTurret(Turret *newTurret)
{
	this->turret = newTurret;
}

double Player::getAmmo()
{
	//double ammoClips[] = {-0.4, -0.4, -0.4, -0.4};
	//for(int i = 0; i < this->rocketAmmo; i++)
	//{
	//	ammoClips[i] = 0;
	//}
	//if(this->rocketAmmo < 4) {
	
	double	ammoClips = -0.3*(this->reloadTimer/(double)200);
	//}
	//printf("%i,   %lf\n", this->reloadTimer, -0.3*(this->reloadTimer/200));
	//printf("%lf, %lf, %lf, %lf\n", ammoClips[0], ammoClips[1], ammoClips[2], ammoClips[3]);
	return ammoClips;
}

int Player::getAmmoAmount()
{
	return this->rocketAmmo;
}

void Player::setScores(int newScores[])
{
	this->scores[0] = newScores[0];
	this->scores[1] = newScores[1];
	this->scores[2] = newScores[2];
	this->scores[3] = newScores[3];
}

int Player::getScore(int team)
{
	return this->scores[team];
}



