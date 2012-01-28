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

    virtual void blocksUpdated(Tower *tower, unsigned level);

protected:
    Tower *tower;
    Ogre::SceneManager *sceneManager;

    std::vector<GraphicsChunk *> chunks;

    Ogre::ManualObject *wholething; // TODO: temporary
    virtual void rebuildTowerObject(unsigned level, bool first);
};

#endif // #ifndef __TowerGraphics_h_
