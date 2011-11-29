#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "Player.h"

Player::Player(Ogre::Vector3 position)
{
    this->position = position;
}

Player::~Player(void)
{
}

void Player::addToScene(Ogre::SceneManager *sceneMgr, std::string name)
{
    Ogre::Entity* ogreHead = sceneMgr->createEntity(name, "ninja.mesh");

    mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(ogreHead);

    mSceneNode->translate(position);
}

void Player::update(void)
{
    mSceneNode->yaw(Ogre::Degree(0.5));
    mSceneNode->pitch(Ogre::Degree(0.1));
}

void Player::forward(void)
{
    std::cout << "MOVING FORWARD" << std::endl;
}

void Player::back(void)
{
    std::cout << "MOVING BACK" << std::endl;
}

void Player::left(void)
{
    std::cout << "MOVING LEFT" << std::endl;
}

void Player::right(void)
{
    std::cout << "MOVING RIGHT" << std::endl;
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

