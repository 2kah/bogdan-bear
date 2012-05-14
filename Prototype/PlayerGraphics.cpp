#include "PlayerGraphics.h"

#include <string>
#include <math.h>

#include "Player.h"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreSubEntity.h>
#define PI 3.1415

PlayerGraphics::PlayerGraphics(Player *player, Ogre::SceneManager *sceneManager)
{
    this->player = player;
    
    this->sceneManager = sceneManager;
	this->sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    //this->entity = sceneManager->createEntity(this->player->prop->getCostumeName());

	if(!this->player->prop->isPlayerLocal())
	{
	    this->entity = sceneManager->createEntity("newWarrior.mesh");
	    this->entity->setMaterialName(this->player->prop->getCostumeName());//, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
	    //this->entity->setMaterial(Ogre::MaterialManager::getSingleton().getByName(this->player->prop->getCostumeName()));
		this->sceneNode->attachObject(this->entity);
		float scale = 0.7;
	    this->sceneNode->setScale(scale, scale, scale);

		animationState = this->entity->getAnimationState("Run");
		animationState->setLoop(true);
		animationState->setEnabled(true);
	}
	else
	{
        Ogre::SceneNode* attach = this->sceneNode->createChildSceneNode();
	    this->camNode = attach;
	    attach->setPosition(0, 3.0 ,0);
	    
	    this->entity = sceneManager->createEntity("gunBody2.mesh");
	    Ogre::SceneNode* attach2 = attach->createChildSceneNode();
        attach2->attachObject(this->entity);
		//this->sceneNode->setScale(3,3,3);
	    //attach2->setPosition(-2.3, -0.8 ,-2);
		//attach2->pitch(Ogre::Radian(0.1));
		//attach2->roll(Ogre::Radian(-0.1));
	    this->sceneNode->setScale(3,3,3);
		attach2->setScale(0.5, 0.5, 0.5);
	    //attach2->setPosition(-1.15, -0.4 ,-1);
	    attach2->setPosition(-1.15, -0.4 ,-0.9);
		attach2->pitch(Ogre::Radian(0.1));
		attach2->roll(Ogre::Radian(-0.1));
	    //attach->setPosition(0,2.5,0);
	    
	    
	    this->team1 = sceneManager->createEntity("gunTeam1.mesh");
	    this->team1->setMaterialName("gunTeam1");
	    Ogre::SceneNode* attach3 = attach2->createChildSceneNode();
	    attach3->attachObject(this->team1);
	    this->team2 = sceneManager->createEntity("gunTeam2.mesh");
	    this->team2->setMaterialName("gunTeam2");
	    attach3 = attach2->createChildSceneNode();
	    attach3->attachObject(this->team2);
	    this->team3 = sceneManager->createEntity("gunTeam3.mesh");
	    this->team3->setMaterialName("gunTeam3");
	    attach3 = attach2->createChildSceneNode();
	    attach3->attachObject(this->team3);
	    this->team4 = sceneManager->createEntity("gunTeam4.mesh");
	    this->team4->setMaterialName("gunTeam4");
	    attach3 = attach2->createChildSceneNode();
	    attach3->attachObject(this->team4);
	    this->teamWin = sceneManager->createEntity("gunTeamWin.mesh");
	    this->teamWin->setMaterialName("gunTeamWin");
	    attach3 = attach2->createChildSceneNode();
	    attach3->attachObject(this->teamWin);
	    this->ammo1 = sceneManager->createEntity("gunAmmo1.mesh");
	    this->ammo1->setMaterialName(this->player->prop->getGunAmmoName());
	    attach3 = attach2->createChildSceneNode();
	    attach3->attachObject(this->ammo1);
	    this->ammo2 = sceneManager->createEntity("gunAmmo2.mesh");
	    this->ammo2->setMaterialName(this->player->prop->getGunAmmoName() + "1");
	    attach3 = attach2->createChildSceneNode();
	    attach3->attachObject(this->ammo2);
	    this->ammo3 = sceneManager->createEntity("gunAmmo3.mesh");
	    this->ammo3->setMaterialName(this->player->prop->getGunAmmoName() + "2");
	    attach3 = attach2->createChildSceneNode();
	    attach3->attachObject(this->ammo3);
	    this->ammo4 = sceneManager->createEntity("gunAmmo4.mesh");
	    this->ammo4->setMaterialName(this->player->prop->getGunAmmoName() + "3");
	    attach3 = attach2->createChildSceneNode();
	    attach3->attachObject(this->ammo4);
		this->platforms = sceneManager->createEntity("gunPlatforms.mesh");
	    this->platforms->setMaterialName(this->player->prop->getGunPlatformsName());
	    attach3 = attach2->createChildSceneNode();
	    attach3->attachObject(this->platforms);
	    
	    
	    this->check = 0;
	}

    this->playerUpdated(this->player);

    rem = this->player->signals.removed.connect(boost::bind(&PlayerGraphics::playerRemoved, this, _1));
    up = this->player->signals.updated.connect(boost::bind(&PlayerGraphics::playerUpdated, this, _1));
    set = this->player->signals.stateSet.connect(boost::bind(&PlayerGraphics::playerUpdated, this, _1));
}

