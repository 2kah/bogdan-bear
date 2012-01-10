#include "TurretGraphics.h"

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include "Turret.h"

TurretGraphics::TurretGraphics(Turret *turret, Ogre::SceneManager *sceneManager)
{
    this->turret = turret;
    
    this->sceneManager = sceneManager;
    this->entity = sceneManager->createEntity("facial.mesh");
    this->sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    this->sceneNode->attachObject(this->entity);

    this->sceneNode->setScale(Ogre::Vector3::UNIT_SCALE / 2);

    this->turretUpdated(this->turret);

    this->turret->signals.updated.connect(boost::bind(&TurretGraphics::turretUpdated, this, _1));
    this->turret->signals.fired.connect(boost::bind(&TurretGraphics::turretFired, this, _1, _2));
}

TurretGraphics::~TurretGraphics()
{
    this->sceneManager->destroyEntity(this->entity);
    this->sceneManager->destroySceneNode(this->sceneNode);
}

void TurretGraphics::turretUpdated(Turret *turret)
{
    this->sceneNode->setPosition(this->turret->position - Ogre::Vector3::UNIT_Y * 11);
    this->sceneNode->setOrientation(this->turret->orientation * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y));
}

void TurretGraphics::turretFired(Turret *turret, Rocket *rocket)
{
}
