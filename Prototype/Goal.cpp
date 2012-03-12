#include "Goal.h"

#include <iostream>
#include <vector>
#include <set>

#include <boost/math/constants/constants.hpp>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "PhysicsObject.h"
#include "Goal.h"
#include "Player.h"

//holdingTeam :0 redTeam, 1 blueTeam, 2 greenTeam, 3 orangeTeam

Goal::Goal(Ogre::Vector3 position, Player *playerTracked)
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
	this->signals.updated(this);
}

Goal::~Goal()
{
}

void Goal::update(void)
{
	//Loop over players
	if((this->goalPosition - player->position).length() < 40 && player->position.y > this->goalPosition.y)
	{
		//flag the team to add points to
		//If goal contains both then add points to neither
		this->holdingTeam[0] = 1;
	}
	//end loop of players
	//Add the points to the relevant team: loop to see if only one team is in the hill (count is how many teams are in the hill,
	//currentHold is the team holding the hill (will get awarded points if there is only one team)
	int count = 0;
	int currentHold = 4;
	for(int i = 0; i < 4; i++)
	{
		if(this->holdingTeam[i] > 0)
		{
			count++;
			currentHold = i;
		}
		this->holdingTeam[i] = 0;
	}
	if(count = 1)
	{
	    this->teamPoints[currentHold]++;
	}
	this->signals.updated(this);

}