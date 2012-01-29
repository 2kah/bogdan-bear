#ifndef __RocketGraphics_h_
#define __RocketGraphics_h_

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>

class Rocket;
class Explosion;

class RocketGraphics
{
public:
    RocketGraphics(Rocket *rocket, Ogre::SceneManager *sceneManager);
    virtual ~RocketGraphics();
protected:
    virtual void rocketUpdated(Rocket *rocket);
    virtual void rocketExploded(Rocket *rocket, Explosion *explosion);

    Rocket *rocket;

    Ogre::SceneManager *sceneManager;
    Ogre::SceneNode *sceneNode;
    Ogre::Entity *entity;
};

#endif // #ifndef __RocketGraphics_h_
