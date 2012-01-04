#ifndef __RocketGraphics_h_
#define __RocketGraphics_h_

#include <OgreSceneManager.h>

#include "RocketListener.h"

class Rocket;

class RocketGraphics : public RocketListener
{
public:
    RocketGraphics(Rocket *rocket, Ogre::SceneManager *sceneManager);
    virtual ~RocketGraphics();
protected:
    virtual void rocketUpdated(Rocket *rocket);

    Rocket *rocket;

    Ogre::SceneNode *sceneNode;
};

#endif // #ifndef __RocketGraphics_h_
