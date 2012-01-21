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

    if (timer >= 20 && this->level >= 10)
    {
        this->timer = 0;

        for (unsigned layer = 0; layer < this->tower->layers; ++layer)
        {
            for (unsigned sector = 0; sector < this->tower->blocks[level][layer].size(); ++sector)
            {
                this->tower->blocks[this->level][layer][sector] = (rand() % (layer + 1) <= 1);
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
    /*
    for (unsigned level = 9; level < 10; ++level)
    {
        for (unsigned layer = 0; layer < this->tower->layers; ++layer)
        {
            for (unsigned sector = 0; sector < this->tower->blocks[level][layer].size(); ++sector)
            {
                this->tower->blocks[level][layer][sector] = (rand() % 3 != 1);
            }
        }
    }

    return;
    */

    for (unsigned level = 0; level < this->tower->levels; ++level)
    {
        for (unsigned layer = 0; layer < this->tower->layers; ++layer)
        {
            unsigned sectors = this->tower->blocks[level][layer].size();

            unsigned stair_sector = level % sectors;
            unsigned rail_sector = (level + 1) % sectors;
            
            this->tower->blocks[level][layer][stair_sector] = true;
            this->tower->blocks[level][layer][rail_sector] = true;
            this->tower->blocks[(level + 1) % this->tower->levels][this->tower->layers - 1][stair_sector] = true;

            for (unsigned sector = 0; sector < sectors; ++sector)
            {
                if (layer < 8) {
                    this->tower->blocks[level][layer][sector] = (rand() % (layer + 1) <= 1); // || (level < 10);
                }
            }
        }
    }
}
