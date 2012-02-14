#include "TowerPhysics.h"

#include <vector>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>

#include "BtOgreExtras.h"

#include "Tower.h"

PhysicsChunk::PhysicsChunk(Tower *tower, BoundingVolume bounds, btDiscreteDynamicsWorld *dynamicsWorld)
    : TowerChunk(tower, bounds)
    , PhysicsObject(TOWER_CHUNK)
    , dynamicsWorld(dynamicsWorld)
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
        this->body = NULL;
        delete this->mesh;
    }

    this->mesh = new btTriangleMesh();

    std::vector<BlockTriangle> triangles;

    this->tower->getChunkTriangles(triangles, *this);

    if (triangles.size() == 0) {
        return;
    }

    for(std::vector<BlockTriangle>::iterator i = triangles.begin(); i != triangles.end(); ++i)
    {
        BlockTriangle triangle = *i;

        this->mesh->addTriangle(BtOgre::Convert::toBullet(triangle.points[0]),
                          BtOgre::Convert::toBullet(triangle.points[1]),
                          BtOgre::Convert::toBullet(triangle.points[2]));
    }

    btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(this->mesh, true, true);
    
    btDefaultMotionState* blockMotionState = new btDefaultMotionState(btTransform(btMatrix3x3::getIdentity(), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo blockRigidBodyCI(0, blockMotionState, shape, btVector3(0, 0, 0));           

    this->body = new btRigidBody(blockRigidBodyCI);
    this->body->setUserPointer(this);
    this->body->setActivationState(ISLAND_SLEEPING);

    dynamicsWorld->addRigidBody(this->body);
}

TowerPhysics::TowerPhysics(Tower *tower, btDiscreteDynamicsWorld *dynamicsWorld)
{
    this->tower = tower;
    this->dynamicsWorld = dynamicsWorld;

    tower->signals.updated.connect(boost::bind(&TowerPhysics::towerUpdated, this, _1, _2));
    
    for (unsigned level = 0; level < this->tower->levels / 8; ++level)
    {
        for (unsigned layer = 0; layer < this->tower->layers / 11; ++layer)
        {
            for (unsigned sector = 0; sector < this->tower->sectors / 16; ++sector)
            {
                BoundingVolume bounds(level  * 8, (level+1)  * 8,
                                      //0,          this->tower->layers,
                                      layer  * 11, (layer+1)  * 11,
                                      //0,          this->tower->sectors);
                                      sector * 16, (sector+1) * 16);

                this->chunks.push_back(new PhysicsChunk(tower, bounds, dynamicsWorld));
            }
        }
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
    unsigned rebuilt = 0;

    for (std::vector<PhysicsChunk *>::iterator it = this->chunks.begin(); it != this->chunks.end(); ++it)
    {
        PhysicsChunk *chunk = *it;

        if (chunk->bounds.collides(bounds)) {
            chunk->rebuild();
            ++rebuilt;
        }
    }

    //std::cout << "REBUILT: " << rebuilt << " / " << this->chunks.size() << std::endl;
}
