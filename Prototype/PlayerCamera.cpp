#include "PlayerCamera.h"

#include <string>

#include "Player.h"

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreCamera.h>

PlayerCamera::PlayerCamera(Player *player, Ogre::Camera *camera)
{
    this->player = player;
    this->camera = camera;

    this->playerUpdated(this->player);

    this->player->signals.removed.connect(boost::bind(&PlayerCamera::playerRemoved, this, _1));
    this->player->signals.updated.connect(boost::bind(&PlayerCamera::playerUpdated, this, _1));
}

PlayerCamera::~PlayerCamera()
{
}

void PlayerCamera::playerRemoved(Player *player)
{
    delete this;
}

void PlayerCamera::playerUpdated(Player *player)
{
    this->camera->setPosition(this->player->position + Ogre::Vector3::UNIT_Y * 180 / 16.0);
    this->camera->setOrientation(this->player->orientation * this->player->relativeAim);
}
