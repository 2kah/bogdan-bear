#ifndef __Explosion_h_
#define __Explosion_h_

#include <boost/signals.hpp>

#include <OGRE/OgreVector3.h>

#include "Updatable.h"
#include "Object.h"

class Explosion;

namespace {
struct ExplosionSignals {
    boost::signal<void (Explosion *)> finished;
};
}

class Explosion: public Updatable, public Object
{
public:
    Explosion(Ogre::Vector3 position);
    virtual ~Explosion();

    virtual void update();

    void addToScene(Ogre::SceneManager *, Ogre::String);
    void removeFromScene(Ogre::SceneManager *sceneMgr);

    ExplosionSignals signals;
protected:
    int size;
    Ogre::SceneNode *mSceneNode;
};

#endif // #ifndef __Explosion_h_
