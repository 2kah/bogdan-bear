#ifndef __Goal_h_
#define __Goal_h_

#include <boost/signals.hpp>

#include <btBulletDynamicsCommon.h>

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>

#include "Updatable.h"
#include "Object.h"
#include "Player.h"

class btDiscreteDynamicsWorld;
class btGhostObject;

class Tower;

class Goal;

namespace {
struct GoalSignals {
    boost::signal<void (Goal *)> updated;
};
}

class Goal: public Updatable, public Object
{
public:
    Goal(Ogre::Vector3 position, Player *playerTracked, Ogre::SceneManager *sceneManager, btDiscreteDynamicsWorld *dynamicsWorld);

    virtual ~Goal();

	virtual void update();

	virtual bool isGameOver();

	GoalSignals signals;
	bool Goal::isPlayerInRange(Ogre::Vector3 position);
	Ogre::Vector3 goalPosition;
protected:

    btDiscreteDynamicsWorld *dynamicsWorld;
    
    btGhostObject *ghost;

	bool gameOver;
	
    Tower *tower;
	
	Player *player;
	int currentHold;
	int holdingTeam[5]; //0 red, 1 blue, 2 green, 3 yellow
	double teamPoints[5];

    Ogre::SceneManager *sceneManager;
    Ogre::SceneNode *sceneNode;
    Ogre::Entity *entity;

};

#endif // #ifndef __Goal_h_
