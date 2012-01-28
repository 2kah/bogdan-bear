#/*
#-----------------------------------------------------------------------------
#Filename:    CMakeLists.txt
#-----------------------------------------------------------------------------
#
#This source file is part of the
#   ___                 __    __ _ _    _ 
#  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
# //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
#/ \_// (_| | | |  __/  \  /\  /| |   <| |
#\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
#      |___/                              
#      Tutorial Framework
#      http://www.ogre3d.org/tikiwiki/
#-----------------------------------------------------------------------------
#*/
cmake_minimum_required(VERSION 2.6)
 
project(Prototype)
 
if(WIN32)
	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH
		$ENV{OGRE_HOME}/Samples/include
	)
  
  set(${BULLET_PHYSICS_SOURCE_DIR}/bullet)
  set(BULLET_LIBS BulletDynamics BulletCollision LinearMath BulletWorldImporter BulletFileLoader)
  
  set(${RAKNET_SOURCE_DIR}/raknet)
  set(RAKNET_LIBS RakNetDLL RakNetStatic)
endif(WIN32)
 
if(UNIX)
	if(EXISTS "/usr/local/lib/OGRE/cmake")

	  set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	  set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/") # We could just *assume* that developers uses this basepath : /usr/local

	elseif(EXISTS "/usr/lib/OGRE/cmake")

	  set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	  set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one

	else ()
	  message(SEND_ERROR "Failed to find module path.")
	endif(EXISTS "/usr/local/lib/OGRE/cmake")
  
  find_package(Bullet REQUIRED)
endif(UNIX)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "$ENV{OGRE_HOME}/boost_1_44")

#set(BULLET_LIBS ${Prototype_SOURCE_DIR}/bullet/lib/BulletDynamics ${Prototype_SOURCE_DIR}/bullet/lib/BulletCollision ${Prototype_SOURCE_DIR}/bullet/lib/LinearMath)

#set(${BULLET_PHYSICS_SOURCE_DIR}/bullet)
#set(BULLET_LIBS BulletDynamics BulletCollision LinearMath BulletWorldImporter BulletFileLoader)
set(BULLET_INCLUDE ${Prototype_SOURCE_DIR}/bullet/src)

#find_package(Bullet REQUIRED)
find_package(OGRE REQUIRED)
find_package(OIS REQUIRED)

set(Boost_ADDITIONAL_VERSIONS "1.47" "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37")

# Find Boost
if (WIN32 OR APPLE)
    set(Boost_USE_STATIC_LIBS TRUE)
else ()
    # Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
    set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
endif ()
if (MINGW)
    # this is probably a bug in CMake: the boost find module tries to look for
    # boost libraries with name libboost_*, but CMake already prefixes library
    # search names with "lib". This is the workaround.
    set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
