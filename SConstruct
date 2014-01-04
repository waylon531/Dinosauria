import os
import fnmatch
from colorizer import colorizer
import doxygen
env = Environment(CXX = "g++",
                  CXXFLAGS = '-std=c++0x -Isrc -Iexternal/include -I/usr/include',
                  LINKFLAGS = '-std=c++0x -L/usr/lib -Lexternal/lib -L/usr/local/lib',
                  LIBS = Split('m stdc++ rt GL GLU GLEW glfw3 Xxf86vm pthread Xrandr X11 Xi freetype freetype-gl png assimp pugixml noise'),
                  CPPPATH = ['src','external/include'],
                  tools = ['default'],
                  variant_dir='build')
env.Decider('MD5')
col = colorizer()
col.colorize(env)
doxygen.generate(env)
env.Doxygen("doc/Doxyfile")

#run configure
conf = Configure(env)
#for l in env['LIBS']:
#    if not conf.CheckLib('lib'+l+'.so'):
#        print "Library not found: lib"+l
#        Exit(0)
#for h in env.Split('cmath cstdlib iostream fstream sstream csignal string memory vector'):
#    if not conf.CheckCXXHeader(h):
#        print "C++ header not found: "+h
#        Exit(0)
#for h in env.Split('unistd.h GL/glew.h GLFW/glfw3.h GL/gl.h'):
#    if not conf.CheckHeader(h):
#        print "C header not found: "+h
#        Exit(0)

if not "release" in COMMAND_LINE_TARGETS:
    env['CXXFLAGS'] += ' -g'

#find source files recursively
srcFiles = []
for root,dirnames,filenames in os.walk('src'):
    for filename in fnmatch.filter(filenames,'*.cpp'):
        srcFiles.append(os.path.join(root, filename))

#build objects
objects = []
for f in srcFiles:
    objects.append(env.Object(f))

dinosauria = env.Program('bin/dinosauria',
                         objects)
env.AlwaysBuild(dinosauria)
env.Alias("release",dinosauria)
