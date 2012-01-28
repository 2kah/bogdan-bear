#ifndef __TowerGraphics_h_
#define __TowerGraphics_h_

#include <vector>

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

class TowerGraphics
{
public:
    TowerGraphics() {};
    TowerGraphics(Tower *tower, Ogre::SceneManager *sceneManager);
    virtual ~TowerGraphics();

    static const unsigned CHUNK_HEIGHT;
protected:
    void towerUpdated(Tower *tower, BoundingVolume bounds);

    Tower *tower;
    Ogre::SceneManager *sceneManager;

    std::vector<GraphicsChunk *> chunks;
};

#endif // #ifndef __TowerGraphics_h_
