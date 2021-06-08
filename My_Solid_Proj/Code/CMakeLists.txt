cmake_minimum_required(VERSION 3.17)
project(My_Solid_Proj)

set(CMAKE_CXX_STANDARD 20)
### Include dirs

include_directories(Include)
include_directories(${PROJECT_SOURCE_DIR}/../EngineInclude)
include_directories(${PROJECT_SOURCE_DIR}/../EngineInclude/Physx)
### Engine Sources Files
#OPENGL
find_package(OpenGL REQUIRED)

# Add source files
file(GLOB_RECURSE GAME_SOURCE_FILES
        Src/*.c
        Src/*.cpp)
file(GLOB LIBS_COMMON
        Dlls/*.lib)
file(GLOB LIBS_DEBUG
        Dlls/Debug/*.lib)
file(GLOB LIBS_RELEASE
        Dlls/Release/*.lib)

link_directories(Dlls)
#add_executable(My_Solid_Proj ${GAME_SOURCE_FILES})
add_library(My_Solid_Proj SHARED ${GAME_SOURCE_FILES})
target_compile_definitions(My_Solid_Proj PUBLIC "IMGUI_API=__declspec(dllimport)")
target_compile_definitions(My_Solid_Proj PUBLIC "SOLID_API=__declspec(dllimport)")
target_compile_definitions(My_Solid_Proj PUBLIC "SOLID_SCRIPT_API=__declspec(dllexport)")#Extern Lib

if(CMAKE_BUILD_TYPE MATCHES Debug)#DEBUG
target_link_libraries(My_Solid_Proj PUBLIC
${LIBS_COMMON}
debug     ${LIBS_DEBUG} )
else()#Release/Other
target_link_libraries(My_Solid_Proj PUBLIC
${LIBS_COMMON}
optimized ${LIBS_RELEASE})
endif()
target_link_libraries(My_Solid_Proj ${GAME_LIBS})
# ========= DLL ==========

#COMMON
ADD_CUSTOM_COMMAND (TARGET My_Solid_Proj POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Dlls/glfw3.dll ${CMAKE_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Dlls/OpenAL32.dll ${CMAKE_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Dlls/sndfile.dll ${CMAKE_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Dlls/SolidEngine.dll ${CMAKE_BINARY_DIR}/

COMMENT "Copying common binaries" VERBATIM)

if(CMAKE_BUILD_TYPE MATCHES Debug)#DEBUG
    ADD_CUSTOM_COMMAND (TARGET My_Solid_Proj POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Dlls/Debug ${CMAKE_BINARY_DIR}/

    COMMENT "Copying debug binaries" VERBATIM)
else()#RELEASE/OTHER

    ADD_CUSTOM_COMMAND (TARGET My_Solid_Proj POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Dlls/Release ${CMAKE_BINARY_DIR}/

    COMMENT "Copying release binaries" VERBATIM)
endif()
add_custom_target(RunGenerator
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND Dlls/RefurekuGenerator
        )

add_dependencies(My_Solid_Proj RunGenerator)
project(Game)
include_directories(Include)
include_directories(${PROJECT_SOURCE_DIR}/../EngineInclude)
include_directories(${PROJECT_SOURCE_DIR}/../EngineInclude/Physx)
set(Game_libs PUBLIC glfw3dll OpenGL::GL SolidEngine)
link_directories(Dlls)
add_executable(Game main.cpp)
target_link_libraries(Game ${Game_libs})
target_compile_definitions(Game PUBLIC "IMGUI_API=__declspec(dllimport)")
target_compile_definitions(Game PUBLIC "SOLID_API=__declspec(dllimport)")
if(CMAKE_BUILD_TYPE MATCHES Debug)#DEBUG
target_link_libraries(Game PUBLIC
${LIBS_COMMON}
debug     ${LIBS_DEBUG} )
else()#Release/Other
target_link_libraries(Game PUBLIC
${LIBS_COMMON}
optimized ${LIBS_RELEASE})
endif()




