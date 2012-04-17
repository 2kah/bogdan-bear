#include "TowerGraphics.h"

#include <iostream>

#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include "Tower.h"

#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreStaticGeometry.h"

#include "OgreManualObject.h"

const unsigned TowerGraphics::CHUNK_HEIGHT = 16;

GraphicsChunk::GraphicsChunk(Tower *tower, BoundingVolume bounds, Ogre::SceneManager *sceneManager)
    : TowerChunk(tower, bounds),
    sceneManager(sceneManager)
{
    this->object = this->sceneManager->createManualObject();
    this->object->setDynamic(true);

    this->object->begin("BOGDANTower", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    this->object->position(0, 0, 0);
    this->object->normal(Ogre::Vector3::UNIT_SCALE);
    this->object->colour(Ogre::ColourValue::White);
    this->object->triangle(0, 0, 0);
    this->object->end();

    this->sceneManager->getRootSceneNode()->attachObject(this->object);

    this->rebuild();
}

GraphicsChunk::~GraphicsChunk()
{
}

void GraphicsChunk::rebuild()
{
    std::vector<BlockTriangle> triangles;

    this->tower->getChunkTriangles(triangles, *this);

    this->object->beginUpdate(0);

    unsigned index = 0;

    for(std::vector<BlockTriangle>::iterator i = triangles.begin(); i != triangles.end(); ++i)
    {
        BlockTriangle triangle = *i;
        
        for (unsigned point = 0; point < 3; ++point)
        {
            this->object->position(triangle.points[point]);
            this->object->normal(triangle.normals[point]);
            this->object->colour(triangle.colours[point]);
        }

        this->object->triangle(index, index + 1, index + 2);

        index += 3;
    }

    this->object->end();
}

// Create a graphical representation of a Tower attached to a given SceneManager.
TowerGraphics::TowerGraphics(Tower *tower, Ogre::SceneManager *sceneManager)
{
    this->tower = tower;
    this->sceneManager = sceneManager;
    
    // Listen for changes to the tower that may need to be reflected in the graphics
    this->tower->signals.updated.connect(boost::bind(&TowerGraphics::towerUpdated, this, _1, _2));

    for (unsigned level = 0; level < this->tower->levels / CHUNK_HEIGHT; ++level)
    {
        BoundingVolume bounds(level * CHUNK_HEIGHT, (level + 1) * CHUNK_HEIGHT,
                              0,                    this->tower->layers,
                              0,                    this->tower->sectors);

        this->chunks.push_back(new GraphicsChunk(tower, bounds, this->sceneManager));
    }
}

TowerGraphics::~TowerGraphics()
{
}

void TowerGraphics::towerUpdated(Tower *tower, BoundingVolume bounds)
{
    for (std::vector<GraphicsChunk *>::iterator it = this->chunks.begin(); it != this->chunks.end(); ++it)
    {
        GraphicsChunk *chunk = *it;
        
        if (chunk->bounds.collides(bounds)) {
            chunk->rebuild();
        }
    }
}
