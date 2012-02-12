#include "TowerBuilder.h"

#include <boost/signal.hpp>
#include <vector>

#include "MetaShapeBuilder.h"
#include "Tower.h"

BuilderChunk::BuilderChunk(Tower *tower, BoundingVolume bounds) : TowerChunk(tower, bounds)
{
}

BuilderChunk::~BuilderChunk()
{
}

bool BuilderChunk::empty()
{
	//iterate over all blocks in chunk and count how many there are total and how many there are active
	int blocksTotal = 0;
	int blocksActive = 0;
	for(unsigned layer = this->bounds.layer_inner; layer < this->bounds.layer_outer; layer++)
	{
		unsigned divisions = this->tower->blocks[0][layer].size();

        double ratio = divisions / (double) this->tower->sectors;
        int left = (int) (this->bounds.sector_left * ratio) % divisions;
        int right = (int) ((this->bounds.sector_right-1) * ratio) % divisions;

        for (unsigned sector = left; sector <= right; ++sector)
        {
            for (unsigned level = this->bounds.level_bottom; level < this->bounds.level_top; ++level)
            {
				blocksTotal++;
                if (this->tower->blocks[level][layer][sector])
                {
					blocksActive++;
                }
            }
        }
	}
	if(((float) blocksActive / (float) blocksTotal) < 0.1)
	{
		return true;
	}
	return false;
}

TowerBuilder::TowerBuilder(Tower *tower)
{
    this->tower = tower;

    this->timer = 0;
    this->level = this->tower->levels - 1;

	this->tower->signals.updated.connect(boost::bind(&TowerBuilder::blocksUpdated, this, _3));

	//TODO: make this take a random seed generated by server (eg. time of day)
	srand(5000);
	this->metaShapeBuilder = new MetaShapeBuilder();

	//TODO: make cvar
	this->maxRegeneratingMetaShapes = 10;
	this->regeneratingMetaShapes = 0;

	//TODO: make this code generic
	BoundingVolume bounds;

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

				layerVector.push_back(BuilderChunk(tower, bounds));
            }
			levelVector.push_back(layerVector);
        }
		this->chunks.push_back(levelVector);
    }
}

TowerBuilder::~TowerBuilder(void)
{
	delete this->metaShapeBuilder;
}

void TowerBuilder::update(void)
{
	this->timer++;

	if(this->timer == 50)
	{
		this->timer = 0;
		this->regenerate();
	}
    //return; // rebuilding static geometry too slow atm
    
    /*++this->timer;

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
    }*/
}

void TowerBuilder::blocksUpdated(int blocksDestroyed)
{
	if(blocksDestroyed > 0)
	{
		this->blocksAvailable += blocksDestroyed;
	}
	std::cout << "blocks available = " << blocksAvailable << std::endl;
}

void TowerBuilder::generate()
{
	//TODO: tweak this
	this->blocksAvailable = 50000;

	//TODO: make this generic
	while(this->blocksAvailable > 399)
	{
		this->regenerate();
	}
}

void TowerBuilder::regenerate(void)
{
	
	//Only proceed if there are actually any missing blocks to regenerate
	if(this->blocksAvailable == 0)
	{
		return;
	}

	//first work out whether to start a new meta shape or not
	//if max regnerating then obviously don't start any more
	if(regeneratingMetaShapes < maxRegeneratingMetaShapes)
	{
		//if no meta shapes or a lot of blocks available (given the number of meta shapes already regenerating) then start a new meta shape
		//TODO: tweak the value (currently 30) defining how many blocks per meta shape is a lot
		if(regeneratingMetaShapes == 0 || (blocksAvailable / regeneratingMetaShapes) > 30)
		{
			regeneratingMetaShapes++;
			//find a relatively empty chunk
			int randLevel, randLayer, randSector;
			do
			{
				randLevel = rand() % this->chunks.size();
				//TODO: make this more likely to choose lower numbers
				randLayer = rand() % this->chunks[randLevel].size();
				randSector = rand() % this->chunks[randLevel][randLayer].size();
			}
			while (!this->chunks[randLevel][randLayer][randSector].empty());
			MetaShape metaShape;
			metaShape.chunk.level = randLevel;
			metaShape.chunk.layer = randLayer;
			metaShape.chunk.sector = randSector;
			this->metaShapeBuilder->getMetaShape(0, &this->chunks[randLevel][randLayer][randSector], &metaShape);
			this->metaShapes.push_back(metaShape);
			//randomly pick which meta shape to generate (if 'open contact' then one which fits)
			//TODO: vary the size of meta shapes
		}
	}

	//only regenerate one meta shape per frame
	//TODO: neaten this up
	int shapeIndex = rand() % this->regeneratingMetaShapes;
	MetaShape metaShape = this->metaShapes[shapeIndex];
	int blocksAdded = 0;
	for(int j = 0; j < metaShape.coords.size() && j < blocksAvailable * 0.7; j++)
	{
		if(this->blocksAvailable == 0)
		{
			break;
		}
		Triple coords = metaShape.coords[j];
		if(!this->tower->blocks[coords.level][coords.layer][coords.sector])
		{
			this->tower->blocks[coords.level][coords.layer][coords.sector] = true;
			blocksAvailable--;
			blocksAdded++;
		}
		//if meta shape has been fully regenerated then remove it
		if(j == metaShape.coords.size() - 1)
		{
			regeneratingMetaShapes--;
			this->metaShapes.erase(metaShapes.begin() + shapeIndex);
			break;
		}
		//distribute some of the available blocks over the regenerating meta shape
	}
	this->tower->signals.updated(this->tower, this->chunks[metaShape.chunk.level][metaShape.chunk.layer][metaShape.chunk.sector].bounds, -blocksAdded);

    //for (unsigned level = 0; level < this->tower->levels; ++level)
    //{
    //    for (unsigned layer = 0; layer < this->tower->layers; ++layer)
    //    {
    //        for (unsigned sector = 0; sector < this->tower->blocks[level][layer].size(); ++sector)
    //        {
    //            this->tower->blocks[level][layer][sector] = true; //(rand() % 3 != 1);
    //        }
    //    }
    //}

    //for (unsigned level = 0; level < this->tower->levels; ++level)
    //{
    //    for (unsigned layer = 9; layer < this->tower->layers; ++layer)
    //    {
    //        unsigned sectors = this->tower->blocks[level][layer].size();

    //        unsigned stair_sector = level % sectors;

    //        for (unsigned height = 0; height < 16; ++height)
    //        {
    //            this->tower->blocks[(level + height) % this->tower->levels][layer][stair_sector] = false;
    //        }
    //    }
    //}

}
