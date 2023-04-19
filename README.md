# ikan
A Complete package for rendering. An Engine(ikan) to support render API. Editor to use create application using the 'ikan' Engine

![](/resources/logo/i_kan.png)

## Getting Started
XCode 13 is recommended

You can clone the repository to a local destination using git:

`git clone --recursive https://github.com/ashish1009/ikan`

Make sure that you do a `--recursive` clone to fetch all of the submodules!

## How to Use the library
### Library
`{...} directory where ikan Engine is cloned`
Link the libikan.a in your project... 
Link the libglfw.a in your project... 

Link ${PROJECT_DIR}/../ikan/vendors/freetype/lib/libfreetype.6.dylib

### Copy the libraried from following paths
Link ${PROJECT_DIR}/../ikan/vendors/freetype/lib/libfreetype.6.dylib
Link ${PROJECT_DIR}/../ikan/vendors/freetype/libpng/1.6.39/lib/libpng16.16.dylib

### Frameworks

### Header Search Paths
Add the following header search paths 
`{...} directory where ikan Engine is cloned`
${PROJECT_DIR}/../ikan/interface
${PROJECT_DIR}/../ikan/vendors/spdlog/include
${PROJECT_DIR}/../ikan/vendors/imgui/imgui
${PROJECT_DIR}/../ikan/vendors/glm
${PROJECT_DIR}/../ikan/vendors/freetype/include/freetype2
${PROJECT_DIR}/../ikan/vendors/entt/include
${PROJECT_DIR}/../ikan/vendors/box_2d/box_2d/include

### Library Search Paths
${PROJECT_DIR}/../ikan/vendors/freetype/lib

### Preprocesor
- Add `IK_DEBUG_FEATURE` to enable Kreator Debug features
- Add `IK_ENABLE_LOG` to enable Logging

## Basic APIs

## Features

## Road Map  
