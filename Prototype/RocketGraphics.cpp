#include "RocketGraphics.h"

#include <string>

#include "Rocket.h"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

RocketGraphics::RocketGraphics(Rocket *rocket, Ogre::SceneManager *sceneManager)
{
    this->rocket = rocket;
    
    this->sceneManager = sceneManager;
    this->entity = sceneManager->createEntity("Barrel.mesh");
    this->sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    this->sceneNode->attachObject(entity);

    this->sceneNode->setScale(2, 10, 2);

    this->rocketUpdated(this->rocket);

    this->rocket->signals.exploded.connect(boost::bind(&RocketGraphics::rocketExploded, this, _1, _2));
    this->rocket->signals.updated.connect(boost::bind(&RocketGraphics::rocketUpdated, this, _1));
}

RocketGraphics::~RocketGraphics()
{
    this->sceneManager->destroyEntity(this->entity);
    this->sceneManager->destroySceneNode(this->sceneNode);
}

void RocketGraphics::rocketUpdated(Rocket *rocket)
{
    this->sceneNode->setPosition(this->rocket->position);
    this->sceneNode->setOrientation(this->rocket->orientation * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Z));
}

void RocketGraphics::rocketExploded(Rocket *rocket, Explosion *explosion)
{
    delete this;
}
