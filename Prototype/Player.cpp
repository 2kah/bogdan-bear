#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "Player.h"

Player::Player(Ogre::Vector3 position)
{
    this->position = position;

    //Movement speed - TODO: this should be a cvar
    mMove = 250;
    mDirection = Ogre::Vector3::ZERO;
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
}

void Player::update(void)
{
    /*mSceneNode->yaw(Ogre::Degree(0.5));
    mSceneNode->pitch(Ogre::Degree(0.1));*/
    //TODO: update this based on time since last frame
    playerNode->translate(mDirection * 0.01, Ogre::Node::TS_LOCAL);
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

void Player::shoot(void)
{
    std::cout << "SHOOTING" << std::endl;
}

void Player::platform(void)
{
    std::cout << "CREATING PLATFORM" << std::endl;
}

