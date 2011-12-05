#ifndef __Player_h_
#define __Player_h_

#include "SceneObject.h"

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
    virtual void shoot();
    virtual void platform();

private:
    Ogre::Real mMove;     //The movement constant
    Ogre::Vector3 mDirection;     //Value to move in the correct direction
    Ogre::SceneNode *playerNode;
};

#endif // #ifndef __Player_h_
