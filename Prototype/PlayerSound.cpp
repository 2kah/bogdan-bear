#include "PlayerSound.h"
#include "include/irrKlang.h"
#include "Player.h"

PlayerSound::PlayerSound(Player *player)
    : player(player)
{
	float x = player->position.x;
    float y = player->position.y;
	float z = player->position.z;
    irrklang::vec3df position(x, y, z);

	//this->player->signals.updated.connect(boost::bind(&PlayerSound::PlayerWalk, this, _1));
    //this->sound = this->soundEngine->play2D("sounds/test.mp3");
	this->soundEngine = irrklang::createIrrKlangDevice();
    /*irrklang::vec3df position(x, y, z);

    this->sound = this->soundEngine->play3D("sounds/ExplosionsA.mp3", position, true, false, true);

	this->rocket->signals.updated.connect(boost::bind(&RocketSound::rocketUpdated, this, _1));
	this->rocket->signals.exploded.connect(boost::bind(&RocketSound::rocketExploded, this, _1, _2));*/
}
PlayerSound::~PlayerSound()
{
    // stop sounds / clear up here
}

void PlayerSound::PlayerWalk(Player *player)
{
	//this->soundEngine->play2D("sounds/footsteps.mp3");

}

void PlayerSound::PlayerStop(Player *player)
{
	//this->soundEngine->stopAllSounds();
}