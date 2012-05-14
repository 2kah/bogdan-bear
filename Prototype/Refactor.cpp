#include <iostream>

#include "Refactor.h"
#include "Game.h"
#include "Ping.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500 //NEED TO HIDE CONSOLE WINDOW
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    //INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
    int main(int argc, char *argv[])
#else
    int main(int argc, char *argv[])
#endif
    {
		//HWND hWnd = GetConsoleWindow(); //NEED TO HIDE CONSOLE WINDOW
        //ShowWindow( hWnd, SW_HIDE ); //NEED TO HIDE CONSOLE WINDOW
		if(argc != 2)
			std::cout << "Please enter the IP address of the server as an argument" << std::endl;
		
        Game game;

        game.run(argv[1]);

        return 0;
    }

#ifdef __cplusplus
}
#endif
