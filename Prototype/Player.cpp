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