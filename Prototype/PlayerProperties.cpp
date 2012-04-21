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
		return "newWarriorPurple";
	}
	else if(this->team == 1) {
		return "newWarriorOrange";
	}
	else if(this->team == 2) {
		return "newWarriorGreen";
	}
	else if(this->team == 3) {
		return "newWarriorRed";
	}
}

bool PlayerProperties::isPlayerLocal()
{
	return this->isLocal;
}