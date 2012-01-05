#ifndef __FallingObject_h_
#define __FallingObject_h_

#include "SceneObject.h"

class FallingObject: public SceneObject
{
public:
    FallingObject(Ogre::Vector3 position);
    virtual ~FallingObject(void);

    virtual void addToScene(Ogre::SceneManager *sceneMgr);
    virtual void addToPhysics(btDiscreteDynamicsWorld* dynamicsWorld);
    virtual void update(void);

private:
    btRigidBody* fallRigidBody;
};

#endif // #ifndef __FallingObject_h_
