#ifndef __TowerBuilder_h_
#define __TowerBuilder_h_

#include "Updatable.h"

class Tower;

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
};

#endif // #ifndef __TowerBuilder_h_
