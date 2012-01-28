#include "TowerGraphics.h"

#include <iostream>

#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include "Tower.h"

#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreStaticGeometry.h"

#include "OgreManualObject.h"

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

    for (unsigned level = this->bounds.level_bottom; level < this->bounds.level_top; ++level)
    {
        for (unsigned layer = this->bounds.layer_inner; layer < this->bounds.layer_outer; ++layer)
        {
            for (unsigned sector = 0; sector < this->tower->blocks[level][layer].size(); ++sector)
            {
                if (this->tower->blocks[level][layer][sector])
                {
                    this->tower->getBlockTriangles(triangles, level, layer, sector);
                }
            }
        }
    }

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
    //this->tower->signals.levelUpdated.connect(boost::bind(&TowerGraphics::blocksUpdated, this, _1, _2));
    this->tower->signals.updated.connect(boost::bind(&TowerGraphics::towerUpdated, this, _1, _2));

    for (unsigned level = 0; level < this->tower->levels / 4; ++level)
    {
        this->chunks.push_back(new GraphicsChunk(tower, BoundingVolume(level * 4, (level + 1) * 4, 0, this->tower->layers, 0, 0), this->sceneManager));
    }

    /*
    // TODO: temporary
    this->wholething = this->sceneManager->createManualObject("manualTower");
    this->wholething->setDynamic(true);
    this->wholething->estimateVertexCount(8 * this->tower->sectors * this->tower->layers * this->tower->levels);
    this->wholething->estimateIndexCount(6 * 8 * this->tower->sectors * this->tower->layers * this->tower->levels);

    for (unsigned level = 0; level < this->tower->levels / 4; ++level)
    {
        this->wholething->begin("BOGDANTower", Ogre::RenderOperation::OT_TRIANGLE_LIST);
        this->wholething->position(0, 0, 0);
        this->wholething->normal(Ogre::Vector3::UNIT_SCALE);
        this->wholething->colour(Ogre::ColourValue::White);
        this->wholething->triangle(0, 0, 0);
        this->wholething->end();

        std::cout << "SECTIONS: " << this->wholething->getNumSections() << std::endl;
    }

    for (unsigned level = 0; level < this->tower->levels / 4; ++level)
    {
        this->rebuildTowerObject(level * 4, false);
    }

    this->sceneManager->getRootSceneNode()->attachObject(this->wholething);
    */

    Ogre::Entity *bowl = sceneManager->createEntity("Bowl.mesh");
    Ogre::SceneNode *sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    sceneNode->attachObject(bowl);
    sceneNode->setScale(30*Ogre::Vector3::UNIT_SCALE);
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

// Update the graphics of the tower if blocks have changed
void TowerGraphics::blocksUpdated(Tower *tower, unsigned level)
{
    //this->rebuildStaticGeometry();
    this->rebuildTowerObject(level, false);
}

// TODO: temporary
void TowerGraphics::rebuildTowerObject(unsigned level, bool first)
{
    Ogre::ManualObject *block = this->wholething;
    unsigned index = 0;
    
    unsigned section = level / 4;
    unsigned baselevel = section * 4;

    if (first) 
    {
    }
    else 
    {
        this->wholething->beginUpdate(section);
    }

    std::vector<BlockTriangle> triangles;

    for (unsigned l = 0; l < 4; ++l)
    {
        for (unsigned layer = 0; layer < this->tower->layers; ++layer)
        {
            for (unsigned sector = 0; sector < this->tower->blocks[baselevel + l][layer].size(); ++sector)
            {
                if (this->tower->blocks[baselevel + l][layer][sector])
                {
                    this->tower->getBlockTriangles(triangles, baselevel + l, layer, sector);
                }
            }
        }
    }

    for(std::vector<BlockTriangle>::iterator i = triangles.begin(); i != triangles.end(); ++i)
    {
        BlockTriangle triangle = *i;

        for (unsigned point = 0; point < 3; ++point)
        {
            this->wholething->position(triangle.points[point]);
            this->wholething->normal(triangle.normals[point]);
            this->wholething->colour(triangle.colours[point]);
        }

        this->wholething->triangle(index, index + 1, index + 2);

        index += 3;
    }

    this->wholething->end();
}
