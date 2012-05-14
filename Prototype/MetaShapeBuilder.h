#ifndef __MetaShapeBuilder_h_
#define __MetaShapeBuilder_h_

#include "TowerBuilder.h"
#include "Tower.h"

class MetaShapeBuilder
{
public:

	MetaShapeBuilder(boost::random::mt19937 &gen, Tower *tower);
	virtual ~MetaShapeBuilder();

	virtual void getMetaShape(int type, BuilderChunk *chunk, MetaShape *metaShape);

	virtual void makePlatform(BuilderChunk *chunk, MetaShape *metaShape);
	virtual void makeStaircase(MetaShape *metaShape);

	boost::random::mt19937 gen;
	Tower *tower;
};

#endif // #ifndef __MetaShapeBuilder_h_