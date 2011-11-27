#ifndef __SceneObject_h_
#define __SceneObject_h_

#include <string>

#include <OgreSceneManager.h>

#include "Object.h"

class SceneObject: public Object
{
public:
    SceneObject();
    SceneObject(Ogre::Vector3 position);
    virtual ~SceneObject(void);

    virtual void addToScene(Ogre::SceneManager *sceneMgr, std::string name);
    virtual void update(void);

protected:
    Ogre::SceneNode *mSceneNode;
    Ogre::Vector3 position;
};

#endif // #ifndef __SceneObject_h_
