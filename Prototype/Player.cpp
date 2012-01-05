#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <math.h>

#include "Player.h"

#define PI 3.14159265

Player::Player(Ogre::Vector3 position)
{
    this->position = position;

    //Movement speed - TODO: this should be a cvar
    mMove = 2;
    mDirection = Ogre::Vector3::ZERO;
    rotX = 0;
    rotationFactor = 0.05;
}

Player::~Player(void)
{
}

void Player::addToScene(Ogre::SceneManager *sceneMgr, std::string name)
{
    Ogre::Entity* playerEntity = sceneMgr->createEntity(name, "ninja.mesh");

    playerNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    playerNode->attachObject(playerEntity);

    playerNode->translate(position);

    //TODO: put this code somewhere nicer
    cameraNode = playerNode->createChildSceneNode();
    cameraNode->setPosition(0, 180, 0);
    //cameraNode->attachObject(mCamera);
}

void Player::update(void)
{
    /*mSceneNode->yaw(Ogre::Degree(0.5));
    mSceneNode->pitch(Ogre::Degree(0.1));*/
    playerNode->yaw(rotX);
    cameraNode->pitch(rotY);
    rotX = 0;
    rotY = 0;
    playerNode->translate(mDirection, Ogre::Node::TS_LOCAL);
}

void Player::forward(void)
{
    std::cout << "MOVING FORWARD" << std::endl;
    mDirection.z = -mMove;
}

void Player::stopMovingForward(void)
{
    mDirection.z = 0;
}

void Player::back(void)
{
    std::cout << "MOVING BACK" << std::endl;
    mDirection.z = mMove;
}

void Player::stopMovingBack(void)
{
    mDirection.z = 0;
}

void Player::left(void)
{
    std::cout << "MOVING LEFT" << std::endl;
    mDirection.x = -mMove;
}

void Player::stopMovingLeft(void)
{
    mDirection.x = 0;
}

void Player::right(void)
{
    std::cout << "MOVING RIGHT" << std::endl;
    mDirection.x = mMove;
}

void Player::stopMovingRight(void)
{
    mDirection.x = 0;
}

void Player::jump(void)
{
    std::cout << "JUMPING" << std::endl;
}

void Player::shoot(Ogre::SceneManager *mSceneMgr, btDiscreteDynamicsWorld* dynamicsWorld, TowerOld* tower)
{
    double l = 1000;
    double X = cameraNode->convertLocalToWorldPosition(Ogre::Vector3(0,0,0)).x;
    double Y = cameraNode->convertLocalToWorldPosition(Ogre::Vector3(0,0,0)).y;
    double Z = cameraNode->convertLocalToWorldPosition(Ogre::Vector3(0,0,0)).z;
    btVector3 rayFrom(X,Y,Z);
    printf("%f, %f, %f\n", X,Y,Z);
    printf("%f, %f\n", playerNode->getOrientation().getYaw().valueDegrees(), cameraNode->getOrientation().getPitch().valueDegrees());
    double camX1 = 0;
    double camY1 = (-1)*-sin(cameraNode->getOrientation().getPitch().valueRadians());
    double camZ1 = (-1)*cos(cameraNode->getOrientation().getPitch().valueRadians());
    printf("%f, %f, %f\n", camX1,camY1,camZ1);
    double camX = camZ1*sin(playerNode->getOrientation().getYaw().valueRadians());
    double camY = camY1;
    double camZ = camZ1*cos(playerNode->getOrientation().getYaw().valueRadians());
    printf("%f, %f, %f\n", camX,camY,camZ);
    double dirX = X+(l*camX);
    double dirY = Y+(l*camY);
    double dirZ = Z+(l*camZ);
    btVector3 rayTo(dirX,dirY,dirZ);
    printf("%f, %f, %f\n", dirX,dirY,dirZ);
    //btCollisionWorld collisionWorld  = *dynamicsWorld->getCollisionWorld();
    btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
 
    dynamicsWorld->rayTest(rayFrom,rayTo,rayCallback);
    if (rayCallback.hasHit())
    {
        printf("hit!\n");
        btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);
        if (body)
        {
            body->setActivationState(ACTIVE_TAG);
            //btVector3 impulse = rayTo;
            //impulse.normalize();
            //float impulseStrength = 10.f;
            //impulse *= impulseStrength;
            btVector3 relPos = rayCallback.m_hitPointWorld;// - body->getCenterOfMassPosition();
            //body->applyImpulse(impulse,relPos);
            dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
            btGhostObject* explosion = new btGhostObject();
            explosion->setCollisionShape(new btSphereShape(10));
            explosion->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
            explosion->setWorldTransform(btTransform(btQuaternion(0,0,0,1),btVector3(relPos.getX(),relPos.getY(),relPos.getZ())));
            dynamicsWorld->addCollisionObject(explosion,btBroadphaseProxy::SensorTrigger,btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
            //Ogre::Entity* ogreHead = mSceneMgr->createEntity("explosion", "spheredemo.mesh");
            //Ogre::SceneNode* mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
            //mSceneNode->attachObject(ogreHead);
            //mSceneNode->scale(10,10,10);
            //mSceneNode->translate(Ogre::Vector3(relPos.getX(),relPos.getY(),relPos.getZ()));
            printf("Explosion destroyed %i objects\n", explosion->getNumOverlappingObjects());
            printf("Sphere placed at: %f, %f, %f\n", relPos.getX(),relPos.getY(),relPos.getZ());
             
            for(int i = 0; i < explosion->getNumOverlappingObjects(); i++)
            {
                btCollisionObject* block = explosion->getOverlappingObject(i);
                btTransform tran = block->getWorldTransform();
                dynamicsWorld->removeCollisionObject(block); //Delete the physics object
                btVector3 pos = tran.getOrigin();
                double x = pos.getX();
                double y = pos.getY();
                double z = pos.getZ();
                double rads = atan(x/y);
                int h = ((double)y/15)*2;
                int r = ((sqrt(pow(x,2)+pow(y,2)))/15)-0.5;
                int nseg = 12*(pow((double)2,r)); //number of segments in radius r
                int p = ((rads/PI)-((double)1/nseg))*(nseg/2);
                printf("%i, %i, %i\n", h, r, p);
                //Delete graphic meshes h, r and p here. Athough p isn't quite right (
            }
        }
    }
    std::cout << "SHOOTING" << std::endl;
}

void Player::platform(void)
{
    std::cout << "CREATING PLATFORM" << std::endl;
}

void Player::lookX(int dist)
{
    rotX += Ogre::Degree(-dist * rotationFactor);
}

void Player::lookY(int dist)
{
    rotY += Ogre::Degree(-dist * rotationFactor);
}
