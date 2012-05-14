#include "ExplosionPhysics.h"

#include <iostream>
#include <vector>
#include <set>

#include <boost/math/constants/constants.hpp>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "PhysicsObject.h"
#include "Explosion.h"
#include "Tower.h"
#include "TowerPhysics.h"
#include "PlatformPhysics.h"
#include "Platform.h"

ExplosionPhysics::ExplosionPhysics(Explosion *explosion, btDiscreteDynamicsWorld *dynamicsWorld)
{
    this->explosion = explosion;
    this->dynamicsWorld = dynamicsWorld;

    this->explosion->signals.updated.connect(boost::bind(&ExplosionPhysics::explosionUpdated, this, _1));
    this->explosion->signals.finished.connect(boost::bind(&ExplosionPhysics::explosionFinished, this, _1));

    this->ghost = new btPairCachingGhostObject();
    
    btVector3 position(this->explosion->position.x, this->explosion->position.y, this->explosion->position.z);
	double size = Explosion::SIZE;
	if(explosion->isMassive)
		size *= 10;
    this->ghost->setCollisionShape(new btSphereShape(size));

    this->ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    this->ghost->setWorldTransform(btTransform(btMatrix3x3::getIdentity(), position));

    this->dynamicsWorld->addCollisionObject(this->ghost, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);

    //std::cout << "Explosion destroyed " << this->ghost->getNumOverlappingObjects() << " objects!" << std::endl;

    std::vector<btCollisionObject *> destructions;
    
    this->tower = NULL;

    for(int i = 0; i < this->ghost->getNumOverlappingObjects(); i++)
    {
        btCollisionObject *blockObject = this->ghost->getOverlappingObject(i);
        
        PhysicsObject *object = static_cast<PhysicsObject *>(blockObject->getUserPointer());

        if (object != NULL) {
            if (object->type == TOWER_CHUNK) {
                this->tower = static_cast<PhysicsChunk *>(object)->tower;
            } else if (object->type == PLATFORM) {
                static_cast<PlatformPhysics *>(object)->platform->destroy();
            }

            object->explode(this->explosion);
        }
    }
   
    if (this->tower != NULL && !this->explosion->isMassive) {
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
