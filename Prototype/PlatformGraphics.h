#ifndef __PlatformGraphics_h_
#define __PlatformGraphics_h_

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>

class Platform;

class PlatformGraphics
{
public:
    PlatformGraphics(Platform *platform, Ogre::SceneManager *sceneManager);
    virtual ~PlatformGraphics();
protected:
    virtual void platformUpdated(Platform *platform);
    virtual void platformExpired(Platform *platform);

    Platform *platform;

    Ogre::SceneManager *sceneManager;
    Ogre::SceneNode *sceneNode;
    Ogre::Entity *entity;
};

#endif // #ifndef __PlatformGraphics_h_
