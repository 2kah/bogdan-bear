#include <iostream>

#include "Game.h"

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::run(void)
{
    std::cout << "You have been eaten by Bogdan!" << std::endl;
    std::cout << "*** GAME OVER ***" << std::endl;
}
