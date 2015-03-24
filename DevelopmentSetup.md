# Notes #
  * Make sure your environmental variables use / instead of \ or RakNet's CMake files will fail! (Not sure how to fix this)

# Programs #
  * [TortoiseHg](http://tortoisehg.bitbucket.org/download/index.html)
  * VisualStudio 2010
  * [CMake](http://www.cmake.org/files/v2.8/cmake-2.8.7-win32-x86.exe)

# Dependencies #
  * [Ogre 1.7.3](https://sourceforge.net/projects/ogre/files/ogre/1.7/OgreSDK_vc10_v1-7-3.exe/download) with environmental variable OGRE\_HOME
  * [Bullet 2.79](http://bullet.googlecode.com/files/bullet-2.79-rev2440.zip) with the bullet folder in Prototype/
  * [Boost 1.47 (installer)](http://boostpro.com/download/boost_1_47_setup.exe) with environmental variable Boost\_ROOT. Make sure you check the boxes for threaded libs as this will compile the necessary .lib files for you.
  * [RakNet 4.035](http://www.jenkinssoftware.com/raknet/downloads/RakNet_PC-4.035.zip) copied in Prototype/raknet.
  * [irrKlang 1.3.0b](http://www.ambiera.at/downloads/irrKlang-1.3.0b.zip) with the include folder copied into Prototype/.
  * New folder Refactor with environmental variable REFACTOR\_HOME (you can put this folder anywhere, it's the place where the executable will be built and run)
  * Copy Ogre media to Refactor/media
  * Copy Models from the repository into Refactor/media/models
  * Copy Ogre bin/Debug to Refactor/bin/Debug
  * Copy Ogre bin/Release to Refactor/bin/RelWithDebugInfo
  * Copy irrKlang bin/win-32/visualStudio/ dlls into Refactor/bin/RelWithDebugInfo
  * Copy irrKlang lib/Win-32/visualStudio/ contents to Refactor/bin/RelWithDebugInfo
  * Copy Prototype/sounds directory to Refactor/bin/RelWithDebugInfo
  * Copy irrKlang.lib from lib/Win-32/visualStudio/ to Prototype-Build folder (or wherever you've set CMake to build the binaries)

# Instructions #
  * Use TortoiseHg to pull the source code
  * Add the bullet and raknet and include directories to your Hg ignored
  * Open Prototype/CMakeLists.txt with CMake
  * Set build directory to Prototype-Build/
  * Check the variable DISABLE\_EXAMPLES
  * Configure & Generate
  * Add Prototype-Build to your Hg ignored
  * Open Prototype-Build/Prototype.sln
  * Set build mode to RelWithDebInfo
  * Set debug working directory to $(REFACTOR\_HOME)/bin/RelWithDebInfo
  * Set Prototype as StartUp project