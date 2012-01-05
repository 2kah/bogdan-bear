#include "Player.h"

#include <iostream>
#include <math.h>

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <btBulletDynamicsCommon.h>

#include "Platform.h"
#include "Rocket.h"

#define PI 3.14159265

const double Player::MOVEMENT_SPEED = 5;
const double Player::ROTATION_SPEED = 0.05;

Player::Player(Ogre::Vector3 position)
{
    this->position = position;

    this->velocity = Ogre::Vector3::ZERO;
}

Player::~Player()
{
}

void Player::addToScene(Ogre::SceneManager *sceneMgr)
{
    playerNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

    playerNode->translate(position);

    //TODO: put this code somewhere nicer
    cameraNode = playerNode->createChildSceneNode();
    cameraNode->setPosition(0, 180, 0);
    //cameraNode->attachObject(mCamera);
}

void Player::update()
{
    this->orientation = this->orientation * Ogre::Quaternion(Ogre::Radian(rotX), Ogre::Vector3::UNIT_Y);
    //this->orientation = this->orientation * Ogre::Quaternion(Ogre::Radian(rotY), Ogre::Vector3::UNIT_X);

    //this->cameraNode->setOrientation(Ogre::Quaternion(this->orientation.getPitch(), Ogre::Vector3::UNIT_Z));
    //this->cameraNode->setOrientation(this->orientation);

    rotX = 0;
    
    this->position = this->position + this->orientation * this->velocity;
    
    this->signals.updated(this);
}

void Player::doGraphics(Player *player)
{
    this->cameraNode->pitch(rotY);
    rotY = 0;

    this->playerNode->setPosition(this->position);
    this->playerNode->setOrientation(this->orientation);
}

void Player::forward(void)
{
    std::cout << "MOVING FORWARD" << std::endl;
    this->velocity.z = -Player::MOVEMENT_SPEED;
}

void Player::stopMovingForward(void)
{
    this->velocity.z = 0;
}

void Player::back(void)
{
    std::cout << "MOVING BACK" << std::endl;
    this->velocity.z = Player::MOVEMENT_SPEED;
}

void Player::stopMovingBack(void)
{
    this->velocity.z = 0;
}

void Player::left(void)
{
    std::cout << "MOVING LEFT" << std::endl;
    this->velocity.x = -Player::MOVEMENT_SPEED;
}

void Player::stopMovingLeft(void)
{
    this->velocity.x = 0;
}

void Player::right(void)
{
    std::cout << "MOVING RIGHT" << std::endl;
    this->velocity.x = Player::MOVEMENT_SPEED;
}

void Player::stopMovingRight(void)
{
    this->velocity.x = 0;
}

void Player::jump(void)
{
    std::cout << "JUMPING" << std::endl;

    this->position = this->position + Ogre::Vector3(0, 200, 0);
}

void Player::shoot(Ogre::SceneManager *mSceneMgr, btDiscreteDynamicsWorld* dynamicsWorld)
{
	double l = 1000;
	double X = cameraNode->convertLocalToWorldPosition(Ogre::Vector3(0,0,0)).x;
	double Y = cameraNode->convertLocalToWorldPosition(Ogre::Vector3(0,0,0)).y;
	double Z = cameraNode->convertLocalToWorldPosition(Ogre::Vector3(0,0,0)).z;
	btVector3 rayFrom(X,Y,Z);
	printf("%f, %f, %f\n", X,Y,Z);
	printf("%f, %f\n", playerNode->getOrientation().getYaw().valueDegrees(), cameraNode->getOrientation().getPitch().valueDegrees());
	double camX1 = 0;
	double camY1 = (-1)*-sin(cameraNode->getOrientation().getPitch().valueRadians());
	double camZ1 = (-1)*cos(cameraNode->getOrientation().getPitch().valueRadians());
	printf("%f, %f, %f\n", camX1,camY1,camZ1);
	double camX = camZ1*sin(playerNode->getOrientation().getYaw().valueRadians());
	double camY = camY1;
	double camZ = camZ1*cos(playerNode->getOrientation().getYaw().valueRadians());
	printf("%f, %f, %f\n", camX,camY,camZ);
	double dirX = X+(l*camX);
	double dirY = Y+(l*camY);
	double dirZ = Z+(l*camZ);
	btVector3 rayTo(dirX,dirY,dirZ);
	printf("%f, %f, %f\n", dirX,dirY,dirZ);
	//btCollisionWorld collisionWorld  = *dynamicsWorld->getCollisionWorld();
	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

	dynamicsWorld->rayTest(rayFrom,rayTo,rayCallback);
	if (rayCallback.hasHit())
	{
		printf("hit!\n");
		/*btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);
		if (body)
		{
			body->setActivationState(ACTIVE_TAG);
			btVector3 impulse = rayTo;
			impulse.normalize();
			float impulseStrength = 10.f;
			impulse *= impulseStrength;
			btVector3 relPos = rayCallback.m_hitPointWorld - body->getCenterOfMassPosition();
			body->applyImpulse(impulse,relPos);
		}*/
	}

    Ogre::Quaternion orientation = this->cameraNode->getOrientation() * this->playerNode->getOrientation();
    orientation = orientation * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);

    this->signals.fired(this, new Rocket(this->position + Ogre::Vector3::UNIT_Y * 100, orientation));
}

void Player::platform(void)
{
    this->signals.platform(this, new Platform(this->position, this->playerNode->getOrientation()));
}

void Player::lookX(int dist)
{
    rotX += Ogre::Degree(-dist * Player::ROTATION_SPEED);
}

void Player::lookY(int dist)
{
    rotY += Ogre::Degree(-dist * Player::ROTATION_SPEED);
}
