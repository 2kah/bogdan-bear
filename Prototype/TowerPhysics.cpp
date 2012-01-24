#include "TowerPhysics.h"

#include <vector>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "Tower.h"

TowerPhysics::TowerPhysics(Tower *tower, btDiscreteDynamicsWorld* dynamicsWorld)
{
    this->tower = tower;
    
    for (unsigned level = 0; level < 10 /*this->tower->levels*/; ++level)
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
                    blockShape->addPoint(btVector3(points.a1.x, points.a1.y, points.a1.z));
                    blockShape->addPoint(btVector3(points.b1.x, points.b1.y, points.b1.z));
                    blockShape->addPoint(btVector3(points.c1.x, points.c1.y, points.c1.z));
                    blockShape->addPoint(btVector3(points.d1.x, points.d1.y, points.d1.z));
                    blockShape->addPoint(btVector3(points.a2.x, points.a2.y, points.a2.z));
                    blockShape->addPoint(btVector3(points.b2.x, points.b2.y, points.b2.z));
                    blockShape->addPoint(btVector3(points.c2.x, points.c2.y, points.c2.z));
                    blockShape->addPoint(btVector3(points.d2.x, points.d2.y, points.d2.z));

                    btDefaultMotionState* blockMotionState = new btDefaultMotionState(btTransform(btMatrix3x3::getIdentity(), btVector3(0, 0, 0)));

                    btRigidBody::btRigidBodyConstructionInfo blockRigidBodyCI(0, blockMotionState, blockShape, btVector3(0, 0, 0));
                    
                    btRigidBody *blockRigidBody = new btRigidBody(blockRigidBodyCI);

                    blockRigidBody->setUserPointer(new BlockReference(this->tower, level, layer, sector));

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
