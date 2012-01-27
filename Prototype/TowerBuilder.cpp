#include "TowerBuilder.h"

#include <boost/signal.hpp>

#include "Tower.h"

TowerBuilder::TowerBuilder(Tower *tower)
{
    this->tower = tower;

    this->timer = 0;
    this->level = this->tower->levels - 1;

	/*
	//TODO: set to max number of blocks
	blocksAvailable = 1000;
	//TODO: make cvar
	maxRegeneratingMetaShapes = 5;
	regeneratingMetaShapes = 0;*/
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
	//Only proceed if there are actually any missing blocks to regenerate
	if(blocksAvailable == 0)
	{
		return;
	}

	//first work out whether to start a new meta shape or not, currently can only start 1 new meta shape per frame
	//if max regnerating then obviously don't start any more
	if(regeneratingMetaShapes < maxRegeneratingMetaShapes)
	{
		//if no meta shapes or a lot of blocks available (given the number of meta shapes already regenerating then start a new meta shape
		//TODO: tweak the value (currently 30) defining how many blocks per meta shape is a lot
		if(regeneratingMetaShapes == 0 || (blocksAvailable / regeneratingMetaShapes) > 30)
		{
			//start a new meta shape
			regeneratingMetaShapes++;
			//find a relatively empty chunk (high probability of an 'open contact')
			//randomly pick which meta shape to generate (if 'open contact' then one which fits)
			//decide the size
			//store which blocks would be filled by the meta shape if it were complete
		}
	}

	//iterate over all currently regenerating meta shapes
	for(int i = 0; i < regeneratingMetaShapes; i++)
	{
		//distribute some of the available blocks (randomly?) over the regenerating meta shapes
		//if a meta shape is finished then regeneratingMetaShapes--;
	}*/


    ///*
    for (unsigned level = 0; level < this->tower->levels; ++level)
    {
        for (unsigned layer = 0; layer < this->tower->layers; ++layer)
        {
            for (unsigned sector = 0; sector < this->tower->blocks[level][layer].size(); ++sector)
            {
                this->tower->blocks[level][layer][sector] = true; //(rand() % 3 != 1);
            }
        }
    }

    return;
    //*/

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
