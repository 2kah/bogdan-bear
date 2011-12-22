#include <iostream>

#include <OgreSceneManager.h>
#include <OgreStaticGeometry.h>
#include <OgreEntity.h>
#include <math.h>
#include <btBulletDynamicsCommon.h>


#include "bullet/src/BulletWorldImporter/btBulletWorldImporter.h"

#include "Tower.h"
//#include "DebugDraw.h"

#define PI 3.14159265

/*
Tower::Tower()
{

}


Tower::Tower(Ogre::SceneManager *mSceneMgr, btDiscreteDynamicsWorld* dynamicsWorld)
{
    int h = 50;
	int r = 7;
	int p = 84;
	double scale = 15;
	Ogre::Entity* entity [8];
	Ogre::SceneNode* node [50][8][84];
	btRigidBody* blockRigidBody;// [50][8][84];
	btCollisionObject* blockObject;
	int radius = 8;
	int height = 50;
	int position = 84;
	int randSeg [50][8][84];

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
    //Loop through the radius values first
    for (radius = 1; radius < r; radius++)
    {
        //nseg is the number of blocks in that radius value
        int nseg = radius*12;

        //Convert stuff to string to use in file name
        Ogre::String meshnum = Ogre::StringConverter::toString(radius);
        Ogre::String ra = Ogre::StringConverter::toString(radius);
        entity[radius] = mSceneMgr->createEntity("Head" + ra, meshnum + ".mesh");

		//Loads bullet meshes for each sector shape
		if(radius == 1)
		{
			fileLoader->loadFile("onebbb.bullet");
			printf("onebb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("oneb.bullet");

		}
		if(radius == 2) 
		{
			fileLoader->loadFile("twobbb.bullet");
			printf("twobb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("twob.bullet");
		}
		if(radius == 3) 
		{
			fileLoader->loadFile("threebbb.bullet");
			printf("threebb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("threeb.bullet");
		}
		if(radius == 4)
		{
			fileLoader->loadFile("fourbbb.bullet");
			printf("fourbb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("fourb.bullet");
		}
		if(radius == 5) 
		{
			fileLoader->loadFile("fivebbb.bullet");
			printf("fiveb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("fiveb.bullet");
		}
		if(radius == 6) 
		{
			fileLoader->loadFile("sixbbb.bullet");
			printf("sixb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("sixb.bullet");
		}
		if(radius == 7) 
		{
			fileLoader->loadFile("sevenbbb.bullet");
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
                if (randSeg [height][radius][position] == 1)
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
                    Ogre::Vector3 scale(scale, scale, scale);

                    //Add the entity to the static geometry
                    sg->addEntity(entity[radius], pos, rot, scale);

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

					dynamicsWorld->addRigidBody(blockRigidBody);

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

Tower::~Tower(void)
{

}

void Tower::update(void)
{
}
*/

// TOWER REFACTOR

TowerRefactor::TowerRefactor(): blocksize(0), levels(0), layers(0), sectors(0), blocks(0) {}

TowerRefactor::TowerRefactor(double blocksize, unsigned levels, unsigned layers, unsigned sectors)
{
    this->blocksize = blocksize;
    this->levels = levels;
    this->layers = layers;
    this->sectors = sectors;

    this->blocks = std::vector<std::vector<std::vector<bool> > >(levels, std::vector<std::vector<bool> >(layers, std::vector<bool>(sectors, false)));

    // Random generation

    //Loop through 3D array, filling with random 0's and 1's
    for (unsigned height = 0; height < levels; ++height)
    {
        for (unsigned radius = 0; radius < layers; ++radius)
        {
            for (unsigned position = 0; position < sectors; ++position)
            {
                //Worth looking at different seeds for this
                unsigned x = rand() % levels; 

                //Makes the tower more sparse at the top - Larger the height, less likely the blocks
                if (x < (height - 2)) x = 0;
                else x = 1;

                //Makes the tower taper at the top - Bigger radiuses at top are cut off
                if ((floor((double)(levels-height)/radius)) <= radius) x = 0;
                //if (height == (h-1)) x = 1;
                this->blocks[height][radius][position] = x;
            }
        }
		printf("Done floor %i\n", height);
    }
	printf("Done random gen.\n");
}

TowerRefactor::~TowerRefactor(void)
{
}

void TowerRefactor::update(void)
{
}

void TowerRefactor::carveSphere()
{
}

void TowerRefactor::rebuild()
{
}

void TowerRefactor::synchronise()
{
}