endif ()
set(Boost_ADDITIONAL_VERSIONS "1.47" "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
# Components that need linking (NB does not include header-only components like bind)
set(OGRE_BOOST_COMPONENTS thread date_time)
find_package(Boost REQUIRED)

# Set up referencing of Boost
include_directories(${Boost_INCLUDE_DIR})
add_definitions(-DBOOST_ALL) # _NO_LIB)
set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
set(RAKNET_LIBRARIES)
 
# raknet maybe
include_directories("raknet/Source")
 
set(HDRS
    ./Refactor.h
	./Game.h
    
    ./GameTestThing.h
    
    ./Updatable.h
    
    ./Object.h
    ./PhysicsObject.h
    
    ./Tower.h
    ./TowerChunk.h
    ./TowerBuilder.h
    ./TowerPhysics.h
    ./TowerGraphics.h

    ./Turret.h
    ./TurretGraphics.h
    
    ./PlayerInput.h
    ./Player.h
    ./PlayerPhysics.h
    ./PlayerGraphics.h
    ./PlayerCamera.h
    ./BtKinematicCharacterController.h
    
    ./Platform.h
    ./PlatformPhysics.h
    ./PlatformGraphics.h
    
    ./Rocket.h
    ./RocketPhysics.h
    ./RocketGraphics.h
    
    ./Explosion.h
    ./ExplosionPhysics.h
    ./ExplosionGraphics.h
    
    ./FallingObject.h
    
    ./BaseApplication.h
    
    ./BtOgreGP.h
    ./BtOgrePG.h
    ./BtOgreExtras.h
)
 
set(SRCS
    ./Refactor.cpp
	./Game.cpp
    
    ./GameTestThing.cpp
    
    ./Tower.cpp
    ./TowerBuilder.cpp
    ./TowerPhysics.cpp
    ./TowerGraphics.cpp
    
    ./Turret.cpp
    ./TurretGraphics.cpp
    
    ./PlayerInput.cpp
    ./Player.cpp
    ./PlayerPhysics.cpp
    ./PlayerGraphics.cpp
    ./PlayerCamera.cpp
    ./BtKinematicCharacterController.cpp
    
    ./Platform.cpp
    ./PlatformPhysics.cpp
    ./PlatformGraphics.cpp
    
    ./Rocket.cpp
    ./RocketPhysics.cpp
    ./RocketGraphics.cpp
    
    ./Explosion.cpp
    ./ExplosionPhysics.cpp
    ./ExplosionGraphics.cpp
    
    ./FallingObject.cpp
    
    ./BaseApplication.cpp
    
    ./BtOgre.cpp
    
    ./Ping.cpp
)

find_package(Boost 1.47 REQUIRED)
add_definitions(-DBOOST_ALL)
link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

include_directories(
    ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
    
    ${BULLET_INCLUDE}
    ${Prototype_SOURCE_DIR}/bullet/Extras/Serialize/BulletWorldImporter
    ${Prototype_SOURCE_DIR}/bullet/Extras/Serialize/BulletFileLoader
)
 
subdirs(raknet)
subdirs(bullet)
 
add_executable(Prototype ${HDRS} ${SRCS})
 
set_target_properties(Prototype PROPERTIES DEBUG_POSTFIX _d)
 
target_link_libraries(
    Prototype
    ${OGRE_LIBRARIES}
    ${OIS_LIBRARIES}
    ${BULLET_LIBS}
    ${RAKNET_LIBS}
    ${BOOST_LIBRARIES}
)
 
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)
 
# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command( TARGET Prototype PRE_BUILD
		COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin )
	add_custom_command( TARGET Prototype POST_BUILD
		COMMAND copy \"$(TargetPath)\" .\\dist\\bin )
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)
 
if(WIN32)
 
	install(TARGETS Prototype
		RUNTIME DESTINATION bin
		CONFIGURATIONS All)
 
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/Media
		DESTINATION ./
		CONFIGURATIONS Release RelWithDebInfo Debug
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins_d.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources_d.cfg
		DESTINATION bin
		CONFIGURATIONS Debug
	)
 
        # NOTE: for the 1.7.1 sdk the OIS dll is called OIS.dll instead of libOIS.dll
        # so you'll have to change that to make it work with 1.7.1
	install(FILES ${OGRE_PLUGIN_DIR_REL}/OgreMain.dll
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_Direct3D9.dll
		${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL.dll
		${OGRE_PLUGIN_DIR_REL}/libOIS.dll
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo
	)
 
	install(FILES ${OGRE_PLUGIN_DIR_DBG}/OgreMain_d.dll
		${OGRE_PLUGIN_DIR_DBG}/RenderSystem_Direct3D9_d.dll
		${OGRE_PLUGIN_DIR_DBG}/RenderSystem_GL_d.dll
		${OGRE_PLUGIN_DIR_DBG}/libOIS_d.dll
		DESTINATION bin
		CONFIGURATIONS Debug
	)
 
   # as of sdk 1.7.2 we need to copy the boost dll's as well
   # because they're not linked statically (it worked with 1.7.1 though)
   install(FILES ${Boost_DATE_TIME_LIBRARY_RELEASE}
      ${Boost_THREAD_LIBRARY_RELEASE}
      DESTINATION bin
      CONFIGURATIONS Release RelWithDebInfo
   )
 
   install(FILES ${Boost_DATE_TIME_LIBRARY_DEBUG}
      ${Boost_THREAD_LIBRARY_DEBUG}
      DESTINATION bin
      CONFIGURATIONS Debug
   )
endif(WIN32)

if(UNIX)
 
	install(TARGETS Prototype
		RUNTIME DESTINATION bin
		CONFIGURATIONS All)
 
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/media
		DESTINATION ./
		CONFIGURATIONS Release RelWithDebInfo Debug
	)
 
	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
		${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
		DESTINATION bin
		CONFIGURATIONS Release RelWithDebInfo Debug
	)
 
endif(UNIX)

set(EXECUTABLE_OUTPUT_PATH $ENV{REFACTOR_HOME}/bin)
set(LIBRARY_OUTPUT_PATH $ENV{REFACTOR_HOME}/bin)
