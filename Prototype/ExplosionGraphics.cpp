#include "ExplosionGraphics.h"

#include <string>

#include "Explosion.h"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreOverlay.h>
#include <OGRE/OgreOverlayManager.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreMovableObject.h>
#include <OGRE/OgreParticleSystem.h>

ExplosionGraphics::ExplosionGraphics(Explosion *explosion, Ogre::SceneManager *sceneManager, int numb)
{
    this->explosion = explosion;
    this->sceneManager = sceneManager;
	//Name for particle system (needs unique name each time)
	std::stringstream out;
	out << numb;
	std::string s = out.str();
	std::string nameP = "particle" + s;
	//Create particle system
	this->particleSystem = this->sceneManager->createParticleSystem(nameP, "explosionSphere");

	this->sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    //this->sceneNode->attachObject(this->entity);
	this->sceneNode->setPosition(this->explosion->position);
	this->sceneNode->attachObject(this->particleSystem);

    //this->explosionUpdated(this->explosion);

    this->explosion->signals.finished.connect(boost::bind(&ExplosionGraphics::explosionFinished, this, _1));
    this->explosion->signals.updated.connect(boost::bind(&ExplosionGraphics::explosionUpdated, this, _1));
}

ExplosionGraphics::~ExplosionGraphics()
{
    //this->sceneManager->destroyEntity(this->entity);
	this->sceneNode->detachObject(this->particleSystem);
    this->sceneManager->destroySceneNode(this->sceneNode);
}

void ExplosionGraphics::explosionUpdated(Explosion *explosion)
{
   // this->sceneNode->setPosition(this->explosion->position);
    //this->sceneNode->setOrientation(this->explosion->orientation);
    //this->sceneNode->setScale(Ogre::Vector3::UNIT_SCALE * (this->explosion->size / 100.0 / 100.0 * Explosion::SIZE));
}

void ExplosionGraphics::explosionFinished(Explosion *explosion)
{
    delete this;
}