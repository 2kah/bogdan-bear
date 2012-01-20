#ifndef __TowerGraphics_h_
#define __TowerGraphics_h_

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreStaticGeometry.h>
#include <OGRE/OgreMeshSerializer.h>

class Tower;
class ComplexTower;

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

    std::vector<Ogre::Entity *> blockEntities;

    Ogre::StaticGeometry *geometry;

    Ogre::ManualObject *wholething; // TODO: temporary
    virtual void rebuildTowerObject(unsigned level, bool first);

    virtual void createBlockEntities(void);
    virtual void rebuildStaticGeometry(void);
};

class ComplexTowerGraphics : public TowerGraphics
{
public:
    ComplexTowerGraphics(ComplexTower *tower, Ogre::SceneManager *sceneManager);

    virtual void rebuildTowerObject(unsigned level, bool first);

    ComplexTower *tower;
};

#endif // #ifndef __TowerGraphics_h_
