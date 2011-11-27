#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "Object.h"

Object::Object(): mSceneNode(0)
{
}

Object::Object(Ogre::SceneManager *sceneMgr, Ogre::Vector3 position, std::string name)
{
    Ogre::Entity* ogreHead = sceneMgr->createEntity(name, "ogrehead.mesh");

    mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(ogreHead);

    mSceneNode->translate(position);
}

Object::~Object(void)
{
}

void Object::update(void)
{
    mSceneNode->yaw(Ogre::Degree(0.5));
}
