#include "Tower.h"

#include <math.h>

#include <iostream>
#include <vector>

#include <OGRE/OgreVector3.h>

#define PI 3.14159265

Tower::Tower(double blocksize, unsigned levels, std::vector<unsigned> structure)
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

    this->radii[0] = 5;

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
    double size = this->heights[layer];
    double radius = this->radii[layer];

    double angle = ((2*PI) / this->blocks[level][layer].size());
    double offset = angle * sector;

    blockheight *= 2 * PI * this->radii[this->layers - 1] / this->sectors / 2;

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

    BlockPoints points = {a1, b1, c1, d1,
                          a2, b2, c2, d2};

    return points;
}

std::vector<BlockTriangle> Tower::getBlockTriangles(unsigned level, unsigned layer, unsigned sector)
{
    std::vector<BlockTriangle> triangles;

    BlockPoints points = this->getBlockPoints(level, layer, sector);

    // back face
    Ogre::Vector3 inner_clock_normal = points.c2 - points.b2;
    Ogre::Vector3 inner_anti_normal = points.a1 - points.d1;

    inner_clock_normal.normalise();
    inner_anti_normal.normalise();

    BlockTriangle back1;
    back1.points[0] = points.b2;
    back1.points[1] = points.b1;
    back1.points[2] = points.d1;

    back1.colours[0] = Ogre::ColourValue::Blue;
    back1.colours[1] = Ogre::ColourValue::Green;
    back1.colours[2] = Ogre::ColourValue::White;

    back1.normals[0] = inner_clock_normal;
    back1.normals[1] = inner_clock_normal;
    back1.normals[2] = inner_anti_normal;

    BlockTriangle back2;
    back2.points[0] = points.d1;
    back2.points[1] = points.d2;
    back2.points[2] = points.b2;

    back2.colours[0] = Ogre::ColourValue::White;
    back2.colours[1] = Ogre::ColourValue::Red;
    back2.colours[2] = Ogre::ColourValue::Blue;

    back2.normals[0] = inner_anti_normal;
    back2.normals[1] = inner_anti_normal;
    back2.normals[2] = inner_clock_normal;

    triangles.push_back(back1);
    triangles.push_back(back2);

    // clockwise face
    Ogre::Vector3 clock_normal = points.b2 - points.b1;
    clock_normal = clock_normal.crossProduct(points.b2 - points.c2);
    clock_normal.normalise();

    BlockTriangle clock_1;
    clock_1.points[0] = points.c2;
    clock_1.points[1] = points.c1;
    clock_1.points[2] = points.b1;

    clock_1.colours[0] = Ogre::ColourValue::Red;
    clock_1.colours[1] = Ogre::ColourValue::Blue;
    clock_1.colours[2] = Ogre::ColourValue::Green;

    clock_1.normals[0] = clock_normal;
    clock_1.normals[1] = clock_normal;
    clock_1.normals[2] = clock_normal;

    BlockTriangle clock_2;
    clock_2.points[0] = points.b1;
    clock_2.points[1] = points.b2;
    clock_2.points[2] = points.c2;

    clock_2.colours[0] = Ogre::ColourValue::Red;
    clock_2.colours[1] = Ogre::ColourValue::Blue;
    clock_2.colours[2] = Ogre::ColourValue::Green;

    clock_2.normals[0] = clock_normal;
    clock_2.normals[1] = clock_normal;
    clock_2.normals[2] = clock_normal;

    if (!this->blocks[level][layer][(sector - 1) % this->blocks[level][layer].size()]) {
        triangles.push_back(clock_1);
        triangles.push_back(clock_2);
    }

    // anticlockwise face
    Ogre::Vector3 anti_normal = points.d2 - points.a2;
    anti_normal = anti_normal.crossProduct(points.d2 - points.d1);
    anti_normal.normalise();

    BlockTriangle anti_1;
    anti_1.points[0] = points.d2;
    anti_1.points[1] = points.d1;
    anti_1.points[2] = points.a1;

    anti_1.colours[0] = Ogre::ColourValue::Red;
    anti_1.colours[1] = Ogre::ColourValue::White;
    anti_1.colours[2] = Ogre::ColourValue::Red;

    anti_1.normals[0] = anti_normal;
    anti_1.normals[1] = anti_normal;
    anti_1.normals[2] = anti_normal;

    BlockTriangle anti_2;
    anti_2.points[0] = points.a1;
    anti_2.points[1] = points.a2;
    anti_2.points[2] = points.d2;

    anti_2.colours[0] = Ogre::ColourValue::Red;
    anti_2.colours[1] = Ogre::ColourValue::White;
    anti_2.colours[2] = Ogre::ColourValue::Red;

    anti_2.normals[0] = anti_normal;
    anti_2.normals[1] = anti_normal;
    anti_2.normals[2] = anti_normal;

    if (!this->blocks[level][layer][(sector + 1) % this->blocks[level][layer].size()]) {
        triangles.push_back(anti_1);
        triangles.push_back(anti_2);
    }

    // wrong up face
    Ogre::Vector3 up_normal(Ogre::Vector3::UNIT_Y);

    BlockTriangle up_1;
    up_1.points[0] = points.c2;
    up_1.points[1] = points.b2;
    up_1.points[2] = points.d2;

    up_1.colours[0] = Ogre::ColourValue::Green;
    up_1.colours[1] = Ogre::ColourValue::Blue;
    up_1.colours[2] = Ogre::ColourValue::Red;

    up_1.normals[0] = up_normal;
    up_1.normals[1] = up_normal;
    up_1.normals[2] = up_normal;

    BlockTriangle up_2;
    up_2.points[0] = points.d2;
    up_2.points[1] = points.a2;
    up_2.points[2] = points.c2;

    up_2.colours[0] = Ogre::ColourValue::Red;
    up_2.colours[1] = Ogre::ColourValue::White;
    up_2.colours[2] = Ogre::ColourValue::Green;

    up_2.normals[0] = up_normal;
    up_2.normals[1] = up_normal;
    up_2.normals[2] = up_normal;

    triangles.push_back(up_1);
    triangles.push_back(up_2);

    // wrong down face
    Ogre::Vector3 down_normal(-Ogre::Vector3::UNIT_Y);

    BlockTriangle down_1;
    down_1.points[0] = points.d1;
    down_1.points[1] = points.b1;
    down_1.points[2] = points.c1;

    down_1.colours[0] = Ogre::ColourValue::White;
    down_1.colours[1] = Ogre::ColourValue::Green;
    down_1.colours[2] = Ogre::ColourValue::Blue;

    down_1.normals[0] = down_normal;
    down_1.normals[1] = down_normal;
    down_1.normals[2] = down_normal;

    BlockTriangle down_2;
    down_2.points[0] = points.c1;
    down_2.points[1] = points.a1;
    down_2.points[2] = points.d1;

    down_2.colours[0] = Ogre::ColourValue::Blue;
    down_2.colours[1] = Ogre::ColourValue::Red;
    down_2.colours[2] = Ogre::ColourValue::White;

    down_2.normals[0] = down_normal;
    down_2.normals[1] = down_normal;
    down_2.normals[2] = down_normal;

    triangles.push_back(down_1);
    triangles.push_back(down_2);

    // wrong forward face
    Ogre::Vector3 outer_clock_normal = -inner_clock_normal;
    Ogre::Vector3 outer_anti_normal = -inner_anti_normal;

    BlockTriangle forward_1;
    forward_1.points[0] = points.a2;
    forward_1.points[1] = points.a1;
    forward_1.points[2] = points.c1;

    forward_1.colours[0] = Ogre::ColourValue::White;
    forward_1.colours[1] = Ogre::ColourValue::Red;
    forward_1.colours[2] = Ogre::ColourValue::Blue;

    forward_1.normals[0] = outer_anti_normal;
    forward_1.normals[1] = outer_anti_normal;
    forward_1.normals[2] = outer_clock_normal;

    BlockTriangle forward_2;
    forward_2.points[0] = points.c1;
    forward_2.points[1] = points.c2;
    forward_2.points[2] = points.a2;

    forward_2.colours[0] = Ogre::ColourValue::Blue;
    forward_2.colours[1] = Ogre::ColourValue::Green;
    forward_2.colours[2] = Ogre::ColourValue::White;

    forward_2.normals[0] = outer_clock_normal;
    forward_2.normals[1] = outer_clock_normal;
    forward_2.normals[2] = outer_anti_normal;

    triangles.push_back(forward_1);
    triangles.push_back(forward_2);

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
