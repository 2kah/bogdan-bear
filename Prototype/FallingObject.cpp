#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <btBulletDynamicsCommon.h>
#include <OgreSphere.h>

#include "FallingObject.h"
#include "Tower.h"

FallingObject::FallingObject(Ogre::Vector3 position)
{
    this->position = position;
}

FallingObject::~FallingObject(void)
{
}

void FallingObject::addToScene(Ogre::SceneManager *sceneMgr)
{
	//Falling object mesh 
    Ogre::Entity* ogreHead = sceneMgr->createEntity("spheredemo.mesh");;

    mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(ogreHead);
    mSceneNode->translate(position);
    mSceneNode->setScale(Ogre::Vector3::UNIT_SCALE * 10 / 16.0);
}

void FallingObject::addToPhysics(btDiscreteDynamicsWorld* dynamicsWorld)
{
	//Creates physics collision shape
    btCollisionShape* ogreHead = new btSphereShape(3 * 10 / 16.0);
	//Gives starting position of object, worth changing this and seeing effects of colliding with Tower, currently (-10,800,0)
    
    btVector3 position(this->position.x, this->position.y, this->position.z);
    
    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), position));
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    ogreHead->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, ogreHead, fallInertia);
    fallRigidBody = new btRigidBody(fallRigidBodyCI);
	
    dynamicsWorld->addRigidBody(fallRigidBody); // 2, Tower::COLLISION_GROUP & 4);
}

void FallingObject::update(void)
{
    btTransform trans;
    fallRigidBody->getMotionState()->getWorldTransform(trans);
    Ogre::Vector3 pos(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ());
    mSceneNode->setPosition(pos);
    /*mSceneNode->yaw(Ogre::Degree(0.5));
    mSceneNode->pitch(Ogre::Degree(0.1));*/
}

