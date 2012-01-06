#include "ExplosionPhysics.h"

#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "Explosion.h"

ExplosionPhysics::ExplosionPhysics(Explosion *explosion, btDiscreteDynamicsWorld *dynamicsWorld)
{
    this->explosion = explosion;
    this->dynamicsWorld = dynamicsWorld;

    this->explosion->signals.updated.connect(boost::bind(&ExplosionPhysics::explosionUpdated, this, _1));
    this->explosion->signals.finished.connect(boost::bind(&ExplosionPhysics::explosionFinished, this, _1));
}

ExplosionPhysics::~ExplosionPhysics()
{
}

void ExplosionPhysics::explosionUpdated(Explosion *explosion)
{
}

void ExplosionPhysics::explosionFinished(Explosion *explosion)
{
    delete this;
}
