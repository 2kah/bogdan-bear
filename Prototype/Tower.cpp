#include "Tower.h"

#include <math.h>

#include <iostream>
#include <vector>

#include <OGRE/OgreVector3.h>

#define PI 3.14159265

Tower::Tower(double blocksize, unsigned levels, unsigned layers, unsigned sectors)
{
    this->blocksize = blocksize;
    this->levels = levels;
    this->layers = layers;
    this->sectors = sectors;

    this->blocks = std::vector<std::vector<std::vector<bool> > >(levels, std::vector<std::vector<bool> >(layers, std::vector<bool>(sectors, false)));
}

Tower::~Tower()
{
}

void Tower::update()
{
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
