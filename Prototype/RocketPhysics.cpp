#include "RocketPhysics.h"

#include <boost/bind.hpp>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "Rocket.h"

RocketPhysics::RocketPhysics(Rocket *rocket, btDiscreteDynamicsWorld *dynamicsWorld)
{
    this->rocket = rocket;
    this->dynamicsWorld = dynamicsWorld;
    
    this->rocket->signals.updated.connect(boost::bind(&RocketPhysics::rocketUpdated, this, _1));
    this->rocket->signals.exploded.connect(boost::bind(&RocketPhysics::rocketExploded, this, _1, _2));

    this->ghost = new btPairCachingGhostObject();
    
    this->ghost->setCollisionShape(new btSphereShape(0.5));
    
    this->rocketUpdated(rocket);

    this->ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    this->dynamicsWorld->addCollisionObject(this->ghost, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
}

RocketPhysics::~RocketPhysics()
{
    this->dynamicsWorld->removeCollisionObject(this->ghost);

    delete this->ghost;
}

void RocketPhysics::rocketUpdated(Rocket *rocket)
{
    btVector3 position(this->rocket->position.x, this->rocket->position.y, this->rocket->position.z);
    btQuaternion orientation(this->rocket->orientation.x, this->rocket->orientation.y, this->rocket->orientation.z, this->rocket->orientation.w);

    this->ghost->setWorldTransform(btTransform(orientation, position));

    //this->dynamicsWorld->getDispatcher()->dispatchAllCollisionPairs(this->ghost->getOverlappingPairCache(), this->dynamicsWorld->getDispatchInfo(), this->dynamicsWorld->getDispatcher());

    btManifoldArray manifolds;

    //if (this->ghost->getNumOverlappingObjects() > 1) {
    //    this->rocket->explode();
    //    return;
    //}

    btBroadphasePairArray& pairs = this->ghost->getOverlappingPairCache()->getOverlappingPairArray();

    for (int i=0; i < pairs.size(); i++)
    {
        manifolds.clear();

        const btBroadphasePair& pair = pairs[i];
         
        //unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
        btBroadphasePair* collisionPair = dynamicsWorld->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);
        if (!collisionPair) {
            continue;
        }

        if (collisionPair->m_algorithm) {
            collisionPair->m_algorithm->getAllContactManifolds(manifolds);
        }

        for (int j=0; j < manifolds.size(); j++)
        {
            btPersistentManifold* manifold = manifolds[j];
            btScalar directionSign = manifold->getBody0() == this->ghost ? btScalar(-1.0) : btScalar(1.0);

            for (int p=0; p < manifold->getNumContacts(); p++)
            {
                const btManifoldPoint&pt = manifold->getContactPoint(p);
            
                if (pt.getDistance() < 0.f)
	            {
		            const btVector3& ptA = pt.getPositionWorldOnA();
		            const btVector3& ptB = pt.getPositionWorldOnB();
		            const btVector3& normalOnB = pt.m_normalWorldOnB;
		            
                    this->rocket->explode();
                    return;
	            }
            }
        }
    }
}

void RocketPhysics::rocketExploded(Rocket *rocket, Explosion *explosion)
{
    delete this;
}
