#include "RocketGraphics.h"
#include "Rocket.h"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

RocketGraphics::RocketGraphics(Rocket *rocket, Ogre::SceneManager *sceneManager)
{
    this->rocket = rocket;

    Ogre::Entity *entity = sceneManager->createEntity("rocket__", "Barrel.mesh");
    
    this->sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    this->sceneNode->attachObject(entity);

    this->sceneNode->setScale(2, 10, 2);

    this->sceneNode->setPosition(this->rocket->position);
    this->sceneNode->setOrientation(this->rocket->orientation);
}

RocketGraphics::~RocketGraphics()
{
}
