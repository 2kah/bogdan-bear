#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "Explosion.h"

Explosion::Explosion(Ogre::Vector3 position): size(0)
{
    this->position = position;
}

Explosion::~Explosion(void)
{
}

void Explosion::addToScene(Ogre::SceneManager *sceneMgr, std::string name)
{
    Ogre::Entity *ogreHead = sceneMgr->createEntity(name, "sphere.mesh");

    mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(ogreHead);

    mSceneNode->translate(position);

    this->update();
}

void Explosion::removeFromScene(Ogre::SceneManager *sceneMgr)
{
    for (int i = 0; i < mSceneNode->numAttachedObjects(); ++i){
        sceneMgr->destroyMovableObject(mSceneNode->getAttachedObject(i));
    }
}

void Explosion::update(void)
{
    if (size < 100)
    {
        mSceneNode->setScale(size / 100.0, size / 100.0, size / 100.0);
        ++size;
    }
    else
    {
        this->signals.finished(this);
    }
}
