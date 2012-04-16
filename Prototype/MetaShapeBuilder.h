#ifndef __MetaShapeBuilder_h_
#define __MetaShapeBuilder_h_

#include "TowerBuilder.h";

class MetaShapeBuilder
{
public:

	MetaShapeBuilder();
	virtual ~MetaShapeBuilder();

	virtual void getMetaShape(int type, BuilderChunk *chunk, MetaShape *metaShape);

	virtual void makePlatform(BuilderChunk *chunk, MetaShape *metaShape);
};

#endif // #ifndef __MetaShapeBuilder_h_