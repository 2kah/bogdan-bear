#include "TowerPhysics.h"

#include <vector>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "Tower.h"

TowerPhysics::TowerPhysics(Tower *tower, btDiscreteDynamicsWorld* dynamicsWorld)
{
    this->tower = tower;
    
    std::vector<btCollisionShape *> blockShapes;

    for (unsigned radius = 0; radius < this->tower->layers; ++radius)
    {
        {
        BlockPoints points = this->tower->getBlockPoints(0, radius, 0);

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

        blockShapes.push_back(blockShape);
        }

        for (unsigned height = 0; height < 10 /*this->tower->levels*/; ++height)
        {
            for (unsigned position = 0; position < this->tower->sectors; ++position)
            {
                //Generates random segments
                if (this->tower->blocks[height][radius][position] == 1)
                {
                    // Generated block physics:
                    ///*
                    {
                    ///*
                    BlockPoints points = this->tower->getBlockPoints(height, radius, position);

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
                    //*/

                    BlockPosition position_ = this->tower->getBlockPosition(height, radius, position);

                    //btDefaultMotionState* blockMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0, 1, 0), btScalar(-position_.angle)), btVector3(0, height * this->tower->blocksize, 0)));
                    btRigidBody::btRigidBodyConstructionInfo blockRigidBodyCI(0, blockMotionState, blockShape, btVector3(0, 0, 0));

                    //btRigidBody::btRigidBodyConstructionInfo blockRigidBodyCI(0, blockMotionState, blockShapes[radius], btVector3(0, 0, 0));

                    btRigidBody *blockRigidBody = new btRigidBody(blockRigidBodyCI);

                    blockRigidBody->setUserPointer(new BlockReference(this->tower, height, radius, position));

                    blockRigidBody->setActivationState(ISLAND_SLEEPING); // no benefit?

                    dynamicsWorld->addRigidBody(blockRigidBody); // , 1, 2);
                    }
                    //*/
                }
            }
        }
    }
}

TowerPhysics::~TowerPhysics()
{
}
