#ifndef __Tower_h_
#define __Tower_h_

#include <string>

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <math.h>
#include <btBulletDynamicsCommon.h>

class Tower
{
public:
    Tower();
    Tower(Ogre::SceneManager *msceneMgr);
    virtual ~Tower(void);

    virtual void update(void);

protected:
    Ogre::SceneNode *mSceneNode;
};

#endif // #ifndef __Tower_h_
