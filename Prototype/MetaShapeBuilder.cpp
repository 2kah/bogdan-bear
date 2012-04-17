#include "MetaShapeBuilder.h"

MetaShapeBuilder::MetaShapeBuilder(boost::random::mt19937 &gen)
{
	this->gen = gen;
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
	//randomly generate the size of the platform
	boost::random::uniform_int_distribution<> heightDist(1, (chunk->bounds.level_top - chunk->bounds.level_bottom) - 1);
	int height = heightDist(gen);
	boost::random::uniform_int_distribution<> depthDist(1, (chunk->bounds.layer_outer - chunk->bounds.layer_inner) - 1);
	int depth = depthDist(gen);
	boost::random::uniform_int_distribution<> widthDist(1, (chunk->bounds.sector_right - chunk->bounds.sector_left) - 1);
	int width = widthDist(gen);
	/*int height = (rand() % (chunk->bounds.level_top - chunk->bounds.level_bottom)) + 1;
	int depth = (rand() % (chunk->bounds.layer_outer - chunk->bounds.layer_inner)) + 1;
	int width = (rand() % (chunk->bounds.sector_right - chunk->bounds.sector_left)) + 1;*/

	/*//randomly generate the placement of the platform within the chunk (guard against 0 modulus)
	unsigned levelPlacement = ((chunk->bounds.level_top - chunk->bounds.level_bottom) - height) == 0 ? 0 : rand() % ((chunk->bounds.level_top - chunk->bounds.level_bottom) - height);
	unsigned layerPlacement = ((chunk->bounds.layer_outer - chunk->bounds.layer_inner) - depth) == 0 ? 0 : rand() % ((chunk->bounds.layer_outer - chunk->bounds.layer_inner) - depth);
	unsigned sectorPlacement = ((chunk->bounds.sector_right - chunk->bounds.sector_left) - width) == 0 ? 0 : rand() % ((chunk->bounds.sector_right - chunk->bounds.sector_left) - width);

	height += levelPlacement;
	depth += layerPlacement;
	width += sectorPlacement;*/

	for(int levels = 0; levels < height; levels++) //zero replaces levelPlacement
	{
		for(int layers = 0; layers < depth; layers++) //zero replaces layerPlacement
		{
			for(int sectors = 0; sectors < width; sectors++) //zero replaces sectorPlacement
			{
				metaShape->coords.push_back(Triple(levels + chunk->bounds.level_bottom, layers + chunk->bounds.layer_inner, sectors + chunk->bounds.sector_left));
			}
		}
	}
}

void MetaShapeBuilder::makeStaircase(Tower *tower, MetaShape *metaShape)
{
	for (unsigned level = 0; level < tower->levels; ++level)
    {
        for (unsigned layer = 20; layer < tower->layers; ++layer)
        {
            unsigned sectors = tower->blocks[level][layer].size();

            unsigned stair_sector = level % sectors;

            for (unsigned height = 0; height < 8; ++height)
            {
                //tower->blocks[(level + height) % tower->levels][layer][stair_sector] = false;
				metaShape->coords.push_back(Triple((level + height) % tower->levels, layer, stair_sector));
            }
        }
    }
}