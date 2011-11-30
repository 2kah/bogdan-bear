#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <btBulletDynamicsCommon.h>

#include "FallingObject.h"

FallingObject::FallingObject(Ogre::Vector3 position)
{
    this->position = position;
}

FallingObject::~FallingObject(void)
{
}

void FallingObject::addToScene(Ogre::SceneManager *sceneMgr, std::string name)
{
    Ogre::Entity* ogreHead = sceneMgr->createEntity(name, "ogrehead.mesh");

    mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(ogreHead);

    mSceneNode->translate(position);
}

void FallingObject::addToPhysics(btDiscreteDynamicsWorld* dynamicsWorld)
{
    btCollisionShape* ogreHead = new btSphereShape(1);

    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    ogreHead->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, ogreHead, fallInertia);
    fallRigidBody = new btRigidBody(fallRigidBodyCI);
    dynamicsWorld->addRigidBody(fallRigidBody);
}

void FallingObject::update(void)
{
    btTransform trans;
    fallRigidBody->getMotionState()->getWorldTransform(trans);
    Ogre::Vector3 pos(0,trans.getOrigin().getY(),0);
    mSceneNode->setPosition(pos);
    /*mSceneNode->yaw(Ogre::Degree(0.5));
    mSceneNode->pitch(Ogre::Degree(0.1));*/
}

