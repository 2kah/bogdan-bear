#include "Sounds.h"

#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include "include/irrKlang.h"

#include "Player.h"
#include "Turret.h"

Sounds::Sounds(Player *player)
{
    this->engine = irrklang::createIrrKlangDevice();
	this->engine->setSoundVolume(0.5f);

    if (!this->engine)
    {
	    std::cout << "Error: Could not create Sound Engine" << std::endl;
    }

    player->signals.updated.connect(boost::bind(&Sounds::listenerUpdated, this, _1));
}

Sounds::~Sounds()
{
}

void Sounds::listenerUpdated(Player *player)
{
    float x = player->position.x;
	float y = player->position.y;
	float z = player->position.z;
	irrklang::vec3df position(x, y, z);

    this->engine->setListenerPosition(position, irrklang::vec3df(0, 1, 0));
}
