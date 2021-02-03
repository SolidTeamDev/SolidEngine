cmake_minimum_required(VERSION 3.17)

# ========= ENGINE ==========

project(SolidEngine)

set(CMAKE_CXX_STANDARD 20)

### Include dirs

include_directories(SolidEngine/Include)
include_directories(SolidEngine/ThirdParty/Include)

### Engine Sources Files

# Add source files
file(GLOB_RECURSE ENGINE_SOURCE_FILES
        SolidEngine/Src/*.c
        SolidEngine/Src/*.cpp)

# Add ThirdParty files
file(GLOB_RECURSE THIRD_PARTY_FILES
        SolidEngine/ThirdParty/Src/*.c
        SolidEngine/ThirdParty/Src/*.cpp)

#### Lib

#OPENGL
find_package(OpenGL REQUIRED)




#Extern Lib
link_directories(SolidEngine/ThirdParty/Lib)

set(ENGINE_LIBS PUBLIC glfw3 OpenGL::GL assimp-vc142-mtd IrrXML zlibstaticd)

add_library(SolidEngine SHARED ${ENGINE_SOURCE_FILES} ${THIRD_PARTY_FILES})

target_link_libraries(SolidEngine ${ENGINE_LIBS})

# ========= EDITOR ==========

project(SolidEditor)

### Include dirs

include_directories(SolidEditor/Include)
include_directories(SolidEngine/Include)

### Editor Sources Files

file(GLOB_RECURSE EDITOR_SOURCE_FILES
        SolidEditor/Src/*.c
        SolidEditor/Src/*.cpp
        $<TARGET_OBJECTS:SolidEngine>)

#### Lib

#OPENGL
find_package(OpenGL REQUIRED)

#Extern Lib
link_directories(SolidEngine/ThirdParty/Lib Build/${CMAKE_BUILD_TYPE})

set(EDITOR_LIBS PUBLIC glfw3 OpenGL::GL SolidEngine)

add_executable(SolidEditor ${EDITOR_SOURCE_FILES})
target_link_libraries(SolidEditor ${EDITOR_LIBS})



