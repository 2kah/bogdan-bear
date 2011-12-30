#include "Builder.h"

Builder::Builder()
{
}

Builder::Builder(TowerRefactor *tower)
{
    this->tower = tower;
}

Builder::~Builder(void)
{
}

void Builder::regenerate(void)
{     
    //Loop through 3D array, filling with random 0's and 1's
    for (unsigned height = 0; height < this->tower->levels; ++height)
    {
        for (unsigned radius = 0; radius < this->tower->layers; ++radius)
        {
            for (unsigned position = 0; position < this->tower->sectors; ++position)
            {
                //Worth looking at different seeds for this
                unsigned x = rand() % this->tower->levels; 

                //Makes the tower more sparse at the top - Larger the height, less likely the blocks
                bool block = x >= (height - 2);

                //Makes the tower taper at the top - Bigger radiuses at top are cut off
                block = block && !((floor((double)(this->tower->levels-height)/radius)) <= radius);
                //if (height == (h-1)) x = 1;

                this->tower->blocks[height][radius][position] = block;
            }
        }
    }
}
