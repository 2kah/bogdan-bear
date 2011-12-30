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
class Tower
{
public:
	Tower();
    Tower(Ogre::SceneManager *msceneMgr, btDiscreteDynamicsWorld* dynamicsWorld);
    virtual ~Tower(void);

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

class TowerRefactor
{
public:
    static const short COLLISION_GROUP = 1; // 0000 0001
    static const short COLLISION_MASK  = 2; // 0000 0010

    TowerRefactor();
    TowerRefactor(double blocksize, unsigned levels, unsigned layers, unsigned sectors);
    virtual ~TowerRefactor(void);

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
    TowerGraphics(TowerRefactor *tower, Ogre::SceneManager *sceneManager);
    virtual ~TowerGraphics();

protected:
    TowerRefactor *tower;
};

class TowerPhysics : public TowerListener
{
public:
    TowerPhysics();
    TowerPhysics(TowerRefactor *tower, btDiscreteDynamicsWorld* dynamicsWorld);
    virtual ~TowerPhysics();

protected:
    TowerRefactor *tower;
};

#endif // #ifndef __Tower_h_
