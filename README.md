#Dinosauria 0.1.1

##Introduction
This is a game from the perspective of a dinosaur with the goal of being very realistic (and scientifically accurate). It is currently in a stage where pretty much all hat you can actually do is walk around and look at the graphics. Most of the work so far has been on the graphics, but I plan to implement better gameplay soon.

##Compiling
In the "dinosauira" root direcotry, type:
```
cmake -"Unix Makefiles" #or whatever type of build you want to generate

make -j2
```
Note that if you want documentation to be generated, you must have doxygen installed. In addition, if you want inheritance graphs, you must have graphviz dot installed.

##Prebuilt Binaries
Prebuilt binaries are hosted on Google Drive at https://drive.google.com/folderview?id=0B8ATzH7YnDGKb1dwRjA2ZkJWS1E&usp=sharing. They are currently  only available for Linux, but Mac OSX support will be added. There are no plans as of now to build Windows binaries, but that may change in the future. Compiling anything on Windows is really unpleasant and compling a portable binary is even worse.

##License
See the file "LICENSE".
  
##Authors
 - Benjamin Lee (design, programming)
 - Nikolas Kubler (art, design)