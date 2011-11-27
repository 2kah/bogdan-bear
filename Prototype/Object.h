#ifndef __Object_h_
#define __Object_h_

#include <string>

#include <OgreSceneManager.h>

class Object
{
public:
    Object();
    Object(Ogre::SceneManager *sceneMgr, Ogre::Vector3 position, std::string name);
    virtual ~Object(void);

    virtual void update(void);

protected:
    Ogre::SceneNode *mSceneNode;
};

#endif // #ifndef __Object_h_
