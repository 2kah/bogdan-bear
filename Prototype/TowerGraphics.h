#ifndef __TowerGraphics_h_
#define __TowerGraphics_h_

#include "Tower.h"

#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreStaticGeometry.h"

class TowerGraphics : public TowerListener
{
public:
    TowerGraphics();
    TowerGraphics(Tower *tower, Ogre::SceneManager *sceneManager);
    virtual ~TowerGraphics();

    virtual void blocksUpdated(unsigned level);

protected:
    Tower *tower;
    Ogre::SceneManager *sceneManager;

    std::vector<Ogre::Entity *> blockEntities;

    Ogre::StaticGeometry *geometry;

    Ogre::ManualObject *wholething; // TODO: temporary
    virtual void rebuildTowerObject(unsigned level, bool first);

    virtual void createBlockEntities(void);
    virtual void rebuildStaticGeometry(void);
};

#endif // #ifndef __TowerGraphics_h_
