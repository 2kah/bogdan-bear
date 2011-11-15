#ifndef __Object_h_
#define __Object_h_

#include <OgreSceneManager.h>

class Object
{
public:
    Object(Ogre::SceneManager *sceneMgr);
    virtual ~Object(void);

    virtual void update(void);

private:
    Ogre::SceneNode *mSceneNode;
};

#endif // #ifndef __Object_h_
