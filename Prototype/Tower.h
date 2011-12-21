#ifndef __Tower_h_
#define __Tower_h_

#include <string>

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <math.h>
#include <btBulletDynamicsCommon.h>

#include "bullet/src/BulletWorldImporter/btBulletWorldImporter.h"


class Tower
{
public:
	Tower();
    Tower(Ogre::SceneManager *msceneMgr, btDiscreteDynamicsWorld* dynamicsWorld);
    virtual ~Tower(void);

    virtual void update(void);

protected:
    Ogre::SceneNode *mSceneNode;
	btCollisionShape* blockShape;
};

#endif // #ifndef __Tower_h_
