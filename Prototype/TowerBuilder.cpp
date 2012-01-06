#include "TowerBuilder.h"

#include <boost/signal.hpp>

#include "Tower.h"

TowerBuilder::TowerBuilder(Tower *tower)
{
    this->tower = tower;

    this->timer = 0;
    this->level = this->tower->levels - 1;
}

TowerBuilder::~TowerBuilder(void)
{
}

void TowerBuilder::update(void)
{
    //return; // rebuilding static geometry too slow atm
    
    ++this->timer;

    if (timer >= 20 && this->level >= 0)
    {
        this->timer = 0;

        for (unsigned layer = 0; layer < this->tower->layers; ++layer)
        {
            for (unsigned sector = 0; sector < this->tower->sectors; ++sector)
            {
                this->tower->blocks[this->level][layer][sector] = true;
            }
        }

        this->tower->signals.levelUpdated(this->tower, level);

        if (this->level != 0) {
            --this->level;
        }
    }
}

void TowerBuilder::blocksUpdated(Tower *tower, unsigned level)
{
}

void TowerBuilder::regenerate(void)
{    
//#define USE_OLD_GENERATOR
#ifndef USE_OLD_GENERATOR
    for (unsigned level = 0; level < this->tower->levels; ++level)
    {
        for (unsigned layer = 0; layer < this->tower->layers; ++layer)
        {
            unsigned stair_sector = level % this->tower->sectors;
            unsigned rail_sector = (level + 1) % this->tower->sectors;
            
            this->tower->blocks[level][layer][stair_sector] = true;
            this->tower->blocks[level][layer][rail_sector] = true;
            this->tower->blocks[(level + 1) % this->tower->levels][this->tower->layers - 1][stair_sector] = true;

            for (unsigned sector = 0; sector < this->tower->sectors; ++sector)
            {
                if (layer < 8) {
                    this->tower->blocks[level][layer][sector] = (rand() % (layer + 1) <= 1);
                }
            }
        }
    }
#else
    //Loop through 3D array, filling with random 0's and 1's
    for (unsigned height = 0; height < this->tower->levels; ++height)
    {
        for (unsigned radius = 0; radius < this->tower->layers; ++radius)
        {
            for (unsigned position = 0; position < this->tower->sectors; ++position)
            {
                //Worth looking at different seeds for this
                unsigned x = rand() % this->tower->levels; 

                //Makes the tower more sparse at the top - Larger the height, less likely the blocks
                bool block = x >= (height - 2);

                //Makes the tower taper at the top - Bigger radiuses at top are cut off
                block = block && !((floor((double)(this->tower->levels-height)/radius)) <= radius);
                //if (height == (h-1)) x = 1;

                this->tower->blocks[height][radius][position] = block;
            }
        }
    }
#endif
}