void TowerRefactor::addTowerListener(TowerListener *listener)
{
    this->towerListeners.insert(listener);
}

TowerListener::~TowerListener()
{
}

// TowerGraphics

TowerGraphics::TowerGraphics()
{
}

TowerGraphics::TowerGraphics(TowerRefactor *tower, Ogre::SceneManager *sceneManager)
{
    this->tower = tower;

    Ogre::Entity *entity[8];

    Ogre::StaticGeometry* sg = sceneManager->createStaticGeometry("Tower");

    double scale = 15;

    //Loop through the radius values first
    for (unsigned radius = 1; radius < this->tower->layers; radius++)
    {
        //nseg is the number of blocks in that radius value
        unsigned nseg = radius*12;

        //Convert stuff to string to use in file name
        Ogre::String meshnum = Ogre::StringConverter::toString(radius);
        Ogre::String ra = Ogre::StringConverter::toString(radius);
        entity[radius] = sceneManager->createEntity("Head" + ra, meshnum + ".mesh");

        //Loop through the height
        for (unsigned height = 0; height < this->tower->levels; height++)
        {
            for (unsigned position = 0; position < nseg; position++)
            {
                if (this->tower->blocks[height][radius][position] == 1)
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
                    Ogre::Vector3 scale(scale, scale, scale);

                    //Add the entity to the static geometry
                    sg->addEntity(entity[radius], pos, rot, scale);
                }
            }
        }
    }

	//Display the static geometry
    sg->build();
}

TowerGraphics::~TowerGraphics()
{
}

// Tower Physics

TowerPhysics::TowerPhysics()
{
}

TowerPhysics::TowerPhysics(TowerRefactor *tower, btDiscreteDynamicsWorld* dynamicsWorld)
{
    this->tower = tower;

	double scale = 15;

	btRigidBody* blockRigidBody;// [50][8][84];
	btCollisionObject* blockObject;

    btBulletWorldImporter* fileLoader = new btBulletWorldImporter(dynamicsWorld);

    btCollisionShape* blockShape;

    for (unsigned radius = 1; radius < this->tower->layers; radius++)
    {
        //nseg is the number of blocks in that radius value
        unsigned  nseg = radius*12;

		//Loads bullet meshes for each sector shape
		if(radius == 1)
		{
			fileLoader->loadFile("onebbb.bullet");
			printf("onebb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("oneb.bullet");

		}
		if(radius == 2) 
		{
			fileLoader->loadFile("twobbb.bullet");
			printf("twobb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("twob.bullet");
		}
		if(radius == 3) 
		{
			fileLoader->loadFile("threebbb.bullet");
			printf("threebb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("threeb.bullet");
		}
		if(radius == 4)
		{
			fileLoader->loadFile("fourbbb.bullet");
			printf("fourbb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("fourb.bullet");
		}
		if(radius == 5) 
		{
			fileLoader->loadFile("fivebbb.bullet");
			printf("fiveb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("fiveb.bullet");
		}
		if(radius == 6) 
		{
			fileLoader->loadFile("sixbbb.bullet");
			printf("sixb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("sixb.bullet");
		}
		if(radius == 7) 
		{
			fileLoader->loadFile("sevenbbb.bullet");
			printf("sevenb.bullet loaded\n");
			//blockShape = fileLoader->getCollisionShapeByName("sevenb.bullet");
			
		}

		//blockShape = new btBoxShape(btVector3(scale,scale,scale));
		//blockShape = new btCylinderShape(btVector3(r+1,(scale*height*0.5)+(0.5*scale),r+1));
		//printf("This many objects %i\n", (dynamicsWorld->getNumCollisionObjects()));

        //Loop through the height
        for (unsigned height = 0; height < this->tower->levels; height++)
        {
            for (unsigned position = 0; position < nseg; position++)
            {
                //Generates random segments
                if (this->tower->blocks[height][radius][position] == 1)
                {
                    //rads is the radian value for each block around the centre point
                    double rads = ((1/nseg)+((double)position/(nseg/2)))*PI;
                    //degs is the degree for how much the block is rotated
                    double degs = 360/(double)nseg;
					double xPos = ((scale*radius)+(scale*(0.5)))*cos(rads);
					double yPos = ((scale*((double)height/2)));
					double zPos = ((scale*radius)+(scale*(0.5)))*sin(rads);
                    //Postion k is the height, the others are the postion relative to the middle of the tower

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

					dynamicsWorld->addRigidBody(blockRigidBody);

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
}

TowerPhysics::~TowerPhysics()
{
}
