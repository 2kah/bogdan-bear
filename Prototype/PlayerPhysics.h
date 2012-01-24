#ifndef __PlayerPhysics_h_
#define __PlayerPhysics_h_

#include <btBulletDynamicsCommon.h>

class PlayerInput;
enum DIRECTION;
class Player;
class btDiscreteDynamicsWorld;
class btRigidBody;

class btCharacterControllerInterface;
class btKinematicCharacterController;
class btCollisionShape;

class PlayerPhysics
{
public:
    PlayerPhysics(Player *player, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~PlayerPhysics();

	virtual void addInput(PlayerInput &input);

	virtual void movement(DIRECTION direction, bool state);

	btKinematicCharacterController* m_character;
	class btPairCachingGhostObject* m_ghostObject;
protected:
    virtual void playerUpdated(Player *player);

    Player *player;
	//btRigidBody* playerRigidBody;
	btVector3 walkDirection;
};

#endif // #ifndef __PlayerPhysics_h_
