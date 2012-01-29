#ifndef __TowerChunk_h_
#define __TowerChunk_h_

class Tower;

struct BoundingVolume {
    BoundingVolume() {}
    BoundingVolume(unsigned level_bottom, unsigned level_top,
                   unsigned layer_inner,  unsigned layer_outer,
                   unsigned sector_left,  unsigned sector_right)
    : level_bottom(level_bottom), level_top(level_top)
    , layer_inner(layer_inner)  , layer_outer(layer_outer)
    , sector_left(sector_left)  , sector_right(sector_right)
    , wraps(sector_left > sector_right)
    {}

    bool collides(BoundingVolume volume)
    {
        if (this->level_bottom > volume.level_top    || this->level_top    < volume.level_bottom) return false;
        if (this->layer_inner  > volume.layer_outer  || this->layer_outer  < volume.layer_inner ) return false;
        
        if (!this->wraps && !volume.wraps) {
            if (this->sector_left > volume.sector_right || this->sector_right < volume.sector_left) return false;
        } else {
        }

        return true;
    }

    unsigned level_bottom, level_top;
    unsigned layer_inner, layer_outer;
    unsigned sector_left, sector_right;
    bool wraps;
};

class TowerChunk
{
public:
    TowerChunk()
        : tower(NULL),
        bounds(0, 0, 0, 0, 0, 0)
    {}
    TowerChunk(Tower *tower, BoundingVolume bounds)
        : tower(tower),
        bounds(bounds)
    {}

    Tower *tower;
    BoundingVolume bounds;
};

#endif // #ifndef __TowerChunk_h_
