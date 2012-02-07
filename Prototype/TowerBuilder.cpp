#include "TowerBuilder.h"

#include <boost/signal.hpp>
#include <vector>

#include "Tower.h"

BuilderChunk::BuilderChunk(Tower *tower, BoundingVolume bounds) : TowerChunk(tower, bounds)
{
}

BuilderChunk::~BuilderChunk()
{
}

bool BuilderChunk::empty()
{
	return true;
}

TowerBuilder::TowerBuilder(Tower *tower)
{
    this->tower = tower;

    this->timer = 0;
    this->level = this->tower->levels - 1;

	this->blocksAvailable = 0;
	this->tower->signals.updated.connect(boost::bind(&TowerBuilder::blocksUpdated, this, _3));

	/*
	//TODO: set to max number of blocks
	this->blocksAvailable = 10000;
	//TODO: make cvar
	this->maxRegeneratingMetaShapes = 5;
	this->regeneratingMetaShapes = 0;*/

	//TODO: make this code generic
	BoundingVolume bounds;
	//this->chunks = std::vector<std::vector<std::vector<BuilderChunk>>>(tower->levels/8, std::vector<std::vector<BuilderChunk>>(tower->layers/11, std::vector<BuilderChunk>(1, BuilderChunk(tower, bounds))));

	for (unsigned level = 0; level < this->tower->levels / 8; ++level)
    {
		std::vector<std::vector<BuilderChunk>> levelVector;
        for (unsigned layer = 0; layer < this->tower->layers / 11; ++layer)
        {
			std::vector<BuilderChunk> layerVector;
            for (unsigned sector = 0; sector < this->tower->sectors / 16; ++sector)
            {
                BoundingVolume bounds(level  * 8, (level+1)  * 8,
                                      //0,          this->tower->layers,
                                      layer  * 11, (layer+1)  * 11,
                                      //0,          this->tower->sectors);
                                      sector * 16, (sector+1) * 16);

				//this->chunks[level][layer] = std::vector<BoundingVolume>(this->tower->sectors/16, BuilderChunk(tower, bounds));
				layerVector.push_back(BuilderChunk(tower, bounds));
            }
			levelVector.push_back(layerVector);
        }
		this->chunks.push_back(levelVector);
    }
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

void TowerBuilder::blocksUpdated(int blocksDestroyed)
{
	this->blocksAvailable += blocksDestroyed;
	std::cout << "blocks available = " << blocksAvailable << std::endl;
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

    for (unsigned level = 0; level < this->tower->levels; ++level)
    {
        for (unsigned layer = 9; layer < this->tower->layers; ++layer)
        {
            unsigned sectors = this->tower->blocks[level][layer].size();

            unsigned stair_sector = level % sectors;

            for (unsigned height = 0; height < 16; ++height)
            {
                this->tower->blocks[(level + height) % this->tower->levels][layer][stair_sector] = false;
            }
        }
    }
}
