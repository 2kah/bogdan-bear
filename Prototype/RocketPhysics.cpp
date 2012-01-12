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

    this->ghost = new btGhostObject();
    
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

    if (this->ghost->getNumOverlappingObjects() > 2) {
        this->rocket->explode();
    }
}

void RocketPhysics::rocketExploded(Rocket *rocket, Explosion *explosion)
{
    delete this;
}
