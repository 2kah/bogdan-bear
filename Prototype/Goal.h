#ifndef __Goal_h_
#define __Goal_h_

#include <btBulletDynamicsCommon.h>

#include "Updatable.h"
#include "Object.h"

class btDiscreteDynamicsWorld;
class btGhostObject;

class Tower;

class Goal: public Updatable, public Object
{
public:
    Goal(Ogre::Vector3 position, btDiscreteDynamicsWorld *dynamicsWorld);

    virtual ~Goal();

	virtual void update();

protected:

    btDiscreteDynamicsWorld *dynamicsWorld;
    
    btGhostObject *ghost;

    Tower *tower;
};

#endif // #ifndef __Goal_h_
