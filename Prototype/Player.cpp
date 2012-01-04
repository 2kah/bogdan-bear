#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <math.h>

#include "Player.h"
#include "Rocket.h"

#define PI 3.14159265

Player::Player(Ogre::Vector3 position)
{
    this->position = position;

    //Movement speed - TODO: this should be a cvar
    mMove = 2;
    mDirection = Ogre::Vector3::ZERO;
    rotX = 0;
    rotationFactor = 0.05;
}

Player::~Player(void)
{
}

void Player::addToScene(Ogre::SceneManager *sceneMgr, std::string name)
{
    Ogre::Entity* playerEntity = sceneMgr->createEntity(name, "ninja.mesh");

    playerNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    playerNode->attachObject(playerEntity);

    playerNode->translate(position);

    //TODO: put this code somewhere nicer
    cameraNode = playerNode->createChildSceneNode();
    cameraNode->setPosition(0, 180, 0);
    //cameraNode->attachObject(mCamera);
}

void Player::update(void)
{
    /*mSceneNode->yaw(Ogre::Degree(0.5));
    mSceneNode->pitch(Ogre::Degree(0.1));*/
    playerNode->yaw(rotX);
    cameraNode->pitch(rotY);
    rotX = 0;
    rotY = 0;
    playerNode->translate(mDirection, Ogre::Node::TS_LOCAL);
}

void Player::forward(void)
{
    std::cout << "MOVING FORWARD" << std::endl;
    mDirection.z = -mMove;
}

void Player::stopMovingForward(void)
{
    mDirection.z = 0;
}

void Player::back(void)
{
    std::cout << "MOVING BACK" << std::endl;
    mDirection.z = mMove;
}

void Player::stopMovingBack(void)
{
    mDirection.z = 0;
}

void Player::left(void)
{
    std::cout << "MOVING LEFT" << std::endl;
    mDirection.x = -mMove;
}

void Player::stopMovingLeft(void)
{
    mDirection.x = 0;
}

void Player::right(void)
{
    std::cout << "MOVING RIGHT" << std::endl;
    mDirection.x = mMove;
}

void Player::stopMovingRight(void)
{
    mDirection.x = 0;
}

void Player::jump(void)
{
    std::cout << "JUMPING" << std::endl;
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

    this->signals.fired(this, new Rocket(this->position));

    std::cout << "SHOOTING" << std::endl;
}

void Player::platform(void)
{
    std::cout << "CREATING PLATFORM" << std::endl;
}

void Player::lookX(int dist)
{
    rotX += Ogre::Degree(-dist * rotationFactor);
}

void Player::lookY(int dist)
{
    rotY += Ogre::Degree(-dist * rotationFactor);
}
