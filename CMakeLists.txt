cmake_minimum_required(VERSION 3.16)

project(Artillery VERSION 0.1)

set(CMAKE_CODEBLOCKS_EXCLUDE_EXTERNAL_FILES ON)

find_package(TinyGLTF REQUIRED)
find_package(SDL2 REQUIRED)
find_package(X11 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ============================================================
# OGRE 2
# ============================================================

set(OGRE_HOME "$ENV{HOME}/Projects/Ogre2/install")

set(OGRE_SOURCE_DIR
    "$ENV{HOME}/Projects/Ogre2/source"
)

# ============================================================
# Terra
# ============================================================

set(TERRA_SOURCE_DIR
    "${OGRE_SOURCE_DIR}/Samples/2.0/Tutorials/Tutorial_Terrain"
)


# ============================================================
# Include directories
# ============================================================

include_directories(
    ${OGRE_HOME}/include
    ${OGRE_HOME}/include/OGRE

    ${OGRE_HOME}/include/OGRE/Hlms
    ${OGRE_HOME}/include/OGRE/Hlms/Common
    ${OGRE_HOME}/include/OGRE/Hlms/Unlit
    ${OGRE_HOME}/include/OGRE/Hlms/Pbs

    # Terra
    ${TERRA_SOURCE_DIR}/include
)


# ============================================================
# OGRE libraries
# ============================================================

link_directories(
    ${OGRE_HOME}/lib
)


# ============================================================
# Terra source files
# ============================================================

set(TERRA_SOURCE_FILES

    ${TERRA_SOURCE_DIR}/src/Terra/Terra.cpp
    ${TERRA_SOURCE_DIR}/src/Terra/TerrainCell.cpp
    ${TERRA_SOURCE_DIR}/src/Terra/TerraShadowMapper.cpp
    ${TERRA_SOURCE_DIR}/src/Terra/TerraWorkspaceListener.cpp

    ${TERRA_SOURCE_DIR}/src/Terra/Hlms/OgreHlmsTerra.cpp
    ${TERRA_SOURCE_DIR}/src/Terra/Hlms/OgreHlmsTerraDatablock.cpp
    ${TERRA_SOURCE_DIR}/src/Terra/Hlms/OgreHlmsJsonTerra.cpp

    ${TERRA_SOURCE_DIR}/src/Terra/Hlms/PbsListener/OgreHlmsPbsTerraShadows.cpp
)



# ============================================================
# Executable
# ============================================================

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
    src/Input/LinuxInputBackend.cpp

    # Terra
    ${TERRA_SOURCE_FILES}
)


# ============================================================
# Target include directories
# ============================================================

target_include_directories(Artillery PRIVATE

    ${PROJECT_SOURCE_DIR}/src

    /usr/include/stb

    ${SDL2_INCLUDE_DIRS}

    ${X11_INCLUDE_DIR}

    # Terra
    ${TERRA_SOURCE_DIR}/include
)


# ============================================================
# Libraries
# ============================================================

target_link_libraries(Artillery

    OgreMain
    OgreHlmsUnlit
    OgreHlmsPbs

    TinyGLTF::TinyGLTF

    SDL2::SDL2

    ${X11_LIBRARIES}

    Xi
)







# ============================================================
# Binary output
# ============================================================

set_target_properties(Artillery PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)


# ============================================================
# Compile definitions
# ============================================================

target_compile_definitions(Artillery PRIVATE

    ARTILLERY_CFG_DIR="${CMAKE_SOURCE_DIR}/cfg"

    ARTILLERY_MEDIA_DIR="${CMAKE_SOURCE_DIR}/media"
)


# ============================================================
# Copy cfg
# ============================================================

file(COPY
    ${CMAKE_SOURCE_DIR}/cfg
    DESTINATION
    ${CMAKE_BINARY_DIR}/bin
)


# ============================================================
# Copy media
# ============================================================

file(COPY
    ${CMAKE_SOURCE_DIR}/media
    DESTINATION
    ${CMAKE_BINARY_DIR}/bin
)


# ============================================================
# Code::Blocks path cleanup
# ============================================================

if(CMAKE_HOST_UNIX AND CMAKE_GENERATOR MATCHES "CodeBlocks")

    find_program(SED_PROGRAM sed)

    if(SED_PROGRAM)

        set(ABS_PATH "${CMAKE_SOURCE_DIR}")

        execute_process(
            COMMAND sh -c
            "${SED_PROGRAM} -i 's|${ABS_PATH}/|../|g; s|${ABS_PATH}|..|g' *.cbp"

            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"

            COMMENT
            "Minden abszolút hivatkozás kényszerített tisztítása..."
        )

    endif()

endif()
