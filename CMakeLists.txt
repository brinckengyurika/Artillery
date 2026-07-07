cmake_minimum_required(VERSION 3.16)

project(Artillery VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OGRE_HOME "$ENV{HOME}/Projects/Ogre2/install")

include_directories(
    ${OGRE_HOME}/include
    ${OGRE_HOME}/include/OGRE
)

link_directories(
    ${OGRE_HOME}/lib
)

add_executable(Artillery
    src/main.cpp
    src/Engine.cpp
)

target_include_directories(Artillery PRIVATE src)

target_link_libraries(Artillery
    OgreMain
)

# Bin könyvtár
set_target_properties(Artillery PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# cfg átmásolása
file(COPY ${CMAKE_SOURCE_DIR}/cfg
     DESTINATION ${CMAKE_BINARY_DIR}/bin)

# media átmásolása
file(COPY ${CMAKE_SOURCE_DIR}/media
     DESTINATION ${CMAKE_BINARY_DIR}/bin)
