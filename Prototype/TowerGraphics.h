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

    virtual void blocksUpdated(void);

protected:
    Tower *tower;
    Ogre::SceneManager *sceneManager;

    std::vector<Ogre::Entity *> blockEntities;

    Ogre::StaticGeometry* geometry;

    virtual void createBlockEntities(void);
public: // for now
    virtual void rebuildStaticGeometry(void);
};

#endif // #ifndef __TowerGraphics_h_
