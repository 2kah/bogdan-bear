#ifndef __TowerBuilder_h_
#define __TowerBuilder_h_

#include <vector>

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
    TowerBuilder(Tower *tower);
    virtual ~TowerBuilder(void);

    virtual void update(void);
	virtual void generate(void);
    virtual void regenerate(void);

    virtual void blocksUpdated(int blocksDestroyed);

    Tower *tower;
	MetaShapeBuilder *metaShapeBuilder;
protected:
    unsigned timer;
    unsigned level;

	int blocksAvailable;
	int maxRegeneratingMetaShapes;
	int regeneratingMetaShapes;
	int maxBlocks;
	int activeBlocks;

	std::vector<std::vector<std::vector<BuilderChunk>>> chunks;
	std::vector<MetaShape> metaShapes;
};

#endif // #ifndef __TowerBuilder_h_
