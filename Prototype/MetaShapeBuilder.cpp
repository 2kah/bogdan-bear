#include "MetaShapeBuilder.h"

#include <algorithm>

MetaShapeBuilder::MetaShapeBuilder(boost::random::mt19937 &gen, Tower *tower)
{
	this->gen = gen;
	this->tower = tower;
}

MetaShapeBuilder::~MetaShapeBuilder()
{
}

void MetaShapeBuilder::getMetaShape(int type, BuilderChunk *chunk, MetaShape *metaShape)
{
	switch(type)
	{
	case 0:
		return makePlatform(chunk, metaShape);
	default:
		return makePlatform(chunk, metaShape);
	}
}

void MetaShapeBuilder::makePlatform(BuilderChunk *chunk, MetaShape *metaShape)
{
	BoundingVolume platformBounds;
	//randomly generate the size of the platform
	boost::random::uniform_int_distribution<> levelTopDist(chunk->bounds.level_bottom, chunk->bounds.level_top - 2);
	platformBounds.level_bottom = levelTopDist(gen);
	boost::random::uniform_int_distribution<> levelBottomDist(platformBounds.level_bottom +1, chunk->bounds.level_top - 1);
	platformBounds.level_top = levelBottomDist(gen);
	boost::random::uniform_int_distribution<> layerInnerDist(chunk->bounds.layer_inner, chunk->bounds.layer_outer - 2);
	platformBounds.layer_inner = layerInnerDist(gen);
	boost::random::uniform_int_distribution<> layerOuterDist(platformBounds.layer_inner +1, chunk->bounds.layer_outer - 1);
	platformBounds.layer_outer = layerOuterDist(gen);

	unsigned divisions = this->tower->blocks[0][platformBounds.layer_outer].size();

	int outerLeft = chunk->bounds.sector_left_layer(this->tower, platformBounds.layer_outer);
	int outerRight = (chunk->bounds.sector_right_layer(this->tower, platformBounds.layer_outer) - 1) % divisions;
	
	int maxWidth = outerRight - outerLeft;
	//5 is the minimum width of a platform
	int minWidth = std::min(5, maxWidth);

	boost::random::uniform_int_distribution<> widthDist(minWidth, maxWidth);
	int width = widthDist(gen);

	
	
	boost::random::uniform_int_distribution<> sectorLeftDist(outerLeft, outerRight - width);
	int left = sectorLeftDist(gen);
	int right = left + width;

    double ratio = (double) this->tower->sectors / (double) divisions;
    platformBounds.sector_left = (int) (left * ratio);
    platformBounds.sector_right = (int) (right * ratio);

	for (unsigned layer = platformBounds.layer_inner; layer < platformBounds.layer_outer; ++layer)
    {
        unsigned divisions = this->tower->blocks[0][layer].size();

        double ratio = divisions / (double) this->tower->sectors;
        int left = (int) (platformBounds.sector_left * ratio) % divisions;
        int right = (int) (platformBounds.sector_right * ratio) % divisions;

        for (unsigned level = platformBounds.level_bottom; level < platformBounds.level_top; ++level)
        {
            if (left <= right) {
                for (int sector = left; sector <= right; ++sector)
                {
					metaShape->coords.push_back(Triple(level, layer, sector));
                }
            } else {
				std::cout << "bug in makePlatform" << std::endl;
                /*for (int sector = left; sector < divisions; ++sector)
                {
                    metaShape->coords.push_back(Triple(level, layer, sector));
                }
                for (int sector = 0; sector <= right; ++sector)
                {
                    metaShape->coords.push_back(Triple(level, layer, sector));
                }*/
            }
        }
    }
}

void MetaShapeBuilder::makeStaircase(MetaShape *metaShape)
{
	for (unsigned level = 0; level < this->tower->levels; ++level)
    {
        for (unsigned layer = 20; layer < this->tower->layers; ++layer)
        {
            unsigned sectors = this->tower->blocks[level][layer].size();

            unsigned stair_sector = level % sectors;

            for (unsigned height = 0; height < 8; ++height)
            {
                //tower->blocks[(level + height) % tower->levels][layer][stair_sector] = false;
				metaShape->coords.push_back(Triple((level + height) % this->tower->levels, layer, stair_sector));
            }
        }
    }
}