#ifndef __TowerGraphics_h_
#define __TowerGraphics_h_

#include <vector>

#include <boost/signal.hpp>

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreStaticGeometry.h>
#include <OGRE/OgreMeshSerializer.h>

#include "TowerChunk.h"

class Tower;

class GraphicsChunk : public TowerChunk
{
public:
    GraphicsChunk(Tower *tower, BoundingVolume bounds, Ogre::SceneManager *sceneManager);
    virtual ~GraphicsChunk();

    virtual void rebuild();
protected:
    Ogre::SceneManager *sceneManager;
    Ogre::ManualObject *object;
};

class TowerGraphics : public boost::signals::trackable
{
public:
    TowerGraphics() {};
    TowerGraphics(Tower *tower, Ogre::SceneManager *sceneManager);
    virtual ~TowerGraphics();

    static const unsigned CHUNK_HEIGHT;
protected:
    virtual void towerUpdated(Tower *tower, BoundingVolume bounds);
    virtual void towerRemoved(Tower *tower);

    Tower *tower;
    Ogre::SceneManager *sceneManager;

    std::vector<GraphicsChunk *> chunks;
};

#endif // #ifndef __TowerGraphics_h_
