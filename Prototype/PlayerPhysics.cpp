#include "PlayerPhysics.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>
#include <BulletDynamics\Character\btKinematicCharacterController.h>

#include "Player.h"
#include "PlayerInput.h"

PlayerPhysics::PlayerPhysics(Player *player, btDiscreteDynamicsWorld *dynamicsWorld)
{
	this->player = player;

	btTransform startTransform;
	startTransform.setIdentity();
	btVector3 bPosition(this->player->position.x, this->player->position.y + 5, this->player->position.z);
	startTransform.setOrigin(bPosition);

	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(startTransform);

	btConvexShape* capsule = new btCapsuleShape(1.5, 8);
	m_ghostObject->setCollisionShape(capsule);
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	btScalar stepHeight = btScalar(0.35);

	m_character = new btKinematicCharacterController(m_ghostObject, capsule, stepHeight);

	dynamicsWorld->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	dynamicsWorld->addAction(m_character);

	walkDirection.setZero();

	/*walkDirection.setZero();
	
	this->player = player;
	btConvexShape* capsule = new btCapsuleShape(1.5, 8);
	
    btTransform playerTransform;
    playerTransform.setIdentity();
	btVector3 bPosition(this->player->position.x, this->player->position.y + 5, this->player->position.z);
	playerTransform.setOrigin(bPosition);
    btScalar mass = 1;
    bool isDynamic = (mass != 0.f);
    btVector3 localInertia(0,0,0);
    if (isDynamic)
    {
        capsule->calculateLocalInertia(mass, localInertia);
    }
    btDefaultMotionState* playerMotionState = new btDefaultMotionState(playerTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, playerMotionState, capsule, localInertia);
    playerRigidBody = new btRigidBody(rbInfo);
	playerRigidBody->setActivationState(DISABLE_DEACTIVATION);

    dynamicsWorld->addRigidBody(playerRigidBody);*/

    //TODO: utility function to convert between ogre and bullet vector3
	this->player->signals.updated.connect(boost::bind(&PlayerPhysics::playerUpdated, this, _1));
}

PlayerPhysics::~PlayerPhysics()
{
}

void PlayerPhysics::addInput(PlayerInput &input)
{
	input.signals.move.connect(boost::bind(&PlayerPhysics::movement, this, _1, _2));
}

void PlayerPhysics::playerUpdated(Player *player)
{
	btTransform xform;
	xform = m_ghostObject->getWorldTransform();
	
	Ogre::Vector3 oPosition(xform.getOrigin().x(), xform.getOrigin().y() - 5, xform.getOrigin().z());
	this->player->position = oPosition;

	/*btVector3 forwardDir = xform.getBasis()[2];
	btVector3 upDir = xform.getBasis()[1];
	btVector3 strafeDir = xform.getBasis()[0];
	forwardDir.normalize ();
	upDir.normalize ();
	strafeDir.normalize ();*/

	//TODO: make a cvar
	btScalar walkVelocity = btScalar(4.0);

	Ogre::Vector3 movement = this->player->orientation * Ogre::Vector3(walkDirection.x(), walkDirection.y(), walkDirection.z());
	btVector3 walk(movement.x, movement.y, movement.z);

	m_character->setWalkDirection(walk * walkVelocity);



	//btTransform trans;
	//playerRigidBody->getMotionState()->getWorldTransform(trans);

	//trans.setRotation(btQuaternion(btVector3(0, 1, 0), 0));

	//Ogre::Vector3 oPosition(trans.getOrigin().x(), trans.getOrigin().y() - 5, trans.getOrigin().z());
	//this->player->position = oPosition;

	//playerRigidBody->setCenterOfMassTransform(trans);


	//Ogre::Vector3 move = this->player->orientation * Ogre::Vector3(walkDirection.x(), walkDirection.y(), walkDirection.z());

	////std::cout << this->player->orientation << std::endl;
	////std::cout << move << std::endl;

	////btQuaternion rotation(this->player->orientation.x, this->player->orientation.y, this->player->orientation.z, this->player->orientation.w);

	//btVector3 walk(move.x, move.y, move.z);
	////walk = walkDirection;

	////std::cout << walk.getX() << " " << walk.getY() << " " << walk.getZ() << std::endl;
	//btVector3 currentVelocity = playerRigidBody->getLinearVelocity();
	////std::cout << currentVelocity.getX() << " " << currentVelocity.getY() << " " << currentVelocity.getZ() << std::endl;
	//btVector3 newVelocity = currentVelocity + (walk * 50);
	//btScalar speed = newVelocity.length();
	////std::cout << newVelocity.getX() << " " << newVelocity.getY() << " " << newVelocity.getZ() << std::endl;

	//if(walkDirection.length() == 0)
	//{
	//	currentVelocity *= btScalar(0.2);
	//	playerRigidBody->setLinearVelocity(currentVelocity);
	//}
	//else
	//{
	//	if(speed < 80)
	//	{
	//		playerRigidBody->setLinearVelocity(newVelocity);
	//	}
	//}
}

void PlayerPhysics::movement(DIRECTION direction, bool state)
{
    if (direction == DIRECTION::FORWARD)
    {
		walkDirection.setZ(-Player::MOVEMENT_SPEED * state);
    }
    else if (direction == DIRECTION::BACKWARD)
    {
        walkDirection.setZ(Player::MOVEMENT_SPEED * state);
    }
    else if (direction == DIRECTION::LEFT)
    {
        walkDirection.setX(-Player::MOVEMENT_SPEED * state);
    }
    else if (direction == DIRECTION::RIGHT)
    {
        walkDirection.setX(Player::MOVEMENT_SPEED * state);
    }
}
