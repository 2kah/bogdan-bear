#ifndef __Tower_h_
#define __Tower_h_

#include <vector>

#include <boost/signal.hpp>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreColourValue.h>

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
};
}

class Tower
{
public:
    static const short COLLISION_GROUP = 1; // 0000 0001
    static const short COLLISION_MASK  = 2; // 0000 0010

    Tower(double blocksize, unsigned levels, std::vector<unsigned> structure);
    virtual ~Tower();

    virtual void update();

    virtual void rebuild();
    virtual void synchronise();

    virtual BlockPosition getBlockPosition(unsigned level, unsigned layer, unsigned sector);
    virtual BlockPoints getBlockPoints(unsigned level, unsigned layer, unsigned sector);
    virtual std::vector<BlockTriangle> getBlockTriangles(unsigned level, unsigned layer, unsigned sector);


    TowerSignals signals;

//protected:
    double blocksize;
    unsigned levels;
    unsigned layers;
    unsigned sectors;

    std::vector<std::vector<std::vector<bool> > > blocks;

    std::vector<bool> subdivide; // subdivide[layer] = does next layer subdivide?
    std::vector<double> radii;   //     radii[layer] = distance of blocks in layer from centre
    std::vector<double> heights; //   heights[layer] = how big is a block in this layer
};

#endif // #ifndef __Tower_h_
