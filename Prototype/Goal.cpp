#include "Goal.h"

#include <iostream>
#include <vector>
#include <set>

#include <boost/math/constants/constants.hpp>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <btBulletWorldImporter.h>

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>

#include "PhysicsObject.h"
#include "Goal.h"
#include "Player.h"

//holdingTeam :0 redTeam, 1 blueTeam, 2 greenTeam, 3 orangeTeam
//PlayerTracked should be a list of players to loop over.
Goal::Goal(Ogre::Vector3 position, Player *playerTracked, Ogre::SceneManager *sceneManager, btDiscreteDynamicsWorld *dynamicsWorld)
{
	player = playerTracked;
	goalPosition = position;
	goalPosition.y = goalPosition.y - 20;
	this->holdingTeam[0] = 0;
	this->holdingTeam[1] = 0;
	this->holdingTeam[2] = 0;
	this->holdingTeam[3] = 0;
	this->holdingTeam[4] = 0;
	this->teamPoints[0] = 0;
	this->teamPoints[1] = 0;
	this->teamPoints[2] = 0;
	this->teamPoints[3] = 0;
	this->teamPoints[4] = 0;
	this->gameOver = false;
	this->signals.updated(this);
    
	this->sceneManager = sceneManager;
    this->entity = sceneManager->createEntity("goal.mesh");
	this->entity->setMaterialName("goal");
    this->sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    this->sceneNode->attachObject(this->entity);

    //this->sceneNode->setScale(Ogre::Vector3::UNIT_SCALE * 4);
	this->sceneNode->setPosition(0,goalPosition.y + 20, 0);

	btBulletWorldImporter* fileLoader = new btBulletWorldImporter(dynamicsWorld);
	fileLoader->loadFile("goal.bullet");
	
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[dynamicsWorld->getNumCollisionObjects()-1];
	obj->setWorldTransform(btTransform(btQuaternion(), btVector3(0,goalPosition.y + 20, 0)));
}

Goal::~Goal()
{
}

bool Goal::isPlayerInRange(Ogre::Vector3 position)
{
	if (position.y > this->goalPosition.y && position.distance(this->goalPosition) < 40.0)
		return true;
	return false;
}

void Goal::update(void)
{
	////Loop over players
	//if((this->goalPosition - player->position).length() < 40 && player->position.y > this->goalPosition.y)
	//{
	//	//flag the team to add points to
	//	//If goal contains both then add points to neither
	//	this->holdingTeam[player->prop->getTeam()] = 1;
	//}
	////end loop of players
	////Add the points to the relevant team: loop to see if only one team is in the hill (count is how many teams are in the hill,
	////currentHold is the team holding the hill (will get awarded points if there is only one team)
	//int count = 0;
	//int currentHold = 4;
	//for(int i = 0; i < 4; i++)
	//{
	//	if(this->holdingTeam[i] > 0)
	//	{
	//		count++;
	//		currentHold = i;
	//	}
	//	this->holdingTeam[i] = 0;
	//}
	//if(count == 1)
	//{
	//    this->teamPoints[currentHold]++;
	//	if(this->teamPoints[currentHold] > 3000)
	//	{
	//		std::cout<<"GAME OVER " << currentHold << "WINS WITH " << this->teamPoints[currentHold] <<std::endl;
	//		this->gameOver = true;
	//	}
	//}

	//int points = this->teamPoints[0];
	//if(points > 0 && points % 100 == 0)std::cout<<"Team points:" << this->teamPoints[0] <<std::endl;
	//this->signals.updated(this);

}

bool Goal::isGameOver()
{
	return this->gameOver;
}

void Goal::setGameOver()
{
	this->gameOver = true;
}

void Goal::setNotGameOver()
{
	this->gameOver = false;
}