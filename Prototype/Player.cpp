#include "Player.h"

#include <iostream>
#include <math.h>

#include <boost/bind.hpp>

#include <OGRE/OgreQuaternion.h>

#include "PlayerInput.h"

#include "Platform.h"
#include "Rocket.h"

#include "Tower.h"

const double Player::MOVEMENT_SPEED = 5 / 16.0;
const double Player::ROTATION_SPEED = 0.05;

Player::Player(Ogre::Vector3 position)
    : Object(position, Ogre::Quaternion())
    , velocity(Ogre::Vector3::ZERO)
    , turret(NULL)
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

    rotX = 0;
    rotY = 0;
    
    //this->position = this->position + this->orientation * this->velocity;
    
    this->signals.updated(this);
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
    if (direction == FORWARD)
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
    }

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
}

void Player::fire(bool state)
{
    if (state) {
        Ogre::Quaternion orientation = this->orientation * this->relativeAim;
        orientation = orientation * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);

        this->signals.fired(this, new Rocket(this->position + Ogre::Vector3::UNIT_Y * 180 / 16.0 + (orientation * Ogre::Vector3::UNIT_X * 10), orientation));
    }
}

void Player::create(bool state)
{
    if (state) {
        this->signals.platform(this, new Platform(this->position, this->orientation));
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


