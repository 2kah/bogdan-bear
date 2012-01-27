#include "TowerPhysics.h"

#include <vector>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>

#include "BtOgreExtras.h"

#include "Tower.h"

TowerPhysics::TowerPhysics(Tower *tower, btDiscreteDynamicsWorld* dynamicsWorld)
{
    this->tower = tower;

    ///*
    btTriangleMesh *data = new btTriangleMesh();

    std::vector<BlockTriangle> triangles;

    for (unsigned level = 0; level < this->tower->levels; ++level)
    {
        for (unsigned layer = 0; layer < this->tower->layers; ++layer)
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

    btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(data, false, true);
    
    btDefaultMotionState* blockMotionState = new btDefaultMotionState(btTransform(btMatrix3x3::getIdentity(), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo blockRigidBodyCI(0, blockMotionState, shape, btVector3(0, 0, 0));           
    btRigidBody *blockRigidBody = new btRigidBody(blockRigidBodyCI);
    blockRigidBody->setUserPointer(this->tower);
    blockRigidBody->setActivationState(ISLAND_SLEEPING);
    dynamicsWorld->addRigidBody(blockRigidBody);

    return;
    //*/

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
    }
}

TowerPhysics::~TowerPhysics()
{
}

void TowerPhysics::towerUpdated(Tower *tower, BoundingVolume bounds)
{

}
