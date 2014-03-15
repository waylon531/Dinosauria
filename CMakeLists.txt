#####setup#####
project(dinosauria)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} cmake)

set(CMAKE_CXX_COMPILER clang++)

#####configuration#####
set(DEBUG true)
set(RELEASE false)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-deprecated")

if(DEBUG)
	set(GLOBAL_FLAGS "${GLOBAL_FLAGS} -O0 -g")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
endif(DEBUG)

if(RELEASE)
	set(GLOBAL_FLAGS "${GLOBAL_FLAGS} -O4")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRELEASE")
endif(RELEASE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GLOBAL_FLAGS}")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${GLOBAL_FLAGS}")

#####targets#####
include_directories(src)

set(LIB_SRC_FILES ${LIB_SRC_FILES} external/pugixml-1.4/src/pugixml.cpp src/util.cpp src/engine/state_manager.cpp src/engine/settings.cpp)

add_subdirectory(external)

add_library(libdinosauria STATIC ${LIB_SRC_FILES})

set(GRAPHICS_SRC_FILES src/graphics/context.cpp src/game/state_mmenu.cpp src/game/state_options.cpp src/game/state_test.cpp)
add_executable(dinosauria src/main.cpp ${GRAPHICS_SRC_FILES})
add_executable(runTests src/tests.cpp)

#target_link_libraries(otst pugixml)
target_link_libraries(dinosauria libdinosauria)

target_link_libraries(runTests libdinosauria)
#target_link_libraries(runTests pugixml)

#####libraries#####

##Boost##
set(BOOST_COMPONENTS thread system unit_test_framework)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
target_link_libraries(runTests ${Boost_LIBRARIES})
target_link_libraries(dinosauria ${Boost_LIBRARIES})
target_link_libraries(libdinosauria ${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

##OGRE##
find_package(OGRE REQUIRED)
target_link_libraries(dinosauria ${OGRE_LIBRARIES})
include_directories(${OGRE_INCLUDE_DIRS})

##OIS##
find_package(OIS REQUIRED)
target_link_libraries(dinosauria ${OIS_LIBRARIES})
include_directories(${OIS_INCLUDE_DIRS})

##CEGUI##
find_package(CEGUI COMPONENTS Ogre REQUIRED)
target_link_libraries(dinosauria ${CEGUI_LIBRARY})
include_directories(${CEGUI_INCLUDE_DIR})

#####run unit tests#####
######TODO######

##debug info##
#message("cxx_flags = ${CMAKE_CXX_FLAGS}")
#message("link_flags = ${CMAKE_CXX_LINK_FLAGS}")
