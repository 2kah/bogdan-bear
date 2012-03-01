#ifndef __PlayerPhysics_h_
#define __PlayerPhysics_h_

#include <btBulletDynamicsCommon.h>
#include <boost/signals.hpp>

#include "PhysicsObject.h"

class PlayerInput;
enum DIRECTION;
class Player;
class Explosion;
class btDiscreteDynamicsWorld;
class btRigidBody;

class btCharacterControllerInterface;
class btKinematicCharacterController;
class btCollisionShape;

class PlayerPhysics : public boost::signals::trackable, public PhysicsObject
{
public:
    PlayerPhysics(Player *player, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~PlayerPhysics();

	virtual void addInput(PlayerInput &input);

    virtual void playerRemoved(Player *player);

	virtual void movement(DIRECTION direction, bool state);
	virtual void jump(bool state);

	virtual void explode(Explosion *explosion);

	virtual void deactivate();
	virtual void reactivate();

	btKinematicCharacterController* m_character;
	class btPairCachingGhostObject* m_ghostObject;
protected:
    virtual void playerUpdated(Player *player);

    Player *player;

	btScalar walkSpeed;
	btVector3 walkDirection;
	btVector3 oldWalkDirection;
	btScalar airMovementSpeed;

	btVector3 pushDirection;

	btDiscreteDynamicsWorld *dynamicsWorld;

	//TODO: not sure if this is necessary/the correct way to do this
	boost::signals::connection playerUpdateConnection;
	boost::signals::connection enterTurretConnection;
	boost::signals::connection exitTurretConnection;
    boost::signals::connection removedConnection;
};

#endif // #ifndef __PlayerPhysics_h_
