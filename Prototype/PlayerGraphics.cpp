#include "PlayerGraphics.h"

#include <string>

#include "Player.h"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

PlayerGraphics::PlayerGraphics(Player *player, Ogre::SceneManager *sceneManager)
{
    this->player = player;
    
    this->sceneManager = sceneManager;
    this->entity = sceneManager->createEntity("Ninja.mesh");
    this->sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    this->sceneNode->attachObject(this->entity);
    
    this->playerUpdated(this->player);

    this->player->signals.updated.connect(boost::bind(&PlayerGraphics::playerUpdated, this, _1));
}

PlayerGraphics::~PlayerGraphics()
{
    this->sceneManager->destroyEntity(this->entity);
    this->sceneManager->destroySceneNode(this->sceneNode);
}

void PlayerGraphics::playerUpdated(Player *player)
{
    this->sceneNode->setPosition(this->player->position);
    this->sceneNode->setOrientation(this->player->orientation); // * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Z));
}