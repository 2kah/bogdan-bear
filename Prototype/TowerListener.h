#ifndef __TowerListener_h_
#define __TowerListener_h_

class TowerListener
{
public:
    virtual ~TowerListener() {};

    virtual void blocksUpdated(unsigned level) = 0;
};

#endif // #ifndef __TowerListener_h_
