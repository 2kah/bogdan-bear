#include "ExplosionPhysics.h"

#include <iostream>

#include <boost/math/constants/constants.hpp>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "Explosion.h"
#include "Tower.h"

ExplosionPhysics::ExplosionPhysics(Explosion *explosion, btDiscreteDynamicsWorld *dynamicsWorld)
{
    this->explosion = explosion;
    this->dynamicsWorld = dynamicsWorld;

    this->explosion->signals.updated.connect(boost::bind(&ExplosionPhysics::explosionUpdated, this, _1));
    this->explosion->signals.finished.connect(boost::bind(&ExplosionPhysics::explosionFinished, this, _1));

    this->ghost = new btGhostObject();
    
    btVector3 position(this->explosion->position.x, this->explosion->position.y, this->explosion->position.z);

    this->ghost->setCollisionShape(new btSphereShape(100));
    this->ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    this->ghost->setWorldTransform(btTransform(btMatrix3x3::getIdentity(), position));

    dynamicsWorld->addCollisionObject(this->ghost, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);

    std::cout << "Explosion destroyed " << this->ghost->getNumOverlappingObjects() << " objects!" << std::endl;

    for(int i = 0; i < this->ghost->getNumOverlappingObjects(); i++)
    {
        btCollisionObject *blockObject = this->ghost->getOverlappingObject(i);
        
        BlockReference *block = (BlockReference *) blockObject->getUserPointer();

        if (block != NULL) {
            block->tower->blocks[block->layer][block->level][block->sector] = false;
            block->tower->signals.levelUpdated(block->tower, block->level);
        }
    }
}

ExplosionPhysics::~ExplosionPhysics()
{
    this->dynamicsWorld->removeCollisionObject(this->ghost);

    delete this->ghost;
}

void ExplosionPhysics::explosionUpdated(Explosion *explosion)
{
}

void ExplosionPhysics::explosionFinished(Explosion *explosion)
{
    delete this;
}
