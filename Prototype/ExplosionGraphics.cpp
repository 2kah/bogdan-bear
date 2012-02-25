#include "ExplosionGraphics.h"

#include <string>
#include <boost/bind.hpp>

#include "Explosion.h"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

ExplosionGraphics::ExplosionGraphics(Explosion *explosion, Ogre::SceneManager *sceneManager)
{
    this->explosion = explosion;
    
    this->sceneManager = sceneManager;
    this->entity = sceneManager->createEntity("sphere.mesh");
    this->sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    this->sceneNode->attachObject(this->entity);

    this->explosionUpdated(this->explosion);

    this->explosion->signals.finished.connect(boost::bind(&ExplosionGraphics::explosionFinished, this, _1));
    this->explosion->signals.updated.connect(boost::bind(&ExplosionGraphics::explosionUpdated, this, _1));
}

ExplosionGraphics::~ExplosionGraphics()
{
    this->sceneManager->destroyEntity(this->entity);
    this->sceneManager->destroySceneNode(this->sceneNode);
}

void ExplosionGraphics::explosionUpdated(Explosion *explosion)
{
    this->sceneNode->setPosition(this->explosion->position);
    this->sceneNode->setOrientation(this->explosion->orientation);
    this->sceneNode->setScale(Ogre::Vector3::UNIT_SCALE * (this->explosion->size / 100.0 / 100.0 * Explosion::SIZE));
}

void ExplosionGraphics::explosionFinished(Explosion *explosion)
{
    delete this;
}
