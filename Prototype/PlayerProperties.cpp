#include <string>

#include "PlayerProperties.h"

using namespace std;

PlayerProperties::PlayerProperties(int team, bool isLocal)
{
	this->team = team;
	this->isLocal = isLocal;
}

PlayerProperties::~PlayerProperties()
{

}

int PlayerProperties::getTeam()
{
	return this->team;
}

string PlayerProperties::getCostumeName()
{
    if(this->team == 0) {
		return "newWarriorRed";
	}
	else if(this->team == 1) {
		return "newWarriorOrange";
	}
	else if(this->team == 2) {
		return "newWarriorPurple";
	}
	else if(this->team == 3) {
		return "newWarriorGreen";
	}
	else return "";
}

string PlayerProperties::getGunPlatformsName()
{
    if(this->team == 0) {
		return "gunPlatformsRed";
	}
	else if(this->team == 1) {
		return "gunPlatformsOrange";
	}
	else if(this->team == 2) {
		return "gunPlatformsPurple";
	}
	else if(this->team == 3) {
		return "gunPlatformsGreen";
	}
	else return "";
}

string PlayerProperties::getGunAmmoName()
{
    if(this->team == 0) {
		return "gunAmmoRed";
	}
	else if(this->team == 1) {
		return "gunAmmoOrange";
	}
	else if(this->team == 2) {
		return "gunAmmoPurple";
	}
	else if(this->team == 3) {
		return "gunAmmoGreen";
	}
	else return "";
}


bool PlayerProperties::isPlayerLocal()
{
	return this->isLocal;
}