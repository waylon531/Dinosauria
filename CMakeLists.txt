#####setup#####
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} cmake)

set(CMAKE_CXX_COMPILER clang++ FORCE)

project(dinosauria)

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

#####find libraries#####

##CEGUI##
find_package(CEGUI COMPONENTS Ogre REQUIRED)

##OGRE##
find_package(OGRE REQUIRED)

##Boost##
set(BOOST_COMPONENTS thread system unit_test_framework filesystem)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

##OIS##
find_package(OIS REQUIRED)

#####targets#####
include_directories(src)

set(LIB_SRC_FILES ${LIB_SRC_FILES} external/pugixml-1.4/src/pugixml.cpp src/util.cpp src/engine/state.cpp src/engine/state_manager.cpp src/engine/component.cpp src/engine/entity.cpp src/engine/entity_mgr.cpp src/engine/world.cpp src/engine/settings.cpp src/scripting/context.cpp src/scripting/bindings.cpp)

add_subdirectory(external)

add_library(libdinosauria STATIC ${LIB_SRC_FILES})

set(GRAPHICS_SRC_FILES src/graphics/context.cpp src/game/state_mmenu.cpp src/game/state_options.cpp src/game/state_test.cpp)
add_executable(dinosauria src/main.cpp ${GRAPHICS_SRC_FILES})
add_executable(runTests src/tests.cpp)

#target_link_libraries(otst pugixml)
target_link_libraries(dinosauria libdinosauria)

target_link_libraries(runTests libdinosauria)
#target_link_libraries(runTests pugixml)

#####link libraries#####

##Boost##
target_link_libraries(runTests ${Boost_LIBRARIES})
target_link_libraries(dinosauria ${Boost_LIBRARIES})
target_link_libraries(libdinosauria ${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

##OGRE##
target_link_libraries(dinosauria ${OGRE_LIBRARIES})
include_directories(${OGRE_INCLUDE_DIRS})

##OIS##
target_link_libraries(dinosauria ${OIS_LIBRARIES})
include_directories(${OIS_INCLUDE_DIRS})

##CEGUI##
target_link_libraries(dinosauria ${CEGUI_LIBRARY})
target_link_libraries(runTests ${CEGUI_LIBRARY})
target_link_libraries(dinosauria CEGUILuaScriptModule)
target_link_libraries(runTests CEGUILuaScriptModule)
include_directories(${CEGUI_INCLUDE_DIR})

##Lua##
#link_directories(external/lua)
#target_link_libraries(dinosauria lua)
#target_link_libraries(runTests lua)
#target_link_libraries(libdinosauria lua)
include_directories(${LUA_INCLUDE_DIRS})
target_link_libraries(dinosauria ${LUA_LIBRARIES})
target_link_libraries(runTests ${LUA_LIBRARIES})
target_link_libraries(libdinosauria ${LUA_LIBRARIES})

##Luabind##
#link_directories(external/luabind)
include_directories(external/luabind)
target_link_libraries(dinosauria luabind)
target_link_libraries(runTests luabind)
target_link_libraries(libdinosauria luabind)

#####run unit tests#####
######TODO######
