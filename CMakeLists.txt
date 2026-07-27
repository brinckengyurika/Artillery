cmake_minimum_required(VERSION 3.16)

project(Artillery VERSION 0.1)

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
)

target_include_directories(Artillery PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/tinygltf
)

target_link_libraries(Artillery
    OgreMain
    OgreHlmsUnlit
    OgreHlmsPbs
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
