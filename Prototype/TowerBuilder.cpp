#include "TowerBuilder.h"
#include <boost/signal.hpp>
#include <vector>

#include "MetaShapeBuilder.h"
#include "Tower.h"
#include <time.h>

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
    this->tower->signals.removed.connect(boost::bind(&TowerBuilder::towerRemoved, this, _1));

	//seed the random number generator with the time
	int seed = std::time(NULL);
	gen.seed(seed);
	this->metaShapeBuilder = new MetaShapeBuilder(gen, this->tower);

	//TODO: tweak
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

void TowerBuilder::Init(void)
{
	this->maxBlocks = 0;
	for (int i =0; i < this->tower->levels; i++)
		for (int j =0; j < this->tower->layers; j++)
			for (int k =0; k < this->tower->blocks[i][j].size(); k++)
			{
				this->maxBlocks++;
				this->tower->blocks[i][j][k] = false;
			}
	this->blocksAvailable=this->maxBlocks;
}

void TowerBuilder::update(void)
{
	if (!isPaused)
	{
		this->timer++;

		if(this->timer == 100)
		{
			this->timer = 0;
			this->regenerate();
		}
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
}

void TowerBuilder::generate()
{
	//TODO: tweak this
	this->blocksAvailable = 18000;

	MetaShape metaShape;
	metaShape.chunk.level = 0;
	metaShape.chunk.layer = 0;
	metaShape.chunk.sector = 0;
	this->metaShapeBuilder->makeStaircase(&metaShape);
	this->metaShapes.push_back(metaShape);
	this->regeneratingMetaShapes = 1;

	//TODO: make this generic
	while(this->blocksAvailable > 999)
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
		if(regeneratingMetaShapes == 2 || (blocksAvailable / regeneratingMetaShapes) > 30)
		{
			regeneratingMetaShapes++;
			//find a relatively empty chunk
			int randLevel, randLayer, randSector;
			boost::random::uniform_int_distribution<> levelDist(0, this->chunks.size() - 1);
			do
			{
				randLevel = levelDist(gen);
				boost::random::uniform_int_distribution<> layerDist(0, this->chunks[randLevel].size() - 1);
				randLayer = layerDist(gen);
				boost::random::uniform_int_distribution<> sectorDist(0, this->chunks[randLevel][randLayer].size() - 1);
				randSector = sectorDist(gen);
			}
			while (!this->chunks[randLevel][randLayer][randSector].empty());
			MetaShape metaShape;
			metaShape.chunk.level = randLevel;
			metaShape.chunk.layer = randLayer;
			metaShape.chunk.sector = randSector;
			this->metaShapeBuilder->getMetaShape(0, &this->chunks[randLevel][randLayer][randSector], &metaShape);
			this->metaShapes.push_back(metaShape);
		}
	}

	//only regenerate one meta shape per frame
	//pick which meta shape to regenerate
	boost::random::uniform_int_distribution<> shapeDist(0, this->regeneratingMetaShapes - 1);
	int shapeIndex = shapeDist(gen);
	//int shapeIndex = rand() % this->regeneratingMetaShapes;
	MetaShape metaShape = this->metaShapes[shapeIndex];
	//TODO: tweak maxBlocksToRegen
	int blocksAdded = 0, maxBlocksToRegen = blocksAvailable * 0.15;
	//special case code for staircase as it doesn't fit into one chunk
	if(shapeIndex == 0)
	{
		boost::random::uniform_int_distribution<> staircaseDist(0, metaShape.coords.size() - 20);
		unsigned start = staircaseDist(gen);
		unsigned i;
		for(i = start; i <= (start + maxBlocksToRegen) && i < metaShape.coords.size() && blocksAvailable > 0; i++)
		{
			Triple coords = metaShape.coords[i];
			if(!this->tower->blocks[coords.level][coords.layer][coords.sector])
			{
				//TODO: graphical fade in + collision detection
				this->tower->blocks[coords.level][coords.layer][coords.sector] = true;
				blocksAvailable--;
				blocksAdded++;
			}
		}
		this->tower->signals.updated(this->tower, BoundingVolume(metaShape.coords[start].level, metaShape.coords[i].level, 20, this->tower->layers, 0, this->tower->sectors), -blocksAdded);
	}
	else
	{
		for(int i = 0; i <= maxBlocksToRegen && blocksAvailable > 0; i++)
		{
			if(metaShape.coords.size() == 0)
			{
				std::cout << "metashape is prematurely empty" << std::endl;
				regeneratingMetaShapes--;
				this->metaShapes.erase(metaShapes.begin() + shapeIndex);
				break;
			}
			//get the last set of coords in the meta shape and remove them from the data structure
			Triple coords = metaShape.coords.back();
			metaShape.coords.pop_back();
			//if the block the coords reference is inactive then activate it
			if(!this->tower->blocks[coords.level][coords.layer][coords.sector])
			{
				//TODO: graphical fade in + collision detection
				this->tower->blocks[coords.level][coords.layer][coords.sector] = true;
				blocksAvailable--;
				blocksAdded++;
			}
			//if meta shape has been fully regenerated then remove it
			if(metaShape.coords.size() == 0)
			{
				regeneratingMetaShapes--;
				this->metaShapes.erase(metaShapes.begin() + shapeIndex);
				break;
			}
		}
		BoundingVolume& b = this->chunks[metaShape.chunk.level][metaShape.chunk.layer][metaShape.chunk.sector].bounds;
		this->network->broadcastUpdateTower(b.level_bottom, b.level_top, b.layer_inner, b.layer_outer);
		this->tower->signals.updated(this->tower, this->chunks[metaShape.chunk.level][metaShape.chunk.layer][metaShape.chunk.sector].bounds, -blocksAdded);

	}
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

void TowerBuilder::towerRemoved(Tower *tower)
{
    delete this;
}
