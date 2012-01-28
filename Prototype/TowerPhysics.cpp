#include "TowerPhysics.h"

#include <vector>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>

#include "BtOgreExtras.h"

#include "Tower.h"

PhysicsChunk::PhysicsChunk(Tower *tower, BoundingVolume bounds, btDiscreteDynamicsWorld *dynamicsWorld)
    : TowerChunk(tower, bounds),
    dynamicsWorld(dynamicsWorld)
{
    this->body = NULL;

    this->rebuild();
}

PhysicsChunk::~PhysicsChunk()
{
}

void PhysicsChunk::rebuild()
{
    if (this->body != NULL) {
        this->dynamicsWorld->removeRigidBody(this->body);
        delete this->body;
    }

    btTriangleMesh *data = new btTriangleMesh();

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

    for(std::vector<BlockTriangle>::iterator i = triangles.begin(); i != triangles.end(); ++i)
    {
        BlockTriangle triangle = *i;

        data->addTriangle(BtOgre::Convert::toBullet(triangle.points[0]),
                          BtOgre::Convert::toBullet(triangle.points[1]),
                          BtOgre::Convert::toBullet(triangle.points[2]));
    }

    btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(data, true, true);
    
    btDefaultMotionState* blockMotionState = new btDefaultMotionState(btTransform(btMatrix3x3::getIdentity(), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo blockRigidBodyCI(0, blockMotionState, shape, btVector3(0, 0, 0));           

    this->body = new btRigidBody(blockRigidBodyCI);
    this->body->setUserPointer(this->tower);
    this->body->setActivationState(ISLAND_SLEEPING);

    dynamicsWorld->addRigidBody(this->body);
}

TowerPhysics::TowerPhysics(Tower *tower, btDiscreteDynamicsWorld *dynamicsWorld)
{
    this->tower = tower;
    this->dynamicsWorld = dynamicsWorld;

    tower->signals.updated.connect(boost::bind(&TowerPhysics::towerUpdated, this, _1, _2));
    
    for (unsigned level = 0; level < this->tower->levels / 4; ++level)
    {
        this->chunks.push_back(new PhysicsChunk(tower, BoundingVolume(level * 4, (level + 1) * 4, 0, this->tower->layers, 0, 0), dynamicsWorld));
    }

    //*/

    /*
    for (unsigned level = 0; level < this->tower->levels; ++level)
    {
        for (unsigned layer = 0; layer < this->tower->layers; ++layer)
        {
            for (unsigned sector = 0; sector < this->tower->blocks[level][layer].size(); ++sector)
            {
                if (this->tower->blocks[level][layer][sector] == 1)
                {
                    BlockPoints points = this->tower->getBlockPoints(level, layer, sector);

                    btConvexHullShape *blockShape = new btConvexHullShape();

                    // ugly
                    blockShape->addPoint(BtOgre::Convert::toBullet(points.a1));
                    blockShape->addPoint(BtOgre::Convert::toBullet(points.b1));
                    blockShape->addPoint(BtOgre::Convert::toBullet(points.c1));
                    blockShape->addPoint(BtOgre::Convert::toBullet(points.d1));
                    blockShape->addPoint(BtOgre::Convert::toBullet(points.a2));
                    blockShape->addPoint(BtOgre::Convert::toBullet(points.b2));
                    blockShape->addPoint(BtOgre::Convert::toBullet(points.c2));
                    blockShape->addPoint(BtOgre::Convert::toBullet(points.d2));

                    btDefaultMotionState* blockMotionState = new btDefaultMotionState(btTransform(btMatrix3x3::getIdentity(), btVector3(0, 0, 0)));

                    btRigidBody::btRigidBodyConstructionInfo blockRigidBodyCI(0, blockMotionState, blockShape, btVector3(0, 0, 0));
                    
                    btRigidBody *blockRigidBody = new btRigidBody(blockRigidBodyCI);

                    blockRigidBody->setUserPointer(this->tower);

                    blockRigidBody->setActivationState(ISLAND_SLEEPING); // no benefit?

                    dynamicsWorld->addRigidBody(blockRigidBody); // , 1, 2);
                }
            }
        }
    }*/
}

TowerPhysics::~TowerPhysics()
{
}

void TowerPhysics::towerUpdated(Tower *tower, BoundingVolume bounds)
{
    for (std::vector<PhysicsChunk *>::iterator it = this->chunks.begin(); it != this->chunks.end(); ++it)
    {
        PhysicsChunk *chunk = *it;
        
        if (chunk->bounds.collides(bounds)) {
            chunk->rebuild();
        }
    }
}
