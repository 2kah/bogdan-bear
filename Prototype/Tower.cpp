#include <iostream>

#include <OgreSceneManager.h>
#include <OgreStaticGeometry.h>
#include <OgreEntity.h>
#include <math.h>
#include <btBulletDynamicsCommon.h>

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <btBulletWorldImporter.h>

#include "Tower.h"
//#include "DebugDraw.h"

#include <OgreManualObject.h>
#include <OgreMeshSerializer.h>

#define PI 3.14159265

///*
TowerOld::TowerOld()
{

}


TowerOld::TowerOld(Ogre::SceneManager *mSceneMgr, btDiscreteDynamicsWorld* dynamicsWorld)
{
    int h = 50;
	int r = 5;
	int p = 96;
	double scale = 15;
	Ogre::Entity* entity [8];
	Ogre::SceneNode* node [50][5][96];
	btRigidBody* blockRigidBody;
	btCollisionObject* blockObject;
	int radius = 5;
	int height = 50;
	int position = 84;
	int randSeg [50][5][96];
	int visible [50][5][96] = {0};

	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

	//Creates the static geometry to add geometry to
	Ogre::StaticGeometry* sg = mSceneMgr->createStaticGeometry("Tower");
	btBulletWorldImporter* fileLoader = new btBulletWorldImporter(dynamicsWorld);

	//Loop through 3D array, filling with random 0's and 1's
    for (height = 0; height < h; ++height)
    {
        for (radius = 0; radius < r; ++radius)
        {
            for (position = 0; position < p; ++position)
            {
                //Worth looking at different seeds for this
                int x = rand() % h; 

                //Makes the tower more sparse at the top - Larger the height, less likely the blocks
                if (x < (height - 2)) x = 0;
                else x = 1;

                //Makes the tower taper at the top - Bigger radiuses at top are cut off
                if ((floor((double)(h-height)/radius)) <= radius) x = 0;
                //if (height == (h-1)) x = 1;
                randSeg [height][radius][position] = x;

				
            }
        }
		printf("Done floor %i\n", height);
    }
	printf("Done random gen.\n");

	
	//Remove ones hidden vertically
    for (radius = 0; radius < r; ++radius)
    {
        for (position = 0; position < p; ++position)
        {
			visible[h-1][radius][position]= randSeg [h-1][radius][position];
			visible[0][radius][position]= randSeg [0][radius][position];
        }
    }
	for (position = 0; position < p; ++position)
    {
        for (radius = 0; radius < r; ++radius)
        {
            for (height = 1; height < h-2; ++height)
            {
				int top = randSeg [height+1][radius][position];
				int current = randSeg [height][radius][position];
				int bottom = randSeg [height-1][radius][position];
				int check = visible[height][radius][position];

				if(current == 0) visible[height][radius][position] = 0;
				else if(top == 1 && bottom == 1 && check == 0) visible[height][radius][position] = 0;
				else visible [height][radius][position] = 1;
            }
        }
    }

	//Remove ones hidden from outside to middle
    for (height = 0; height < h; ++height)
    {
        for (position = 0; position < p; ++position)
        {
			visible[height][r-2][position]= randSeg [height][r-2][position];
			visible[height][0][position]= randSeg [height][0][position];
        }
    }
	for (height = 0; height < h; ++height)
    {
        for (position = 0; position < p; ++position)
        {
            for (radius = 1; radius < r-1; ++radius)
            {
				int front = randSeg [height][radius + 1][(position*2)+1];
				int front1 = randSeg [height][radius + 1][(position*2)];
				int current = randSeg [height][radius][position];
				int behind = randSeg [height][radius - 1][(int)floor((double)position/2)];
				int check = visible[height][radius][position];

				if(current == 0) visible[height][radius][position] = 0;
				else if(front == 1 && front1 == 1 && behind == 1 && check == 0) visible[height][radius][position] = 0;
				else visible [height][radius][position] = 1;
            }
        }
    }

	//Remove ones hidden from the sides
	for (height = 0; height < h; ++height)
    {
        for (radius = 0; radius < r; ++radius)
        {
			int nseg = ((int)pow((double)2,(radius))*12);
            for (position = 0; position < p; ++position)
            {
				int left = randSeg [height][radius][(position + 1) % nseg];
				int current = randSeg [height][radius][position];
				int right = randSeg [height][radius][(position - 1) % nseg];
				int check = visible[height][radius][position];

				if(current == 0) visible[height][radius][position] = 0;
				else if(left == 1 && right == 1 && check == 0) visible[height][radius][position] = 0;
				else visible [height][radius][position] = 1;
            }
        }
    }

    //Loop through the radius values first
    for (radius = 1; radius < r; radius++)
    {
        //nseg is the number of blocks in that radius value
        int nseg = ((int)pow((double)2,(radius-1))*12);

        //Convert stuff to string to use in file name
        Ogre::String meshnum = Ogre::StringConverter::toString(radius);
        Ogre::String ra = Ogre::StringConverter::toString(radius);
        entity[radius] = mSceneMgr->createEntity("Head" + ra,"test" + meshnum + ".mesh");

		//Loads bullet meshes for each sector shape
		if(radius == 1)
		{
			fileLoader->loadFile("onebt.bullet");
			printf("onebb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("oneb.bullet");

		}
		if(radius == 2) 
		{
			fileLoader->loadFile("twobt.bullet");
			printf("twobb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("twob.bullet");
		}
		if(radius == 3) 
		{
			fileLoader->loadFile("threebt.bullet");
			printf("threebb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("threeb.bullet");
		}
		if(radius == 4)
		{
			fileLoader->loadFile("fourbt.bullet");
			printf("fourbb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("fourb.bullet");
		}
		if(radius == 5) 
		{
			fileLoader->loadFile("fivebt.bullet");
			printf("fiveb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("fiveb.bullet");
		}
		if(radius == 6) 
		{
			fileLoader->loadFile("sixbt.bullet");
			printf("sixb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("sixb.bullet");
		}
		if(radius == 7) 
		{
			fileLoader->loadFile("sevenbt.bullet");
			printf("sevenb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("sevenb.bullet");
			
		}

		//blockShape = new btBoxShape(btVector3(scale,scale,scale));
		//blockShape = new btCylinderShape(btVector3(r+1,(scale*height*0.5)+(0.5*scale),r+1));
		//printf("This many objects %i\n", (dynamicsWorld->getNumCollisionObjects()));

        //Loop through the height
        for (height = 0; height < h; height++)
        {
            for (position = 0; position < nseg; position++)
            {
                //Generates random segments
                if (visible [height][radius-1][position] == 1)
                {
                    //rads is the radian value for each block around the centre point
                    double rads = ((1/nseg)+((double)position/(nseg/2)))*PI;
                    //degs is the degree for how much the block is rotated
                    double degs = 360/(double)nseg;
					double xPos = ((scale*radius)+(scale*(0.5)))*cos(rads);
					double yPos = ((scale*((double)height/2)));
					double zPos = ((scale*radius)+(scale*(0.5)))*sin(rads);
                    //Postion k is the height, the others are the postion relative to the middle of the tower
                    Ogre::Vector3 pos(Ogre::Vector3(xPos, yPos, zPos));
                    //How much the block is rotated depending on its position
                    Ogre::Quaternion rot(Ogre::Degree(-(degs/2)-(position*degs)), Ogre::Vector3::UNIT_Y);
                    //Scale is 1
                    Ogre::Vector3 scaleVector(scale, scale, scale);

                    //Add the entity to the static geometry
                    sg->addEntity(entity[radius], pos, rot, scaleVector);

					//Adds bullet mesh to object
			        blockObject = dynamicsWorld->getCollisionObjectArray()[(dynamicsWorld->getNumCollisionObjects())-1];//
					blockShape = blockObject->getCollisionShape();
					//Scale bullet mesh to appropriate size
					//THIS MAKES IT -LOOK- RIGHT, DON'T KNOW IF IT IS RIGHT, PLEASE EXPLORE FURTHER
					blockShape->setLocalScaling(btVector3(1,1,1));
								
					btQuaternion qRot(0,0,0,1);
					qRot.setRotation(btVector3(0,1,0), ((-PI/(9.5*radius))-rads));
					btDefaultMotionState* blockMotionState = new btDefaultMotionState(btTransform(qRot,btVector3(xPos,yPos,zPos)));
					btRigidBody::btRigidBodyConstructionInfo blockRigidBodyCI(0,blockMotionState,blockShape,btVector3(0,0,0));
				    blockRigidBody= new btRigidBody(blockRigidBodyCI);
					blockRigidBody->setCenterOfMassTransform(btTransform(qRot,btVector3(xPos,yPos,zPos)));

					//dynamicsWorld->addRigidBody(blockRigidBody);

                }
            }
        }
    }

	btQuaternion qRot(0,0,0,1);
	btDefaultMotionState* blockMotionState = new btDefaultMotionState(btTransform(qRot,btVector3(0,0,0)));
	btRigidBody::btRigidBodyConstructionInfo blockRigidBodyCI(0,blockMotionState,blockShape,btVector3(0,0,0));
	blockRigidBody= new btRigidBody(blockRigidBodyCI);
	
	dynamicsWorld->addRigidBody(blockRigidBody);

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
                groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);
	//DebugDraw->DebugDraw(mSceneMgr, dynamicsWorld);
 
	//Display the static geometry
    sg->build();

    // Set ambient light
    // mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);
	
	//Ogre::Entity* ogreHead = sceneMgr->createEntity(name, "ogrehead.mesh");

    //mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    //mSceneNode->attachObject(ogreHead);

    //mSceneNode->translate(position);
}

TowerOld::~TowerOld(void)
{

}

void TowerOld::update(void)
{
}
//*/

// TOWER REFACTOR

Tower::Tower(): blocksize(0), levels(0), layers(0), sectors(0), blocks(0) {}

Tower::Tower(double blocksize, unsigned levels, unsigned layers, unsigned sectors)
{
    this->blocksize = blocksize;
    this->levels = levels;
    this->layers = layers;
    this->sectors = sectors;

    this->blocks = std::vector<std::vector<std::vector<bool> > >(levels, std::vector<std::vector<bool> >(layers, std::vector<bool>(sectors, false)));
}

Tower::~Tower(void)
{
}

void Tower::update(void)
{
}

void Tower::carveSphere(Ogre::Vector3 position, double radius)
{
    std::cout << "(Tower) Removing blocks in a radius of " << radius << " from " << position << " (Not implemented)" << std::endl;
}

void Tower::rebuild()
{
}

void Tower::synchronise()
{
}

BlockPosition Tower::getBlockPosition(unsigned level, unsigned layer, unsigned sector)
{
    BlockPosition position;
    
    double nseg = this->sectors; //layer * 12;

    double angle = ((2*PI) / nseg) * (sector + 0.5);
    double radius = this->blocksize * (layer + 0.5);

    position.angle = angle;
    position.x = radius * cos(angle);
    position.y = (this->blocksize / 2) * (level + 0.5); // is vertical scale half?
    position.z = radius * sin(angle);

    return position;
}

// These are not the points of the non-generated models
// Points as shown in http://you.mongle.me/tower/circles/gamesproject.png
// base: a1, b1, c1, d1
//  top: a2, b2, c2, d2
BlockPoints Tower::getBlockPoints(unsigned level, unsigned layer, unsigned sector)
{
    double blockheight = this->blocksize;
    double size = this->blocksize;
    double radius = size * layer;

    double angle = ((2*PI) / this->sectors);
    double offset = angle * sector;

    double bottom = blockheight * level;
    double top = blockheight * (level + 1);

    Point a1 = {(radius + size) * cos(offset + angle), bottom, (radius + size) * sin(offset + angle)};
    Point a2 = {(radius + size) * cos(offset + angle),    top, (radius + size) * sin(offset + angle)};

    Point d1 = {(radius) * cos(offset + angle), bottom, (radius) * sin(offset + angle)};
    Point d2 = {(radius) * cos(offset + angle),    top, (radius) * sin(offset + angle)};

    Point b1 = {(radius) * cos(offset), bottom, (radius) * sin(offset)};
    Point b2 = {(radius) * cos(offset),    top, (radius) * sin(offset)};

    Point c1 = {(radius + size) * cos(offset), bottom, (radius + size) * sin(offset)};
    Point c2 = {(radius + size) * cos(offset),    top, (radius + size) * sin(offset)};

    BlockPoints points = {a1, b1, c1, d1,
                          a2, b2, c2, d2};

    return points;
}

void Tower::addTowerListener(TowerListener *listener)
{
    //this->towerListeners.insert(listener);

    this->blocksUpdated.connect(boost::bind(&TowerListener::blocksUpdated, listener, _1));
}

void Tower::fireBlocksUpdated(unsigned level)
{
    /*
    for(std::set<TowerListener *>::iterator i = this->towerListeners.begin(); i != this->towerListeners.end(); ++i)
    {
       TowerListener *listener = *i;

       listener->blocksUpdated(level);
    }
    */

    this->blocksUpdated(level);
}

// Tower Physics
TowerPhysics::TowerPhysics()
{
}

TowerPhysics::TowerPhysics(Tower *tower, btDiscreteDynamicsWorld* dynamicsWorld)
{
    this->tower = tower;
    
	btRigidBody* blockRigidBody;// [50][8][84];
	btCollisionObject* blockObject;

    btBulletWorldImporter* fileLoader = new btBulletWorldImporter(dynamicsWorld);

    btCollisionShape* blockShape;
    
    std::vector<btCollisionShape *> blockShapes;
    //blockShapes.push_back(NULL);

    for (unsigned radius = 0; radius < this->tower->layers; radius++)
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

        for (unsigned height = 0; height < this->tower->levels; height++)
        {
            for (unsigned position = 0; position < this->tower->sectors; position++)
            {
                //Generates random segments
                if (this->tower->blocks[height][radius][position] == 1)
                {
                    /*
                    BlockPosition position_ = this->tower->getBlockPosition(height, radius, position);
                    double degs = 360 / ((double) nseg);

					//Adds bullet mesh to object

                    printf("NumCollisionObjects()-1 = %d\n", (dynamicsWorld->getNumCollisionObjects())-1);

			        blockObject = dynamicsWorld->getCollisionObjectArray()[(dynamicsWorld->getNumCollisionObjects()-1)];//
					blockShape = blockObject->getCollisionShape();

                    printf("%s\n", blockShape->getName());

					//Scale bullet mesh to appropriate size
					//THIS MAKES IT -LOOK- RIGHT, DON'T KNOW IF IT IS RIGHT, PLEASE EXPLORE FURTHER
					blockShape->setLocalScaling(btVector3(1,1,1));
								
					btQuaternion qRot(0,0,0,1);
					qRot.setRotation(btVector3(0,1,0), ((-PI/(9.5*radius))-position_.angle));
					btDefaultMotionState* blockMotionState = new btDefaultMotionState(btTransform(qRot,btVector3(position_.x, position_.y, position_.z)));
					btRigidBody::btRigidBodyConstructionInfo blockRigidBodyCI(0,blockMotionState,blockShape,btVector3(0,0,0));
				    blockRigidBody= new btRigidBody(blockRigidBodyCI);
					blockRigidBody->setCenterOfMassTransform(btTransform(qRot,btVector3(position_.x, position_.y, position_.z)));

                    dynamicsWorld->addRigidBody(blockRigidBody, Tower::COLLISION_GROUP, Tower::COLLISION_MASK);
                    */

                    // Generated block physics:
                    ///*
                    {
                    /*
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
                    */

                    BlockPosition position_ = this->tower->getBlockPosition(height, radius, position);

                    btDefaultMotionState* blockMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(0, 1, 0), btScalar(-position_.angle))));
                    
                    btRigidBody::btRigidBodyConstructionInfo blockRigidBodyCI(0, blockMotionState, blockShapes[radius], btVector3(0, 0, 0));
                    btRigidBody *blockRigidBody = new btRigidBody(blockRigidBodyCI);

                    blockRigidBody->setActivationState(ISLAND_SLEEPING); // no benefit?

                    dynamicsWorld->addRigidBody(blockRigidBody, 1, 2);
                    }
                    //*/
                }
            }
        }
    }

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
                groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody, 4, 2);
	//DebugDraw->DebugDraw(mSceneMgr, dynamicsWorld);
}

TowerPhysics::~TowerPhysics()
{
}
