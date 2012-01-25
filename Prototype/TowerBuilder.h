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

    virtual void blocksUpdated(Tower *tower, unsigned level);

    Tower *tower;
protected:
    unsigned timer;
    unsigned level;

	int blocksAvailable;
	int maxRegeneratingMetaShapes;
	int regeneratingMetaShapes;
};

#endif // #ifndef __TowerBuilder_h_
