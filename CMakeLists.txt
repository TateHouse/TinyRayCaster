CMAKE_MINIMUM_REQUIRED(VERSION 3.28)

SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 1)
SET(VERSION_PATCH 0)
SET(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

PROJECT(TinyRayCaster VERSION ${VERSION} LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(ENTRY_POINT Source/Main.cpp)

ADD_EXECUTABLE(${PROJECT_NAME} ${ENTRY_POINT})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE Include)

ADD_SUBDIRECTORY(Include)
ADD_SUBDIRECTORY(Source)