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
	int height = (rand() % (chunk->bounds.level_top - chunk->bounds.level_bottom)) + 1;
	int depth = (rand() % (chunk->bounds.layer_outer - chunk->bounds.layer_inner)) + 1;
	int width = (rand() % (chunk->bounds.sector_right - chunk->bounds.sector_left)) + 1;

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