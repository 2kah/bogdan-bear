#include "PlatformPhysics.h"

#include <boost/bind.hpp>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "BtOgreExtras.h"

#include "Platform.h"

PlatformPhysics::PlatformPhysics(Platform *platform, btDiscreteDynamicsWorld *dynamicsWorld)
    : PhysicsObject(PLATFORM)
{
	this->platform = platform;
	this->dynamicsWorld = dynamicsWorld;

	this->platform->signals.expired.connect(boost::bind(&PlatformPhysics::platformExpired, this, _1));
    this->platform->signals.destroyed.connect(boost::bind(&PlatformPhysics::platformExpired, this, _1));

	//TODO: tweak
	btVector3 platformSize(10, 1, 10);

	btBoxShape *shape = new btBoxShape(platformSize);

	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), BtOgre::Convert::toBullet(this->platform->position)));
	btRigidBody::btRigidBodyConstructionInfo rbCI(0, motionState, shape, btVector3(0, 0, 0));

	this->body = new btRigidBody(rbCI);
	this->body->setActivationState(ISLAND_SLEEPING);
    this->body->setUserPointer(this);

	this->body->setWorldTransform(btTransform(BtOgre::Convert::toBullet(this->platform->orientation), BtOgre::Convert::toBullet(this->platform->position)));

	dynamicsWorld->addRigidBody(this->body);
}

PlatformPhysics::~PlatformPhysics()
{
	this->dynamicsWorld->removeRigidBody(this->body);
	delete this->body;
}

void PlatformPhysics::platformExpired(Platform *platform)
{
    this->dynamicsWorld->removeRigidBody(this->body);
	delete this->body;
	//delete this;
}
