cmake_minimum_required(VERSION 3.16)

project(Artillery VERSION 0.1)
set(CMAKE_CODEBLOCKS_EXCLUDE_EXTERNAL_FILES ON)


find_package(TinyGLTF REQUIRED)
find_package(SDL2 REQUIRED)
find_package(X11 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OGRE_HOME "$ENV{HOME}/Projects/Ogre2/install")


include_directories(
    ${OGRE_HOME}/include
    ${OGRE_HOME}/include/OGRE

    ${OGRE_HOME}/include/OGRE/Hlms
    ${OGRE_HOME}/include/OGRE/Hlms/Common
    ${OGRE_HOME}/include/OGRE/Hlms/Unlit
    ${OGRE_HOME}/include/OGRE/Hlms/Pbs
)

link_directories(
    ${OGRE_HOME}/lib
)

add_executable(Artillery
    src/main.cpp
    src/Engine.cpp
    src/Renderer.cpp
    src/ResourceManager.cpp
    src/Scene.cpp
    src/MeshFactory.cpp
    src/CameraController.cpp
    src/GltfLoader.cpp
    src/GltfMeshBuilder.cpp
    src/TinyGltfImplementation.cpp
    src/InputManager.cpp
)

target_include_directories(Artillery PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    /usr/include/stb
    ${SDL2_INCLUDE_DIRS}
    ${X11_INCLUDE_DIR}
)

target_link_libraries(Artillery
    OgreMain
    OgreHlmsUnlit
    OgreHlmsPbs
    TinyGLTF::TinyGLTF
    SDL2::SDL2
    #    TinyGLTF
    ${X11_LIBRARIES}
)
# Bin könyvtár
set_target_properties(Artillery PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_compile_definitions(Artillery PRIVATE
    ARTILLERY_CFG_DIR="${CMAKE_SOURCE_DIR}/cfg"
    ARTILLERY_MEDIA_DIR="${CMAKE_SOURCE_DIR}/media"
)

# cfg átmásolása
file(COPY ${CMAKE_SOURCE_DIR}/cfg
     DESTINATION ${CMAKE_BINARY_DIR}/bin)

# media átmásolása
file(COPY ${CMAKE_SOURCE_DIR}/media
     DESTINATION ${CMAKE_BINARY_DIR}/bin)



if(CMAKE_HOST_UNIX AND CMAKE_GENERATOR MATCHES "CodeBlocks")
    find_program(SED_PROGRAM sed)
    if(SED_PROGRAM)
        # Megkeressük a pontos abszolút útvonalat
        set(ABS_PATH "${CMAKE_SOURCE_DIR}")

        # 1. parancs: A projekt mappára mutató hivatkozások cseréje
        # 2. parancs: Bármilyen egyéb, a felhasználói mappára maradt hivatkozás tisztítása
        execute_process(
            COMMAND sh -c "${SED_PROGRAM} -i 's|${ABS_PATH}/|../|g; s|${ABS_PATH}|..|g' *.cbp"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Minden abszolút hivatkozás kényszerített tisztítása..."
        )
    endif()
endif()

