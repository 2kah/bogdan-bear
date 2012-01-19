#include "Tower.h"

#include <math.h>

#include <iostream>
#include <vector>

#include <OGRE/OgreVector3.h>

#define PI 3.14159265

Tower::Tower(double blocksize, unsigned levels, unsigned layers, unsigned sectors)
    : blocksize(blocksize),
    levels(levels),
    layers(layers),
    sectors(sectors),
    blocks(std::vector<std::vector<std::vector<bool> > >(levels, std::vector<std::vector<bool> >(layers, std::vector<bool>(sectors, false))))
{
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

ComplexTower::ComplexTower(double blocksize, unsigned levels, std::vector<unsigned> structure)
{
    this->blocksize = blocksize;
    this->levels = levels;
    this->layers = structure.size();
    this->sectors = structure[structure.size()-1];

    this->blocks = std::vector<std::vector<std::vector<bool> > >(levels, std::vector<std::vector<bool> >(layers, std::vector<bool>(1, false)));

    // Fill each layer in the blocks with a vector of missing blocks of the right size
    for (unsigned level = 0; level < this->levels; ++level)
    {
        for (unsigned layer = 0; layer < this->layers; ++layer)
        {
            this->blocks[level][layer] = std::vector<bool>(structure[layer], false);
        }
    }

    this->subdivide = std::vector<bool>(this->layers, false);
    this->radii = std::vector<double>(this->layers, 0);
    this->heights = std::vector<double>(this->layers, 0);

    this->radii[0] = 25;

    for (unsigned layer = 1; layer < this->layers; ++layer)
    {
        this->subdivide[layer-1] = structure[layer-1] != structure[layer];

        // magic formula to give square shaped blocks
        // (inner circumference of ring) / (number of blocks in this ring - pi)
        this->heights[layer-1] = (2 * PI * this->radii[layer-1]) / (structure[layer-1] - PI);
        
        this->radii[layer] = this->radii[layer-1] + this->heights[layer-1];
    }

    this->subdivide[this->layers-1] = false;
    this->heights[this->layers-1] = (2 * PI * this->radii[this->layers-1]) / (structure[this->layers-1] - PI);;

    std::cout << std::endl;
}

// These are not the points of the non-generated models
// Points as shown in http://you.mongle.me/tower/circles/gamesproject.png
// base: a1, b1, c1, d1
//  top: a2, b2, c2, d2
ComplexPoints ComplexTower::getComplexPoints(unsigned level, unsigned layer, unsigned sector)
{
    double blockheight = this->blocksize;
    double size = this->blocksize;
    double radius = size * layer;

    double angle = ((2*PI) / this->blocks[layer].size());
    double offset = angle * sector;

    double bottom = blockheight * level;
    double top = blockheight * (level + 1);

    Ogre::Vector3 a1((radius + size) * cos(offset + angle), bottom, (radius + size) * sin(offset + angle));
    Ogre::Vector3 a2((radius + size) * cos(offset + angle),    top, (radius + size) * sin(offset + angle));

    Ogre::Vector3 d1((radius) * cos(offset + angle), bottom, (radius) * sin(offset + angle));
    Ogre::Vector3 d2((radius) * cos(offset + angle),    top, (radius) * sin(offset + angle));

    Ogre::Vector3 b1((radius) * cos(offset), bottom, (radius) * sin(offset));
    Ogre::Vector3 b2((radius) * cos(offset),    top, (radius) * sin(offset));

    Ogre::Vector3 c1((radius + size) * cos(offset), bottom, (radius + size) * sin(offset));
    Ogre::Vector3 c2((radius + size) * cos(offset),    top, (radius + size) * sin(offset));

    ComplexPoints points = {a1, b1, c1, d1,
                            a2, b2, c2, d2};

    return points;
}

std::vector<BlockTriangle> ComplexTower::getBlockTriangles(unsigned level, unsigned layer, unsigned sector)
{
    std::vector<BlockTriangle> triangles;

    // back face

    // clockwise face

    // anticlockwise face

    // single forward face
    if (!this->subdivide[layer])
    {
    }
    // subdivide forward faces
    else
    {
    }

    return triangles;
}
