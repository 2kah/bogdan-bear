#include "PlayerPhysics.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>

#include "BtKinematicCharacterController.h"
#include "Player.h"
#include "PlayerInput.h"
#include "Explosion.h"
#include "BtOgreExtras.h"

PlayerPhysics::PlayerPhysics(Player *player, btDiscreteDynamicsWorld *dynamicsWorld) : PhysicsObject()
{
	this->player = player;
	
	//TODO: make this a cvar
	//Defines walk speed
	walkSpeed = btScalar(2.0);

	btTransform startTransform;
	startTransform.setIdentity();
	//6 is added to y because the position is measured from the feet and we want the centre
	btVector3 bPosition(this->player->position.x, this->player->position.y + 6, this->player->position.z);
	startTransform.setOrigin(bPosition);

	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(startTransform);

	btConvexShape* capsule = new btCapsuleShape(1.5, 8);
	m_ghostObject->setCollisionShape(capsule);
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	this->m_ghostObject->setUserPointer(this);

	//TODO: make this sensible, this is the max metres a player can step up (0.35 is recommended)
	btScalar stepHeight = btScalar(2.5);

	m_character = new btKinematicCharacterController(m_ghostObject, capsule, stepHeight, dynamicsWorld);

	//TODO: make these both cvars
	//Terminal velocity for falling
	m_character->setFallSpeed(btScalar(60.0));
	//Defines how quickly terminal velocity is reached
	m_character->setGravity(btScalar(100.0));

	//TODO: make this a cvar
	m_character->setJumpSpeed(btScalar(50.0));
	//The max horizontal movement speed while airborne
	airMovementSpeed = btScalar(0.6);

	dynamicsWorld->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter /*btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter*/);
	dynamicsWorld->addAction(m_character);

	walkDirection.setZero();
	oldWalkDirection.setZero();
	pushDirection.setZero();

    //TODO: utility function to convert between ogre and bullet vector3
	this->player->signals.updated.connect(boost::bind(&PlayerPhysics::playerUpdated, this, _1));
}

PlayerPhysics::~PlayerPhysics()
{
}

void PlayerPhysics::addInput(PlayerInput &input)
{
	input.signals.move.connect(boost::bind(&PlayerPhysics::movement, this, _1, _2));

	input.signals.jump.connect(boost::bind(&PlayerPhysics::jump, this, _1));
}

void PlayerPhysics::playerUpdated(Player *player)
{
	btTransform xform;
	xform = m_ghostObject->getWorldTransform();
	
	Ogre::Vector3 oPosition(xform.getOrigin().x(), xform.getOrigin().y() - 5, xform.getOrigin().z());
	this->player->position = oPosition;

	Ogre::Vector3 movement = this->player->orientation * Ogre::Vector3(walkDirection.x(), walkDirection.y(), walkDirection.z());
	btVector3 walk(movement.x, movement.y, movement.z);
	btVector3 actualMovement = pushDirection;
	pushDirection *= btScalar(0.95);

	//if on ground then allow whatever movement the player wants
	if(m_character->onGround())
	{
		actualMovement += (walk * walkSpeed);
		//m_character->setWalkDirection(walk * walkSpeed);
	}
	//if airborne then use previous movement plus a dampened amount of wanted movement
	else
	{
		walk = walk * btScalar(0.3);
		walk += (oldWalkDirection * btScalar(0.99));
		if(walk.length() > airMovementSpeed)
		{
			walk = walk / (walk.length() / airMovementSpeed);
		}
		actualMovement += walk;
		//m_character->setWalkDirection(walk);
	}
	m_character->setWalkDirection(actualMovement);
	oldWalkDirection = walk;
}

void PlayerPhysics::movement(DIRECTION direction, bool state)
{
    if (direction == FORWARD)
	{
		//there must be a nicer way of doing this surely
		if(state)
		{
			walkDirection.setZ(walkDirection.z() - Player::MOVEMENT_SPEED);
		}
		else
		{
			walkDirection.setZ(walkDirection.z() + Player::MOVEMENT_SPEED);
		}
    }
    else if (direction == BACKWARD)
    {
		if(state)
		{
			walkDirection.setZ(walkDirection.z() + Player::MOVEMENT_SPEED);
		}
		else
		{
			walkDirection.setZ(walkDirection.z() - Player::MOVEMENT_SPEED);
		}
    }
    else if (direction == LEFT)
    {
		if(state)
		{
			walkDirection.setX(walkDirection.x() - Player::MOVEMENT_SPEED);
		}
		else
		{
			walkDirection.setX(walkDirection.x() + Player::MOVEMENT_SPEED);
		}
    }
    else if (direction == RIGHT)
    {
		if(state)
		{
			walkDirection.setX(walkDirection.x() + Player::MOVEMENT_SPEED);
		}
		else
		{
			walkDirection.setX(walkDirection.x() - Player::MOVEMENT_SPEED);
		}
    }
}

void PlayerPhysics::jump(bool state)
{
	if(state)
	{
		m_character->jump();
	}
}

void PlayerPhysics::explode(Explosion *explosion)
{
	std::cout << "Explosion has hit player" << std::endl;

	pushDirection = BtOgre::Convert::toBullet(this->player->position - explosion->position) * btScalar(0.1);
	pushDirection.setY(pushDirection.y() + 0.6);
}
