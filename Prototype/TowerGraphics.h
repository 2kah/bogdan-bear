#ifndef __TowerGraphics_h_
#define __TowerGraphics_h_

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreStaticGeometry.h>
#include <OGRE/OgreMeshSerializer.h>

class Tower;

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

    Ogre::ManualObject *wholething; // TODO: temporary
    virtual void rebuildTowerObject(unsigned level, bool first);
};

#endif // #ifndef __TowerGraphics_h_
