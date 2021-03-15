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

set(ENGINE_LIBS PUBLIC glfw3dll OpenGL::GL assimp-vc142-mtd IrrXML zlibstaticd)

add_library(SolidEngine SHARED ${ENGINE_SOURCE_FILES} ${THIRD_PARTY_FILES})

target_link_libraries(SolidEngine ${ENGINE_LIBS})
target_compile_definitions(SolidEngine PRIVATE "IMGUI_API=__declspec(dllexport)")
target_compile_definitions(SolidEngine PUBLIC "SOLID_API=__declspec(dllexport)")

# ========= EDITOR ==========

project(SolidEditor)

### Include dirs

include_directories(SolidEditor/Include)
include_directories(SolidEngine/Include)
include_directories(SolidEditor/ThirdParty/Include)

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

set(EDITOR_LIBS PUBLIC glfw3dll OpenGL::GL SolidEngine)

add_executable(SolidEditor ${EDITOR_SOURCE_FILES})
target_link_libraries(SolidEditor ${EDITOR_LIBS})
target_compile_definitions(SolidEditor PRIVATE "IMGUI_API=__declspec(dllimport)")
target_compile_definitions(SolidEditor PUBLIC "SOLID_API=__declspec(dllimport)")
target_compile_definitions(SolidEditor PRIVATE "SOLID_EDITOR_API=__declspec(dllexport)")

#Refureku
target_link_directories(SolidEditor PRIVATE SolidEditor/ThirdParty/Lib)
target_link_libraries(SolidEditor PRIVATE $<IF:$<CONFIG:Debug>,Refurekud,Refureku>)

add_custom_target(RunGenerator
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND SolidEditor/ThirdParty/Bin/RefurekuGenerator
        )

add_dependencies(SolidEditor RunGenerator)


# ========= DLL ==========

ADD_CUSTOM_COMMAND (TARGET SolidEditor POST_BUILD

COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Lib/glfw3.dll ${CMAKE_BINARY_DIR}/
COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Lib/assimp-vc142-mtd.dll ${CMAKE_BINARY_DIR}/
# Output Message
COMMENT "Copying binaries" VERBATIM)

# ========= RESOURCES ==========

ADD_CUSTOM_COMMAND (TARGET SolidEditor POST_BUILD

COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Resources ${CMAKE_BINARY_DIR}/Resources
# Output Message
COMMENT "Copying Resources folder" VERBATIM)

# ========= LAYOUT ==========
ADD_CUSTOM_COMMAND (TARGET SolidEditor POST_BUILD

COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/imgui.ini ${CMAKE_BINARY_DIR}
# Output Message
COMMENT "Copying Default Layout" VERBATIM)