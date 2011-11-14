#include <iostream>

#include "Refactor.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
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
        std::cout << "Hello, Bogdan!" << std::endl;

        return 0;
    }

#ifdef __cplusplus
}
#endif
