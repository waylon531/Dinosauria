cmake_minimum_required(VERSION 2.8)

project(Dinosauria)

set(CMAKE_USE_RELATIVE_PATHS true)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(bin/dinosauria ${SRC_FILES})
add_executable(bin/converter converters/converter.cpp)
add_executable(bin/bconverter converters/bconverter.cpp)

include_directories(src external/include)

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -L/usr/local/lib")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -Lexternal/lib")

if(APPLE)
	set(CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
else()
	set(CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x)
endif()

if(UNIX)
	target_link_libraries(bin/dinosauria rt)
	target_link_libraries(bin/dinosauria X11)
	target_link_libraries(bin/dinosauria Xrandr)
	target_link_libraries(bin/dinosauria Xxf86vm)
	target_link_libraries(bin/dinosauria Xi)
	target_link_libraries(bin/dinosauria pthread)
endif()

#find OpenGL
find_package(OpenGL)
if(OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIRS})
	target_link_libraries(bin/dinosauria ${OPENGL_LIBRARIES})

endif()

#find GLEW
find_package(GLEW)
if(GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIRS})
	target_link_libraries(bin/dinosauria ${GLEW_LIBRARIES})
else()
	error(GLEW not found)
endif()

#find Freetype
find_package(Freetype)
if(FREETYPE_FOUND)
	include_directories(${FREETYPE_INCLUDE_DIRS})
	target_link_libraries(bin/dinosauria ${FREETYPE_LIBRARIES})
else()
	error(Freetype not found)
endif()

#find PNG
find_package(PNG)
if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIRS})
	target_link_libraries(bin/dinosauria ${PNG_LIBRARIES})
else()
	error(LibPng not found)
endif()

target_link_libraries(bin/dinosauria glfw3)
target_link_libraries(bin/dinosauria noise)
target_link_libraries(bin/dinosauria freetype-gl)
target_link_libraries(bin/dinosauria pugixml)

target_link_libraries(bin/converter assimp)
target_link_libraries(bin/bconverter assimp)
target_link_libraries(bin/dinosauria assimp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")
