#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "Object.h"

Object::Object(Ogre::SceneManager *sceneMgr)
{
    Ogre::Entity* ogreHead = sceneMgr->createEntity("Head", "ogrehead.mesh");

    mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(ogreHead);
}

Object::~Object(void)
{
}

void Object::update(void)
{
    mSceneNode->yaw(Ogre::Degree(0.5));
}
