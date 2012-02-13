#include "MetaShapeBuilder.h"

MetaShapeBuilder::MetaShapeBuilder()
{
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
	unsigned height = (rand() % (chunk->bounds.level_top - chunk->bounds.level_bottom)) + 1;
	unsigned depth = (rand() % (chunk->bounds.layer_outer - chunk->bounds.layer_inner)) + 1;
	unsigned width = (rand() % (chunk->bounds.sector_right - chunk->bounds.sector_left)) + 1;

	//randomly generate the placement of the platform within the chunk (guard against 0 modulus)
	unsigned levelPlacement = ((chunk->bounds.level_top - chunk->bounds.level_bottom) - height) == 0 ? 0 : rand() % ((chunk->bounds.level_top - chunk->bounds.level_bottom) - height);
	unsigned layerPlacement = ((chunk->bounds.layer_outer - chunk->bounds.layer_inner) - depth) == 0 ? 0 : rand() % ((chunk->bounds.layer_outer - chunk->bounds.layer_inner) - depth);
	unsigned sectorPlacement = ((chunk->bounds.sector_right - chunk->bounds.sector_left) - width) == 0 ? 0 : rand() % ((chunk->bounds.sector_right - chunk->bounds.sector_left) - width);

	height += levelPlacement;
	depth += layerPlacement;
	width += sectorPlacement;

	for(int levels = levelPlacement; levels < height; levels++)
	{
		for(int layers = layerPlacement; layers < depth; layers++)
		{
			for(int sectors = sectorPlacement; sectors < width; sectors++)
			{
				metaShape->coords.push_back(Triple(levels + chunk->bounds.level_bottom, layers + chunk->bounds.layer_inner, sectors + chunk->bounds.sector_left));
			}
		}
	}
}