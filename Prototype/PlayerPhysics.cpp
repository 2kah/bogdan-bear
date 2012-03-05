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
	this->dynamicsWorld = dynamicsWorld;
	
	//TODO: make this a cvar
	//Defines walk speed
	walkSpeed = btScalar(1.0);

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

	playerUpdateConnection = this->player->signals.updated.connect(boost::bind(&PlayerPhysics::playerUpdated, this, _1));
	enterTurretConnection = this->player->signals.enteredTurret.connect(boost::bind(&PlayerPhysics::deactivate, this));
	exitTurretConnection = this->player->signals.exitedTurret.connect(boost::bind(&PlayerPhysics::reactivate, this));

    this->removedConnection = this->player->signals.removed.connect(boost::bind(&PlayerPhysics::playerRemoved, this, _1));

    this->player->signals.stateSet.connect(boost::bind(&PlayerPhysics::playerStateSet, this, _1));
}

PlayerPhysics::~PlayerPhysics()
{
	this->dynamicsWorld->removeCollisionObject(this->m_ghostObject);
	this->dynamicsWorld->removeAction(this->m_character);

	playerUpdateConnection.disconnect();
	enterTurretConnection.disconnect();
	exitTurretConnection.disconnect();
    this->removedConnection.disconnect();

	delete this->m_ghostObject;
	delete this->m_character;
}

void PlayerPhysics::playerRemoved(Player *player)
{
    delete this;
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
		if(walk.length() > 0)
		{
			actualMovement += walk.normalize() * walkSpeed;
		}
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
	}
	m_character->setWalkDirection(actualMovement);
	oldWalkDirection = walk;
}

void PlayerPhysics::playerStateSet(Player *player)
{
    // update position/velocity here
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
	//offset the vertical
	pushDirection.setY(pushDirection.y() + 0.6);
}

void PlayerPhysics::deactivate()
{
	this->dynamicsWorld->removeCollisionObject(this->m_ghostObject);
	this->dynamicsWorld->removeAction(this->m_character);
	playerUpdateConnection.disconnect();
}

void PlayerPhysics::reactivate()
{
	this->dynamicsWorld->addCollisionObject(this->m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter /*btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter*/);
	this->dynamicsWorld->addAction(this->m_character);
	playerUpdateConnection = this->player->signals.updated.connect(boost::bind(&PlayerPhysics::playerUpdated, this, _1));
}