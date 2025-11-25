# Pong with C
This is the pong game built using C and the raylib library.

## Raylib Quickstart
This project uses raylib-quickstart which is a simple cross platform template 
for setting up a project with the bleeding edge raylib code.
More info (here)[https://github.com/raylib-extras/raylib-quickstart/tree/main]

# Building instructions

## Linux Users
* CD into the build folder
* run `./premake5 gmake`
* CD back to the root
* run `make`
* you are good to go

## MacOS Users
* CD into the build folder
* run `./premake5.osx gmake`
* CD back to the root
* run `make`
* you are good to go

## Output files
The built code will be in the bin dir

# Building for other OpenGL targets
If you need to build for a different OpenGL version than the default (OpenGL 3.3) you can specify an OpenGL version in your premake command line. Just modify the bat file or add the following to your command line

## For OpenGL 1.1
`--graphics=opengl11`

## For OpenGL 2.1
`--graphics=opengl21`

## For OpenGL 4.3
`--graphics=opengl43`

## For OpenGLES 2.0
`--graphics=opengles2`

## For OpenGLES 3.0
`--graphics=opengles3`

