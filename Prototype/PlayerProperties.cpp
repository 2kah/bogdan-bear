#include <string>

#include "PlayerProperties.h"

using namespace std;

PlayerProperties::PlayerProperties(int team)
{
	this->team = team;
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
		return "ninja.mesh";
	}
	else if(this->team == 1) {
		return "ninja.mesh";
	}
	else if(this->team == 2) {
		return "ninja.mesh";
	}
	else if(this->team == 3) {
		return "ninja.mesh";
	}
}