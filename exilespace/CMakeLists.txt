PROJECT( exilespace )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

# sources files
Set (PROJECT_SRCS
    src/main.cpp
    src/esControl/ProgramController.cpp
    src/esControl/GameSession.cpp
    src/esControl/Gameboard.cpp
    src/esControl/Section.cpp
    src/esEntity/Entity.cpp
    src/esEntity/Moveable.cpp
    src/esGraphics/Textures.cpp
    src/esGraphics/TextureUtils.cpp
    src/esGraphics/Helpers.cpp
    src/esObjects/Background.cpp
    src/esSubsystems/Engine.cpp
    src/esSubsystems/StandardEngine.cpp
    src/esTools/Debug.cpp
    src/esTools/convolution_fftw.cpp
    src/esUnits/Ship.cpp
    src/esUnits/TestShip.cpp
    src/esUnits/Mothership.cpp
    src/esView/ActiveScreen.cpp

)

ADD_EXECUTABLE( exilespace ${PROJECT_SRCS} )


#map default build type to release
IF(NOT CMAKE_BUILD_TYPE)
       SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)
IF((CMAKE_BUILD_TYPE MATCHES "debug") OR (CMAKE_BUILD_TYPE MATCHES "DEBUG"))
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF((CMAKE_BUILD_TYPE MATCHES "debug") OR (CMAKE_BUILD_TYPE MATCHES "DEBUG"))
IF((CMAKE_BUILD_TYPE MATCHES "release") OR (CMAKE_BUILD_TYPE MATCHES "RELEASE"))
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF((CMAKE_BUILD_TYPE MATCHES "release") OR (CMAKE_BUILD_TYPE MATCHES "RELEASE"))
#enable or disable debugging
IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    ADD_DEFINITIONS(-DDEBUG )
    SET( LINK_LIBRARIES
        /usr/local/include/boost_1_55_0/stage/lib/libboost_system.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_date_time.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_filesystem.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_program_options.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_signals.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_serialization.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_thread.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_unit_test_framework.a;/usr/lib/libpthread.so
    )
ENDIF (CMAKE_BUILD_TYPE MATCHES "Debug")
IF(CMAKE_BUILD_TYPE MATCHES "Release")
    ADD_DEFINITIONS(-DNDEBUG )
    ADD_DEFINITIONS(-DBOOST_UBLAS_NDEBUG )
        add_DEFINITIONS(-O3)
    SET( LINK_LIBRARIES
        /usr/local/include/boost_1_55_0/stage/lib/libboost_system.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_date_time.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_filesystem.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_program_options.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_signals.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_serialization.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_thread.a;/usr/local/include/boost_1_55_0/stage/lib/libboost_unit_test_framework.a;/usr/lib/libpthread.so
    )
ENDIF (CMAKE_BUILD_TYPE MATCHES "Release")
set(CMAKE_CXX_FLAGS " -Wall -Wextra -std=c++11")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

#add boost include directory
include_directories ( /usr/include )


INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

set(CMAKE_EXE_LINKER_FLAGS "-lfftw3_threads -lfftw3 -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread")
#include directories
include_directories (
        /usr/bin
        /usr/include
        /usr/local/include
        /usr/include/SDL2/
)

# Look up SDL2 and add the include directory to our include path
# find_package(SDL2 REQUIRED)
# /include/SDL2
include_directories(${SDL2_INCLUDE_DIR})

TARGET_LINK_LIBRARIES( exilespace ${LINK_LIBRARIES} )

