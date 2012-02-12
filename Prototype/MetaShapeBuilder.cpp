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
	//TODO: allow resizing of the generated platform
	for(int levels = 0; levels < 4; levels++)
	{
		for(int layers = 0; layers < 10; layers++)
		{
			for(int sectors = 0; sectors < 10; sectors++)
			{
				metaShape->coords.push_back(Triple(levels + chunk->bounds.level_bottom, layers + chunk->bounds.layer_inner, sectors + chunk->bounds.sector_left));
			}
		}
	}
}