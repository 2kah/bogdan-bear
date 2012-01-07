#ifndef __Tower_h_
#define __Tower_h_

#include <vector>

#include <boost/signal.hpp>

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

    Tower(double blocksize, unsigned levels, unsigned layers, unsigned sectors);
    virtual ~Tower();

    virtual void update();

    virtual void rebuild();
    virtual void synchronise();

    virtual BlockPosition getBlockPosition(unsigned level, unsigned layer, unsigned sector);
    virtual BlockPoints getBlockPoints(unsigned level, unsigned layer, unsigned sector);

    TowerSignals signals;

//protected:
    double blocksize;
    unsigned levels;
    unsigned layers;
    unsigned sectors;

    std::vector<std::vector<std::vector<bool> > > blocks;
};

#endif // #ifndef __Tower_h_
