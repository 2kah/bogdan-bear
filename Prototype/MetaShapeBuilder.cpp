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
	int height = (rand() % (chunk->bounds.level_top - chunk->bounds.level_bottom)) + 1;
	int depth = (rand() % (chunk->bounds.layer_outer - chunk->bounds.layer_inner)) + 1;
	int width = (rand() % (chunk->bounds.sector_right - chunk->bounds.sector_left)) + 1;
	for(int levels = 0; levels < height; levels++)
	{
		for(int layers = 0; layers < depth; layers++)
		{
			for(int sectors = 0; sectors < width; sectors++)
			{
				metaShape->coords.push_back(Triple(levels + chunk->bounds.level_bottom, layers + chunk->bounds.layer_inner, sectors + chunk->bounds.sector_left));
			}
		}
	}
}