#ifndef __Player_h_
#define __Player_h_

#include "SceneObject.h"

//#include "Playercam.h"

class Player: public SceneObject
{
public:
    Player(Ogre::Vector3 position);
    virtual ~Player(void);

    virtual void addToScene(Ogre::SceneManager *sceneMgr, std::string name);
    virtual void update(void);

    virtual void forward();
    virtual void back();
    virtual void left();
    virtual void right();

    virtual void stopMovingForward();
    virtual void stopMovingBack();
    virtual void stopMovingLeft();
    virtual void stopMovingRight();

    virtual void jump();
    virtual void shoot(Ogre::SceneManager *mSceneMgr, btDiscreteDynamicsWorld* dynamicsWorld);
    virtual void platform();

    virtual void lookX(int dist);
    virtual void lookY(int dist);

    //TODO: make this nicer?
    Ogre::SceneNode *cameraNode;

private:
    Ogre::Real mMove;     //The movement constant
    Ogre::Vector3 mDirection;     //Value to move in the correct direction
    Ogre::SceneNode *playerNode;

    Ogre::Radian rotX;
    Ogre::Radian rotY;
    float rotationFactor; //TODO: scale this to change mouse sensitivity

};

#endif // #ifndef __Player_h_
