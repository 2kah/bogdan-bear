#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "SceneObject.h"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(Ogre::Vector3 position)
{
    this->position = position;
}

SceneObject::~SceneObject(void)
{
}

void SceneObject::addToScene(Ogre::SceneManager *sceneMgr, std::string name)
{
    Ogre::Entity* ogreHead = sceneMgr->createEntity(name, "ogrehead.mesh");

    mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(ogreHead);

    mSceneNode->translate(position);
}

void SceneObject::addToPhysics(btDiscreteDynamicsWorld* dynamicsWorld)
{
}

void SceneObject::removeFromScene(Ogre::SceneManager *sceneMgr)
{
    for (int i = 0; i < mSceneNode->numAttachedObjects(); ++i){
        sceneMgr->destroyMovableObject(mSceneNode->getAttachedObject(i));
    }
}

void SceneObject::removeFromPhysics(btDiscreteDynamicsWorld* dynamicsWorld)
{
}

//TODO: make update take time since last frame as parameter
void SceneObject::update(void)
{
    mSceneNode->yaw(Ogre::Degree(0.5));
}
