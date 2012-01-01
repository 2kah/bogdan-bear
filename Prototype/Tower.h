#ifndef __Tower_h_
#define __Tower_h_

#include <string>
#include <vector>
#include <set>

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <math.h>
#include <btBulletDynamicsCommon.h>

#include <OgreVector3.h>

#include "bullet/src/BulletWorldImporter/btBulletWorldImporter.h"

class TowerListener;

///*
class TowerOld
{
public:
	TowerOld();
    TowerOld(Ogre::SceneManager *msceneMgr, btDiscreteDynamicsWorld* dynamicsWorld);
    virtual ~TowerOld(void);

    virtual void update(void);

protected:
    Ogre::SceneNode *mSceneNode;
	btCollisionShape* blockShape;
};
//*/

struct BlockPosition
{
    double x, y, z, angle;
};

// Because we don't have a unified vector3, not sure what's best
struct Point
{
    double x, y, z;
};

// Points as shown in http://you.mongle.me/tower/circles/gamesproject.png
// base: a1, b1, c1, d1
//  top: a2, b2, c2, d2
struct BlockPoints
{
    Point a1, b1, c1, d1;
    Point a2, b2, c2, d2;
};

class Tower
{
public:
    static const short COLLISION_GROUP = 1; // 0000 0001
    static const short COLLISION_MASK  = 2; // 0000 0010

    Tower();
    Tower(double blocksize, unsigned levels, unsigned layers, unsigned sectors);
    virtual ~Tower(void);

    virtual void update(void);

    virtual void carveSphere(Ogre::Vector3 position, double radius);
    virtual void rebuild();
    virtual void synchronise();

    virtual BlockPosition getBlockPosition(unsigned level, unsigned layer, unsigned sector);
    virtual BlockPoints getBlockPoints(unsigned level, unsigned layer, unsigned sector);

    void addTowerListener(TowerListener *listener);

//protected:
    std::set<TowerListener *> towerListeners;

    double blocksize;
    unsigned levels;
    unsigned layers;
    unsigned sectors;

    std::vector<std::vector<std::vector<bool> > > blocks;

private:
    void fireBlocksUpdated();
};

class TowerListener
{
public:
    TowerListener() {};
    virtual ~TowerListener() = 0;
};

class TowerGraphics : public TowerListener
{
public:
    TowerGraphics();
    TowerGraphics(Tower *tower, Ogre::SceneManager *sceneManager);
    virtual ~TowerGraphics();

protected:
    Tower *tower;
    Ogre::SceneManager *sceneManager;

    std::vector<Ogre::Entity *> blockEntities;

    Ogre::StaticGeometry* geometry;

    virtual void createBlockEntities(void);
    virtual void rebuildStaticGeometry(void);
};

class TowerPhysics : public TowerListener
{
public:
    TowerPhysics();
    TowerPhysics(Tower *tower, btDiscreteDynamicsWorld* dynamicsWorld);
    virtual ~TowerPhysics();

protected:
    Tower *tower;
};

#endif // #ifndef __Tower_h_
