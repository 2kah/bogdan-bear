cmake_minimum_required(VERSION 2.6)

PROJECT(BULLET_PHYSICS)
SET(BULLET_VERSION 2.79)

IF (NOT CMAKE_BUILD_TYPE)
# SET(CMAKE_BUILD_TYPE "Debug")
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE) 

SUBDIRS(
    src/BulletCollision
    src/BulletDynamics
    src/LinearMath
    Extras/Serialize/BulletWorldImporter
    Extras/Serialize/BulletFileLoader
)
