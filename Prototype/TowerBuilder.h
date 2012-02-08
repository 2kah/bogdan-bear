#ifndef __TowerBuilder_h_
#define __TowerBuilder_h_

#include <vector>

#include "Updatable.h"
#include "PhysicsObject.h"
#include "TowerChunk.h"

class Tower;

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
    virtual void regenerate(void);

    virtual void blocksUpdated(int blocksDestroyed);

    Tower *tower;
protected:
    unsigned timer;
    unsigned level;

	int blocksAvailable;
	int maxRegeneratingMetaShapes;
	int regeneratingMetaShapes;
	int maxBlocks;
	int activeBlocks;

	std::vector<std::vector<std::vector<BuilderChunk>>> chunks;
};

#endif // #ifndef __TowerBuilder_h_
