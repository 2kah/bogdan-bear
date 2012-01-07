#ifndef __FallingObject_h_
#define __FallingObject_h_

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <btBulletDynamicsCommon.h>

#include "Updatable.h"
#include "Object.h"

class FallingObject : public Updatable, public Object
{
public:
    FallingObject(Ogre::Vector3 position);
    virtual ~FallingObject();

    virtual void addToScene(Ogre::SceneManager *sceneMgr);
    virtual void addToPhysics(btDiscreteDynamicsWorld* dynamicsWorld);
    virtual void update();

private:
    btRigidBody* fallRigidBody;
    Ogre::SceneNode *mSceneNode;
};

#endif // #ifndef __FallingObject_h_
