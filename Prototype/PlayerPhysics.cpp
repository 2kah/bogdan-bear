#include "PlayerPhysics.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>

#include "BtKinematicCharacterController.h"
#include "Player.h"
#include "PlayerInput.h"
#include "Explosion.h"
#include "BtOgreExtras.h"

PlayerPhysics::PlayerPhysics(Player *player, btDiscreteDynamicsWorld *dynamicsWorld) : PhysicsObject(PLAYER)
{
	this->player = player;
	this->dynamicsWorld = dynamicsWorld;
	
	halfHeight = 7.5;
	width = 4.0;
	
	//Defines walk speed
	walkSpeed = btScalar(0.4);

	btTransform startTransform;
	startTransform.setIdentity();
	//halfHeight is added to y because the position is measured from the feet and we want the centre
	btVector3 bPosition(this->player->position.x, this->player->position.y + halfHeight, this->player->position.z);
	startTransform.setOrigin(bPosition);

	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(startTransform);
	m_ghostObject->CO_COLLISION_OBJECT;
	//btConvexShape* capsule = new btCapsuleShape(2, 8);
	btConvexShape* capsule = new btCylinderShape(btVector3(width, halfHeight, width));
	m_ghostObject->setCollisionShape(capsule);
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	this->m_ghostObject->setUserPointer(this);

	//TODO: make this sensible, this is the max metres a player can step up (0.35 is recommended)
	btScalar stepHeight = btScalar(2.5);

	m_character = new btKinematicCharacterController(m_ghostObject, capsule, stepHeight, dynamicsWorld);

	//TODO: make these both cvars
	//Terminal velocity for falling
	m_character->setFallSpeed(btScalar(50.0));
	//Defines how quickly terminal velocity is reached
	m_character->setGravity(btScalar(100.0));

	//TODO: make this a cvar
	m_character->setJumpSpeed(btScalar(80.0));
	//The horizontal movement speed while airborne
	//was 0.4
	airMovementSpeed = btScalar(0.01);
	//The max movement speed while airborne
	maxMoveSpeed = btScalar(0.5);

	dynamicsWorld->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);//btBroadphaseProxy::AllFilter /*btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter*/);
	dynamicsWorld->addAction(m_character);

	walkDirection.setZero();
	oldWalkDirection.setZero();
	pushDirection.setZero();
	oldPosition = BtOgre::Convert::toBullet(player->position);
	local = false;

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
	local = true;
}

void PlayerPhysics::playerUpdated(Player *player)
{
	btTransform xform;
	xform = m_ghostObject->getWorldTransform();

	//prevent vibration when stood on flat surface
	btVector3 currentPosition = xform.getOrigin();
	float yPos = currentPosition.getY();
	float oldYPos = oldPosition.getY();
	float verticalDistMoved = yPos - oldYPos;
	if(verticalDistMoved < 0.03 && verticalDistMoved > -0.03)
		currentPosition.setY((yPos + oldYPos) / 2);
		//currentPosition.setY(min(yPos, oldYPos));
	xform.setOrigin(currentPosition);

	//might be useful for interpolation, if not then delete
	//determine the horizontal speed
	//btVector3 direction = xform.getOrigin() - oldPosition;
	//btScalar currentSpeed = btSqrt((direction.x() * direction.x()) + (direction.y() * direction.y()));
	//std::cout << "currentSpeed = " << currentSpeed << std::endl;

	Ogre::Vector3 newOPos(xform.getOrigin().x(), (xform.getOrigin().y() - halfHeight) + 4, xform.getOrigin().z());
	this->player->position = newOPos;
	this->player->velocity = BtOgre::Convert::toOgre(oldWalkDirection);
	//std::cout << "velocity is: " << this->player->velocity << std::endl;

	if(local)
	{
		Ogre::Vector3 movement = this->player->orientation * Ogre::Vector3(walkDirection.x(), walkDirection.y(), walkDirection.z());
		btVector3 walk(movement.x, movement.y, movement.z);
		btVector3 actualMovement = pushDirection;
		//reduce the effects of any explosion
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
			if(walk.length() > 0)
			{
				walk = walk.normalize();
				walk *= airMovementSpeed;
			}
			walk += (oldWalkDirection * btScalar(0.99));
			
			actualMovement += walk;
			
			if(actualMovement.length() > maxMoveSpeed)
			{
				actualMovement = actualMovement.normalize();
				actualMovement *= maxMoveSpeed;
			}

		}
		m_character->setWalkDirection(actualMovement);
		oldPosition = xform.getOrigin();
		oldWalkDirection = actualMovement;
	}
}

void PlayerPhysics::playerStateSet(Player *player)
{
	//std::cout << "in playerStateSet" << std::endl;
	//std::cout << "external player velocity = " << player->velocity << std::endl;
    // update position/velocity here
	btVector3 newPos(player->position.x, player->position.y + halfHeight, player->position.z);
	m_character->warp(newPos);
	//should make the kinematic character controller have the given velocity, no idea what units the time interval is in so assuming ms
	//hopefully will have another update before time interval ends or player will stop moving
	//std::cout << "setting velocity: " << player->velocity << std::endl;
	m_character->setVelocityForTimeInterval(BtOgre::Convert::toBullet(player->velocity), btScalar(1000));
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
	//set the push direction to be player position - explosion position, then scale
	pushDirection = BtOgre::Convert::toBullet(this->player->position - explosion->position) * btScalar(0.25);
	//offset the vertical
	pushDirection.setY(pushDirection.y() + (halfHeight * 0.25));
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