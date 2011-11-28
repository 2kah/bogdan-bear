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

    virtual void jump();
    virtual void shoot();
    virtual void platform();
};

#endif // #ifndef __Player_h_
