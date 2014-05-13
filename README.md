#Dinosauria

##Introduction
This is a game from the perspective of a dinosaur with the goal of being very realistic (and scientifically accurate). It is currently in a stage where pretty much all hat you can actually do is walk around and look at the graphics. Most of the work so far has been on the graphics, but I plan to implement better gameplay soon.
###This branch
This branch is my attempt to start all over again (except for art assets) because the master branch has become too unrepairably messy to continue working on. Specifically, I need to implement a scene graph and a culling algorithm but forgot to add bounding box support to the ```RenderableObjectExt``` class.

##Dependencies
Dinosauria depends on the following libraries:
 - Ogre 1.8 (3D graphics)
 - CEGUI with Ogre support enabled (GUI)
 - OIS (I/O support; this should come with Ogre)
 - Boost (general usage)

The following libraries are packaged in submodules:
 - Lua 5.1 (Scripting)
 - Luabind (generate C++->Lua bindings)
 - Libnoise (Procedural generation)

The following libraries are packaged directly:
 - Pugixml (XML file parsing)

##Compiling
In the "dinosauira" root direcotry, type:
```
mkdir build
cd build
cmake ..
make -j2
```
Note that if you want documentation to be generated, you must have doxygen installed. In addition, if you want inheritance graphs for Doxygen, you must have graphviz dot installed.
###Fetching submodules
Before compiling all submodules must be updated. To do this type the following in the project root:
```
git submodule init
git submodule update
```

##License
See the file "LICENSE".
  
##Authors
 - Benjamin Lee (design, programming)
 - Nikolas Kubler (art, design)
