#ifndef __Builder_h_
#define __Builder_h_

#include "Tower.h"

class Builder : public TowerListener
{
public:
    TowerRefactor *tower;

    Builder();
    Builder(TowerRefactor *tower);
    virtual ~Builder(void);

    virtual void regenerate(void);
};

#endif // #ifndef __Builder_h_
