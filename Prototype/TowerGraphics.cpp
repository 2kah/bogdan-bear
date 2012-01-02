#include "TowerGraphics.h"

#include <iostream>

#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreStaticGeometry.h"

#include "OgreManualObject.h"
#include "OgreMeshSerializer.h"

TowerGraphics::TowerGraphics()
{
}

// Create a graphical representation of a Tower attached to a given SceneManager.
TowerGraphics::TowerGraphics(Tower *tower, Ogre::SceneManager *sceneManager)
{
    this->tower = tower;
    this->sceneManager = sceneManager;
    
    // Listen for changes to the tower that may need to be reflected in the graphics
    this->tower->addTowerListener(this);

    // Generate a block entity for each layer of the tower
    this->createBlockEntities();

    // Create static geometry for the tower and build an initial representation of the tower in it
    this->geometry = this->sceneManager->createStaticGeometry("Tower");
    this->rebuildStaticGeometry();
}

TowerGraphics::~TowerGraphics()
{
    // Delete the static geometry
    delete this->geometry;

    // Delete the generated block entities
    for (std::vector<Ogre::Entity *>::iterator iterator = this->blockEntities.begin(); iterator != this->blockEntities.end(); ++iterator)
    {
        Ogre::Entity *entity = *iterator;

        delete entity;
    }
}

// Update the graphics of the tower if blocks have changed
void TowerGraphics::blocksUpdated(void)
{
    this->rebuildStaticGeometry();
}

// Create a block entity for each different block shape (one for each layer of the tower)
void TowerGraphics::createBlockEntities(void)
{
    for (unsigned layer = 0; layer < this->tower->layers; ++layer)
    {
        BlockPoints points = this->tower->getBlockPoints(0, layer, 0);

        Ogre::String name = "manual " + Ogre::StringConverter::toString(layer);

        Ogre::ManualObject *block = sceneManager->createManualObject(name);

        block->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

        Ogre::ColourValue orange = Ogre::ColourValue(1, 0, 0, 0.5);

        block->position(points.a1.x, points.a1.y, points.a1.z); // a1 = 0
        block->colour(orange);
        block->position(points.b1.x, points.b1.y, points.b1.z); // b1 = 1
        block->colour(Ogre::ColourValue::Green);
        block->position(points.c1.x, points.c1.y, points.c1.z); // c1 = 2
        block->colour(Ogre::ColourValue::Blue);
        block->position(points.d1.x, points.d1.y, points.d1.z); // d1 = 3
        block->colour(Ogre::ColourValue::White);
        block->position(points.a2.x, points.a2.y, points.a2.z); // a2 = 4
        block->colour(Ogre::ColourValue::White);
        block->position(points.b2.x, points.b2.y, points.b2.z); // b2 = 5
        block->colour(Ogre::ColourValue::Blue);
        block->position(points.c2.x, points.c2.y, points.c2.z); // c2 = 6
        block->colour(Ogre::ColourValue::Green);
        block->position(points.d2.x, points.d2.y, points.d2.z); // d2 = 7
        block->colour(orange);

        // Top
        // d2 -> b2 -> c2 <--
        block->index(6);
        block->index(5);
        block->index(7);

        // d2 -> c2 -> a2 <--
        block->index(4);
        block->index(6);
        block->index(7);

        // Bottom
        //
        block->index(3);
        block->index(1);
        block->index(2);

        //
        block->index(3);
        block->index(2);
        block->index(0);
                    
        // Outer
        // c1 <- a1 <- a2
        block->index(4);
        block->index(0);
        block->index(2);

        // c1 -> c2 -> a2
        block->index(2);
        block->index(6);
        block->index(4);
                    
        // Clockwise
        // b1 -> c1 -> c2 <--
        block->index(6);
        block->index(2);
        block->index(1);

        // b1 -> c2 -> b2 <--
        block->index(5);
        block->index(6);
        block->index(1);

        // Anti
        // a1 -> d1 -> d2 <--
        block->index(7);
        block->index(3);
        block->index(0);

        // a1 -> d2 -> a2 <--
        block->index(4);
        block->index(7);
        block->index(0);

        // Inner
        // d1 -> b1 -> b2 <--
        block->index(5);
        block->index(1);
        block->index(3);

        // d1 -> b2 -> d2 <--
        block->index(7);
        block->index(5);
        block->index(3);
                    
        block->end();

        Ogre::MeshPtr pointer = block->convertToMesh(name);
        Ogre::MeshSerializer serializer;

        serializer.exportMesh(pointer.getPointer(), name + ".mesh");

        Ogre::Entity *blockEntity = sceneManager->createEntity(name + " ent", name);

        this->blockEntities.push_back(blockEntity);
    }
}

// Empty the static geometry and then refill it with a current representation of the tower
void TowerGraphics::rebuildStaticGeometry(void)
{
    std::cout << "Rebuilding static geometry." << std::endl;
    
    this->geometry->reset();

    for (unsigned level = 0; level < this->tower->levels; ++level)
    {
        for (unsigned layer = 0; layer < this->tower->layers; ++layer)
        {
            for (unsigned sector = 0; sector < this->tower->sectors; ++sector)
            {
                if (this->tower->blocks[level][layer][sector])
                {
                    Ogre::Radian angle(this->tower->getBlockPosition(level, layer, sector).angle);
                    
                    // Block entities are generated with the correct distance from the centre and at the right scale, but must be rotated and brought to the right level
                    Ogre::Vector3 position(0, this->tower->blocksize * level, 0);
                    Ogre::Quaternion rotation(angle, Ogre::Vector3::UNIT_Y);

                    // Add the entity to the static geometry at 
                    this->geometry->addEntity(this->blockEntities[layer], position, rotation);
                }
            }
        }
    }

	//Build the static geometry
    this->geometry->build();
}
