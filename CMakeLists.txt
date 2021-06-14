
cmake_minimum_required(VERSION 3.17)

# ========= ENGINE ==========

project(SolidEngine)

set(CMAKE_CXX_STANDARD 20)

### Include dirs

include_directories(SolidEngine/Include)
include_directories(SolidEngine/ThirdParty/Include)
include_directories(SolidEngine/ThirdParty/Include/Physx)
include_directories(SolidEngine/ThirdParty/Include/FMOD)
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
target_compile_definitions(SolidEngine PUBLIC "SOLID_COMPILED_STATE=\"${CMAKE_BUILD_TYPE}\"")

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

add_executable(SolidEditor ${EDITOR_SOURCE_FILES} Resources/icon.rc)
target_link_libraries(SolidEditor ${EDITOR_LIBS})
target_compile_definitions(SolidEditor PUBLIC "IMGUI_API=__declspec(dllimport)")
target_compile_definitions(SolidEditor PUBLIC "SOLID_API=__declspec(dllimport)")
target_compile_definitions(SolidEditor PRIVATE "SOLID_EDITOR_API=__declspec(dllexport)")



add_custom_target(RunGenerator
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND SolidEngine/ThirdParty/Bin/RefurekuGenerator
        )

add_dependencies(SolidEngine RunGenerator)

# ========= LAUNCHER ==========

project(SolidLauncher)

### Include dirs

include_directories(SolidLauncher/Include)
include_directories(SolidEngine/Include)

### Editor Sources Files

file(GLOB_RECURSE LAUNCHER_SOURCE_FILES
        SolidLauncher/Src/*.c
        SolidLauncher/Src/*.cpp
        $<TARGET_OBJECTS:SolidEngine>)

#### Lib

#OPENGL
find_package(OpenGL REQUIRED)

#Extern Lib
link_directories(SolidEngine/ThirdParty/Lib Build/${CMAKE_BUILD_TYPE})

set(LAUNCHER_LIBS PUBLIC glfw3dll OpenGL::GL SolidEngine)

add_executable(SolidLauncher ${LAUNCHER_SOURCE_FILES} Resources/icon.rc)
target_link_libraries(SolidLauncher ${LAUNCHER_LIBS})
target_compile_definitions(SolidLauncher PUBLIC "IMGUI_API=__declspec(dllimport)")
target_compile_definitions(SolidLauncher PUBLIC "SOLID_API=__declspec(dllimport)")
target_compile_definitions(SolidLauncher PRIVATE "SOLID_EDITOR_API=__declspec(dllexport)")




add_dependencies(SolidEditor SolidEngine)



# ========= DLL ==========

#COMMON
ADD_CUSTOM_COMMAND (TARGET SolidEditor POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Bin/glfw3.dll ${CMAKE_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Bin/fmod.dll ${CMAKE_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Bin/fmodL.dll ${CMAKE_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Bin/fmodstudio.dll ${CMAKE_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Bin/fmodstudioL.dll ${CMAKE_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/SolidEngine.dll ${CMAKE_BINARY_DIR}/Dlls/SolidEngine.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/SolidEngine.lib ${CMAKE_BINARY_DIR}/Dlls/SolidEngine.lib
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

COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/EditorAssets ${CMAKE_BINARY_DIR}/EditorAssets
# Output Message
COMMENT "Copying Resources folder" VERBATIM)

# ========= INCLUDES ==========
ADD_CUSTOM_COMMAND (TARGET SolidEngine POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/SolidEngine/Include ${CMAKE_BINARY_DIR}/Include
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Include ${CMAKE_BINARY_DIR}/Include
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Lib ${CMAKE_BINARY_DIR}/Dlls
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Bin ${CMAKE_BINARY_DIR}/Dlls
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/SolidEngine/ThirdParty/Include ${CMAKE_BINARY_DIR}/Include
        # Output Message
        COMMENT "Copying Includes folder" VERBATIM
        )



# ========= LAYOUT ==========


ADD_CUSTOM_COMMAND (TARGET SolidEditor POST_BUILD

COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/imgui.ini ${CMAKE_BINARY_DIR}
# Output Message
COMMENT "Copying Default Layout" VERBATIM)