PlayerGraphics::~PlayerGraphics()
{
    this->sceneManager->destroyEntity(this->entity);
	/*this->sceneManager->destroyAnimationState("Run");
	if(this->team1 != NULL) this->sceneManager->destroyEntity(this->team1);
	if(this->team2 != NULL) this->sceneManager->destroyEntity(this->team2);
	if(this->team3 != NULL) this->sceneManager->destroyEntity(this->team3);
	if(this->team4 != NULL) this->sceneManager->destroyEntity(this->team4);
	if(this->teamWin != NULL) this->sceneManager->destroyEntity(this->teamWin);
	if(this->ammo1 != NULL) this->sceneManager->destroyEntity(this->ammo1);
	if(this->ammo2 != NULL) this->sceneManager->destroyEntity(this->ammo2);
	if(this->ammo3 != NULL) this->sceneManager->destroyEntity(this->ammo3);
	if(this->ammo4 != NULL) this->sceneManager->destroyEntity(this->ammo4);
	if(this->platforms != NULL) this->sceneManager->destroyEntity(this->platforms);*/
    this->sceneManager->destroySceneNode(this->sceneNode);

}

void PlayerGraphics::playerRemoved(Player *player)
{
    delete this;
}

void PlayerGraphics::playerUpdated(Player *player)
{

	//
	//Ogre::Vector3 addition = Ogre::Vector3(5,9,0) * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y);
    //this->sceneNode->setOrientation(this->player->orientation * this->player->relativeAim * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y));
	if(this->player->prop->isPlayerLocal())
	{
	    this->sceneNode->setPosition(this->player->position);
	    this->camNode->setOrientation(this->player->orientation * this->player->relativeAim * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y));
	    //printf("x %f, y %f, z %f\n", this->player->position.x, this->player->position.y, this->player->position.z);
	    if(this->check >= 1) this->check = 0;
	    this->check += 0.001;

	    this->team1->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->player->getScore(0)/(double)12000);
	    this->team2->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->player->getScore(1)/(double)12000);
	    this->team3->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->player->getScore(2)/(double)12000);
	    this->team4->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->player->getScore(3)/(double)12000);
	    this->team1->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->player->getScore(0)/(double)12000);
	    this->team2->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->player->getScore(1)/(double)12000);
	    this->team3->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->player->getScore(2)/(double)12000);
	    this->team4->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->player->getScore(3)/(double)12000);

		int max = 4;
		int maxScore = 10;
		for(int i = 0; i < 4; i++) {
			if(this->player->getScore(i) > maxScore) {
				max = i;
				maxScore = this->player->getScore(i);
			}
		}
		if(max == 0) {
		    this->teamWin->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0.7, 0);
		    this->teamWin->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0.7, 0);
		}
		else if(max == 1) {
		    this->teamWin->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, 0.5);
		    this->teamWin->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, 0.5);
		}
		else if(max == 2) {
		    this->teamWin->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, 0.25);
		    this->teamWin->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, 0.25);
		}
		else if(max == 3) {
		    this->teamWin->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0.45, 0);
		    this->teamWin->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0.45, 0);
		}
		else {
		    this->teamWin->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, 0);
		    this->teamWin->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, 0);
		}

		/*this->team1->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->check);
	    this->team2->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->check);
	    this->team3->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->check);
	    this->team4->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->check);
	    this->team1->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->check);
	    this->team2->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->check);
	    this->team3->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->check);
	    this->team4->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, this->check);*/

	    double ammo = this->player->getAmmo();
		int reloading = this->player->getAmmoAmount();

		double ammoClips[] = {-0.4, -0.4, -0.4, -0.4};
		for(int i = 0; i < reloading; i++)
	    {
		    ammoClips[i] = 0;
	    }
		if(reloading < 4) ammoClips[reloading] = ammo;

		//printf("%lf, %lf, %lf, %lf\n", ammoClips[0], ammoClips[1], ammoClips[2], ammoClips[3]);
	    this->ammo1->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, ammoClips[0]);
	    this->ammo2->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, ammoClips[1]);
	    this->ammo3->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, ammoClips[2]);
	    this->ammo4->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, ammoClips[3]);
	    this->ammo1->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, ammoClips[0]);
	    this->ammo2->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, ammoClips[1]);
	    this->ammo3->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, ammoClips[2]);
	    this->ammo4->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, ammoClips[3]);

		this->platforms->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, 0.542 - this->player->getPlatformReload());
		this->platforms->getSubEntity(0)->getMaterial()->getTechnique(1)->getPass(0)->getTextureUnitState(0)->setTextureScroll(0, 0.542 - this->player->getPlatformReload());
	}
	else
	{
		if(this->sceneNode != NULL && player != NULL) {
		    if(player->anim) {
		    	if(this->animationState->getEnabled()) {
		            this->animationState->addTime(0.01);
		    	}
		    	else {
		    		this->animationState->setEnabled(true);
		    	}
		    } 
		    else {
		    	this->animationState->setEnabled(false);
		    }

		    this->sceneNode->setPosition(player->position.x, player->position.y + 5, player->position.z);
		    this->sceneNode->setOrientation(player->orientation * Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Y));
		}
	}
	//this->orientation = Ogre::Quaternion(Ogre::Degree(yaw), Ogre::Vector3::UNIT_Y);
	//this->orientation = this->orientation * Ogre::Quaternion(Ogre::Degree(pitch), Ogre::Vector3::UNIT_Z);
	//this->sceneNode->setOrientation(Ogre::Quaternion(Ogre::Degree((this->player->orientation * this->player->relativeAim * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y)).getYaw().valueDegrees()), Ogre::Vector3::UNIT_Y));
	//this->gunNode->setDirection(this->player->relativeAim.x, this->player->relativeAim.y, this->player->relativeAim.z);
	//this->sceneNode->setOrientation(this->sceneNode->getOrientation() * Ogre::Quaternion(Ogre::Degree((this->player->orientation * this->player->relativeAim * Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y)).getPitch().valueDegrees()), Ogre::Vector3::UNIT_Z));
    //this->gunNode->setPosition(-this->player->position.x + (-2*(cos(((this->player->orientation.getYaw().valueDegrees() - 90)*PI)/180))), -this->player->position.y + 2.5, -this->player->position.z + (-1.5*(sin(((this->player->orientation.getYaw().valueDegrees() - 90)*PI)/180))));
	//this->gunNode->setPosition(-2,2.5,-1.5);
    
}

void PlayerGraphics::disconnectSignals() {
	rem.disconnect();
	if(!rem.connected()) printf("Remove disconnected\n");
	up.disconnect();
	if(!up.connected()) printf("Update disconnected\n");
	set.disconnect();
	if(!set.connected()) printf("SetState disconnected\n");
	//rem.~connection();
	//up.~connection();
	//set.~connection();
}

