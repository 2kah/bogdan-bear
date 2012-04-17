#ifndef __TowerBuilder_h_
#define __TowerBuilder_h_

#include <vector>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>

#include "Updatable.h"
#include "PhysicsObject.h"
#include "TowerChunk.h"

class MetaShapeBuilder;
class Tower;

struct Triple {
	Triple() {}
	Triple(int level, int layer, int sector) : level(level), layer(layer), sector(sector) {}

	int level, layer, sector;
};

struct MetaShape {
	MetaShape() {}
	MetaShape(Triple chunk, std::vector<Triple> coords) : chunk(chunk), coords(coords) {}

	Triple chunk;
	std::vector<Triple> coords;
};

class BuilderChunk : public TowerChunk, public PhysicsObject
{
public:
	BuilderChunk(Tower *tower, BoundingVolume bounds);
	virtual ~BuilderChunk();

	virtual bool empty();
};

class TowerBuilder : public Updatable
{
public:
	int maxBlocks;
	bool isPaused;
    TowerBuilder(Tower *tower);
    virtual ~TowerBuilder(void);

    virtual void update(void);
	virtual void generate(void);
    virtual void regenerate(void);

    virtual void blocksUpdated(int blocksDestroyed);
	void FullSync(bool* data);
	void Init();
	void InitFull();
	void GetTowerState(bool* Output);

    Tower *tower;
	MetaShapeBuilder *metaShapeBuilder;

	//pseudo random number generator
	boost::random::mt19937 gen;

protected:
    unsigned timer;
    unsigned level;

	int blocksAvailable;
	int maxRegeneratingMetaShapes;
	int regeneratingMetaShapes;
	int activeBlocks;

	std::vector<std::vector<std::vector<BuilderChunk>>> chunks;
	std::vector<MetaShape> metaShapes;
};

#endif // #ifndef __TowerBuilder_h_
