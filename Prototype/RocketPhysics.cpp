#include "RocketPhysics.h"

#include <boost/bind.hpp>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

#include "Rocket.h"

RocketPhysics::RocketPhysics(Rocket *explosion, btDiscreteDynamicsWorld *dynamicsWorld)
{
    this->explosion = explosion;
    this->dynamicsWorld = dynamicsWorld;
    
    this->explosion->signals.updated.connect(boost::bind(&RocketPhysics::rocketUpdated, this, _1));
    this->explosion->signals.exploded.connect(boost::bind(&RocketPhysics::rocketExploded, this, _1, _2));

    btCollisionShape *shape = new btCapsuleShape(0.1, 0.8);

    
}

RocketPhysics::~RocketPhysics()
{
}

void RocketPhysics::rocketUpdated(Rocket *rocket)
{
}

void RocketPhysics::rocketExploded(Rocket *rocket, Explosion *explosion)
{
    delete this;
}
