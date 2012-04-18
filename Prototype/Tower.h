#ifndef __Tower_h_
#define __Tower_h_

#include <vector>

#include <boost/signal.hpp>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreVector2.h>
#include <OGRE/OgreColourValue.h>

#include "TowerChunk.h"
#include "NetworkTestStuff.h"

// Points as shown in http://you.mongle.me/tower/circles/gamesproject.png
// base: a1, b1, c1, d1
//  top: a2, b2, c2, d2
// pent: e1, e2
struct BlockPoints {
    Ogre::Vector3 a1, b1, c1, d1;
    Ogre::Vector3 a2, b2, c2, d2;
    Ogre::Vector3 e1, e2;
};

struct BlockTriangle {
    Ogre::Vector3 points[3];
    Ogre::Vector3 normals[3];
    Ogre::ColourValue colours[3];
    Ogre::Vector2 coords[3];
};

class Tower;

struct BlockReference
{
    BlockReference(Tower *tower, unsigned level, unsigned layer, unsigned sector) {
        this->tower = tower;
        this->level = level;
        this->layer = layer;
        this->sector = sector;
    }

    Tower *tower;
    unsigned level, layer, sector;
};

namespace {
struct TowerSignals {
    boost::signal<void (Tower *, unsigned level)> levelUpdated;
    boost::signal<void (Tower *, BoundingVolume bounds, int blocksRemoved)> updated;
    boost::signal<void (Tower *)> removed;
};
}

class Tower : public boost::signals::trackable
{
public:
    static const short COLLISION_GROUP = 1; // 0000 0001
    static const short COLLISION_MASK  = 2; // 0000 0010

    Tower(unsigned levels, std::vector<unsigned> structure);
    virtual ~Tower();

    virtual void update();

    virtual void carveSphere(Ogre::Vector3 position, double radius);
    virtual void rebuild();
    virtual void synchronise();

    virtual Ogre::Vector3 getBlockPosition(unsigned level, unsigned layer, unsigned sector);
    virtual BlockPoints getBlockPoints(unsigned level, unsigned layer, unsigned sector);
    virtual void getBlockTriangles(std::vector<BlockTriangle> &triangles, unsigned level, unsigned layer, unsigned sector, BoundingVolume bounds);
    virtual void getChunkTriangles(std::vector<BlockTriangle> &triangles, TowerChunk chunk);

    TowerSignals signals;

    inline unsigned sectorParent(unsigned layer, unsigned sector);

//protected:
    unsigned levels;
    unsigned layers;
    unsigned sectors;

    double block_height;

    std::vector<std::vector<std::vector<bool> > > blocks;

    std::vector<bool> subdivide; // subdivide[layer] = does next layer subdivide?
    std::vector<double> radii;   //     radii[layer] = distance of blocks in layer from centre
    std::vector<double> heights; //   heights[layer] = how big is a block in this layer
};

#endif // #ifndef __Tower_h_
