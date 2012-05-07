#include "PlatformGraphics.h"

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include "Platform.h"

PlatformGraphics::PlatformGraphics(Platform *platform, Ogre::SceneManager *sceneManager)
{
    this->platform = platform;
    
    this->sceneManager = sceneManager;
    this->entity = sceneManager->createEntity("Platform.mesh");
    this->sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    this->sceneNode->attachObject(this->entity);

    //this->sceneNode->setScale(Ogre::Vector3::UNIT_SCALE / 2);
	this->sceneNode->setScale(Ogre::Vector3::UNIT_SCALE * 3);


    this->platformUpdated(this->platform);

    this->platform->signals.expired.connect(boost::bind(&PlatformGraphics::platformExpired, this, _1));
    this->platform->signals.destroyed.connect(boost::bind(&PlatformGraphics::platformExpired, this, _1));
    //this->platform->signals.updated.connect(boost::bind(&PlatformGraphics::platformUpdated, this, _1)); // WARNING: this can be called after the graphics deletes itself atm!
}

PlatformGraphics::~PlatformGraphics()
{
    this->sceneManager->destroyEntity(this->entity);
    this->sceneManager->destroySceneNode(this->sceneNode);
}

void PlatformGraphics::platformUpdated(Platform *platform)
{
    this->sceneNode->setPosition(this->platform->position);
    this->sceneNode->setOrientation(this->platform->orientation);
}

void PlatformGraphics::platformExpired(Platform *platform)
{
    delete this;
}
