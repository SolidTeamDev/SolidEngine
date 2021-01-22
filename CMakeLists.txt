cmake_minimum_required(VERSION 3.17)

project(SolidEngine)

set(CMAKE_CXX_STANDARD 20)

### Include dirs

include_directories(Include)
include_directories(ThirdParty/Include)

### Sources Files

# Add source files
file(GLOB_RECURSE SOURCE_FILES
        Src/*.c
        Src/*.cpp)

# Add ThirdParty files
file(GLOB_RECURSE THIRD_PARTY_FILES
        ThirdParty/Src/*.c
        ThirdParty/Src/*.cpp)

#### Lib

#OPENGL
find_package(OpenGL REQUIRED)

#Extern Lib
link_directories(ThirdParty/Lib)

set(LIBS PUBLIC glfw3 OpenGL::GL)

add_executable(SolidEngine ${SOURCE_FILES} ${THIRD_PARTY_FILES})
target_link_libraries(SolidEngine ${LIBS})