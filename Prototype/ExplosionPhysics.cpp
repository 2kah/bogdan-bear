#include "ExplosionPhysics.h"

#include <iostream>
#include <vector>
#include <set>

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
    this->ghost->setCollisionShape(new btSphereShape(Explosion::SIZE));

    this->ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    this->ghost->setWorldTransform(btTransform(btMatrix3x3::getIdentity(), position));

    this->dynamicsWorld->addCollisionObject(this->ghost, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);

    //std::cout << "Explosion destroyed " << this->ghost->getNumOverlappingObjects() << " objects!" << std::endl;

    std::vector<btCollisionObject *> destructions;
    
    this->tower = NULL;

    for(int i = 0; i < this->ghost->getNumOverlappingObjects(); i++)
    {
        btCollisionObject *blockObject = this->ghost->getOverlappingObject(i);
        
        Tower *tower = (Tower *) blockObject->getUserPointer();

        if (tower != NULL) {
            //block->tower->blocks[block->level][block->layer][block->sector] = false;
            this->tower = tower;

            //destructions.push_back(blockObject);
        }
    }
    
    for(std::vector<btCollisionObject *>::iterator it = destructions.begin(); it != destructions.end(); ++it) {
        btCollisionObject *blockObject = *it;
    
       this->dynamicsWorld->removeCollisionObject(blockObject);
    }

    /*
    if (tower != NULL) {
        tower->carveSphere(this->explosion->position, Explosion::SIZE);
    }
    */
    
    if (this->tower != NULL) {
        this->tower->carveSphere(this->explosion->position, Explosion::SIZE);
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
