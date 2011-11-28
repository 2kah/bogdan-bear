#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "Explosion.h"

Explosion::Explosion(Ogre::Vector3 position)
{
    this->position = position;
}

Explosion::~Explosion(void)
{
}

void Explosion::addToScene(Ogre::SceneManager *sceneMgr, std::string name)
{
    Ogre::Entity* ogreHead = sceneMgr->createEntity(name, "sphere.mesh");

    mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(ogreHead);

    mSceneNode->translate(position);
}

void Explosion::update(void)
{
    mSceneNode->scale(1.01, 1.01, 1.01);
}
