#ifndef __Builder_h_
#define __Builder_h_

#include "Updatable.h"
#include "Tower.h"

class Builder : public Updatable, public TowerListener
{
public:
    Tower *tower;

    Builder();
    Builder(Tower *tower);
    virtual ~Builder(void);

    virtual void update(void);
    virtual void regenerate(void);

    virtual void blocksUpdated(unsigned level);

protected:
    unsigned timer;
    unsigned level;
};

#endif // #ifndef __Builder_h_
