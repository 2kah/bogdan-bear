#include "TurretGraphics.h"

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include "Turret.h"

TurretGraphics::TurretGraphics(Turret *turret, Ogre::SceneManager *sceneManager)
{
    this->turret = turret;
    
    this->sceneManager = sceneManager;
    this->entity = sceneManager->createEntity("turretBase.mesh");
    this->sceneNodeBase = this->sceneManager->getRootSceneNode()->createChildSceneNode();
    this->sceneNodeBase->attachObject(this->entity);
	this->sceneNodeBase->setOrientation(this->turret->orientation);// * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y));

	this->entity = sceneManager->createEntity("turretGuard.mesh");
	this->entity->setMaterialName("turretGuard");
	//this->sceneNodeGuard = this->sceneNodeBase->createChildSceneNode();
	this->sceneNodeGuard = this->sceneManager->getRootSceneNode()->createChildSceneNode();
	this->sceneNodeGuard->attachObject(this->entity);

	this->entity = sceneManager->createEntity("turretGuns.mesh");
	this->entity->setMaterialName("turretGuns");
	//this->sceneNodeGuard = this->sceneNodeBase->createChildSceneNode();
	this->sceneNodeGuns = this->sceneManager->getRootSceneNode()->createChildSceneNode();
	this->sceneNodeGuns->attachObject(this->entity);

	this->sceneNodeBase->setScale(Ogre::Vector3::UNIT_SCALE * 2);
	this->sceneNodeGuard->setScale(Ogre::Vector3::UNIT_SCALE * 2);
	this->sceneNodeGuns->setScale(Ogre::Vector3::UNIT_SCALE * 2);

	this->sceneNodeBase->setPosition(this->turret->position - Ogre::Vector3::UNIT_Y * 11);
	this->sceneNodeGuard->setPosition(this->turret->position - Ogre::Vector3::UNIT_Y * 11);
	this->sceneNodeGuns->setPosition(this->turret->position - Ogre::Vector3::UNIT_Y * 11);

    this->turretUpdated(this->turret);

    this->turret->signals.updated.connect(boost::bind(&TurretGraphics::turretUpdated, this, _1));
    this->turret->signals.fired.connect(boost::bind(&TurretGraphics::turretFired, this, _1, _2));
}

TurretGraphics::~TurretGraphics()
{
    this->sceneManager->destroyEntity(this->entity);
	this->sceneManager->destroySceneNode(this->sceneNodeGuard);
    this->sceneManager->destroySceneNode(this->sceneNodeBase);
}

void TurretGraphics::turretUpdated(Turret *turret)
{
	//this->sceneNodeGuard->setPosition(Ogre::Vector3(this->turret->position.x, this->turret->position.y + 5, this->turret->position.z) - Ogre::Vector3::UNIT_Y * 11);
    this->sceneNodeGuard->setOrientation(this->turret->orientation * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y));
	this->sceneNodeGuns->setOrientation(this->turret->orientation * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y));
}

void TurretGraphics::turretFired(Turret *turret, Rocket *rocket)
{
}
