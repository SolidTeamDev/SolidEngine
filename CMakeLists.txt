
cmake_minimum_required(VERSION 3.17)

# ========= ENGINE ==========

project(SolidEngine)

set(CMAKE_CXX_STANDARD 20)

### Include dirs

include_directories(SolidEngine/Include)
include_directories(SolidEngine/ThirdParty/Include)
include_directories(SolidEngine/ThirdParty/Include/Physx)

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
file(GLOB ENGINE_LIBS_COMMON
        SolidEngine/ThirdParty/Lib/*.lib)
file(GLOB ENGINE_LIBS_DEBUG
        SolidEngine/ThirdParty/Lib/Debug/*.lib)
file(GLOB ENGINE_LIBS_RELEASE
        SolidEngine/ThirdParty/Lib/Release/*.lib)

#set(ENGINE_LIBS PUBLIC glfw3dll OpenGL::GL assimp-vc140-mtd IrrXML zlibstaticd)

add_library(SolidEngine SHARED ${ENGINE_SOURCE_FILES} ${THIRD_PARTY_FILES})






#Extern Lib
link_directories(SolidEngine/ThirdParty/Lib)
if(CMAKE_BUILD_TYPE MATCHES Debug)#DEBUG
    target_link_libraries(SolidEngine PUBLIC
        ${ENGINE_LIBS_COMMON}
        debug     ${ENGINE_LIBS_DEBUG} )
else()#Release/Other
    target_link_libraries(SolidEngine PUBLIC
            ${ENGINE_LIBS_COMMON}
            optimized ${ENGINE_LIBS_RELEASE})
endif()

target_compile_definitions(SolidEngine PUBLIC "IMGUI_API=__declspec(dllexport)")
target_compile_definitions(SolidEngine PUBLIC "SOLID_API=__declspec(dllexport)")

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

set(EDITOR_LIBS PUBLIC glfw3dll OpenGL::GL SolidEngine)

add_executable(SolidEditor ${EDITOR_SOURCE_FILES})
target_link_libraries(SolidEditor ${EDITOR_LIBS})
target_compile_definitions(SolidEditor PUBLIC "IMGUI_API=__declspec(dllimport)")
target_compile_definitions(SolidEditor PUBLIC "SOLID_API=__declspec(dllimport)")
target_compile_definitions(SolidEditor PRIVATE "SOLID_EDITOR_API=__declspec(dllexport)")



add_custom_target(RunGenerator
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND SolidEngine/ThirdParty/Bin/RefurekuGenerator
        )

add_dependencies(SolidEngine RunGenerator)


# ========= DLL ==========

#COMMON
ADD_CUSTOM_COMMAND (TARGET SolidEditor POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Bin/glfw3.dll ${CMAKE_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Bin/OpenAL32.dll ${CMAKE_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Bin/sndfile.dll ${CMAKE_BINARY_DIR}/

COMMENT "Copying common binaries" VERBATIM)

if(CMAKE_BUILD_TYPE MATCHES Debug)#DEBUG
    ADD_CUSTOM_COMMAND (TARGET SolidEditor POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Bin/Debug ${CMAKE_BINARY_DIR}/

    COMMENT "Copying debug binaries" VERBATIM)
else()#RELEASE/OTHER

    ADD_CUSTOM_COMMAND (TARGET SolidEditor POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Bin/Release ${CMAKE_BINARY_DIR}/

    COMMENT "Copying release binaries" VERBATIM)
endif()


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
