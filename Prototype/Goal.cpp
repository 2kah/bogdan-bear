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
#include "Tower.h"
#include "TowerPhysics.h"
#include "PlatformPhysics.h"
#include "Platform.h"

Goal::Goal(Ogre::Vector3 position, btDiscreteDynamicsWorld *dynamicsWorld)
{
    this->dynamicsWorld = dynamicsWorld;

    this->ghost = new btGhostObject();

    this->ghost->setCollisionShape(new btSphereShape(50));

    this->ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    this->ghost->setWorldTransform(btTransform(btMatrix3x3::getIdentity(), btVector3(position.x, position.y, position.z)));

    this->dynamicsWorld->addCollisionObject(this->ghost, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);

    //std::cout << "Goal destroyed " << this->ghost->getNumOverlappingObjects() << " objects!" << std::endl;

    std::vector<btCollisionObject *> destructions;
    
    this->tower = NULL;

    /*for(int i = 0; i < this->ghost->getNumOverlappingObjects(); i++)
    {
        btCollisionObject *blockObject = this->ghost->getOverlappingObject(i);
        
        PhysicsObject *object = static_cast<PhysicsObject *>(blockObject->getUserPointer());

        if (object != NULL) {
            if (object->type == PLAYER) { 
                std::cout << "Player in goal " << this->ghost->getNumOverlappingObjects() << std::endl;
            }
        }
    }*/
}

Goal::~Goal()
{
    this->dynamicsWorld->removeCollisionObject(this->ghost);

    delete this->ghost;
}

void Goal::update(void)
{
	for(int i = 0; i < this->ghost->getNumOverlappingObjects(); i++)
    {
        btCollisionObject *blockObject = this->ghost->getOverlappingObject(i);
        
        PhysicsObject *object = static_cast<PhysicsObject *>(blockObject->getUserPointer());

        if (object != NULL) {
            if (object->type == PLAYER) { 
                std::cout << "Player in goal " << this->ghost->getNumOverlappingObjects() << std::endl;
            }
        }
    }
}