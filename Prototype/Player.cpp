#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "Player.h"

Player::Player(Ogre::SceneManager *sceneMgr, Ogre::Vector3 position, std::string name)
{
}

Player::~Player(void)
{
}

void Player::update(void)
{
    mSceneNode->yaw(Ogre::Degree(0.5));
    mSceneNode->pitch(Ogre::Degree(0.1));
}
