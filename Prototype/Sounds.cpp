#include "Sounds.h"
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include "include/irrKlang.h"
#include "Player.h"
#include "Turret.h"
#include "Platform.h"



Sounds::Sounds()
{
    this->engine = irrklang::createIrrKlangDevice();
	this->engine->setSoundVolume(1.0f);
    if (!this->engine)
    {
	    std::cout << "Error: Could not create Sound Engine" << std::endl;
    }

}

Sounds::~Sounds()
{

}



void Sounds::setListener(Player *player)
{
    player->signals.updated.connect(boost::bind(&Sounds::listenerUpdated, this, _1));

}



void Sounds::listenerUpdated(Player *player)
{
    float x = player->position.x;
	float y = player->position.y;
	float z = player->position.z;

	irrklang::vec3df position(x, y, z);
    this->engine->setListenerPosition(position, irrklang::vec3df(0, 1, 0));

}


void Sounds::createPlatformSound(Platform *platform)
{
    float x = platform->position.x;
	float y = platform->position.y;
	float z = platform->position.z;

	irrklang::vec3df position(x, y, z);
	/*BtOgre convert wasn't recognised here?*/

	create = this->engine->play3D("sounds/pop.wav", position, true, false, true);
 	create->setIsLooped(0);
}

void Sounds::ReloadSound(Player *player)
{
    float x = player->position.x;
	float y = player->position.y;
	float z = player->position.z;

	irrklang::vec3df position(x, y, z);
	/*BtOgre convert wasn't recognised here?*/

	reload = this->engine->play2D("sounds/Gun_Reload.wav");
	//reload->setIsLooped(0);
}



void Sounds::createPlatformStop()
{
try
{
    create->stop();
	}
catch (char* ex){
}
}

void Sounds::enterTurretSound(Turret *turret)
{
    float x = turret->position.x;
	float y = turret->position.y;
	float z = turret->position.z;

	irrklang::vec3df position(x, y, z);
	this->engine->setSoundVolume(0.5f);
	enter = this->engine->play3D("sounds/enter_turret.mp3", position, true, false, true);
	enter->setIsLooped(0);

}

void Sounds::exitTurret()
{
	this->engine->setSoundVolume(1.0f);
}

