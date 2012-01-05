#ifndef __ExplosionGraphics_h_
#define __ExplosionGraphics_h_

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>

class Explosion;

class ExplosionGraphics
{
public:
    ExplosionGraphics(Explosion *explosion, Ogre::SceneManager *sceneManager);
    virtual ~ExplosionGraphics();
protected:
    virtual void explosionUpdated(Explosion *explosion);
    virtual void explosionFinished(Explosion *explosion);

    Explosion *explosion;

    Ogre::SceneManager *sceneManager;
    Ogre::SceneNode *sceneNode;
    Ogre::Entity *entity;
};

#endif // #ifndef __ExplosionGraphics_h_