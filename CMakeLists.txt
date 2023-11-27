cmake_minimum_required(VERSION 3.26)
project(GenericObjectPoolWithExample)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
)

FetchContent_MakeAvailable(sfml)



# Copy the arial.ttf font file to the build directory
set(FONT_FILE "${CMAKE_SOURCE_DIR}/arial.ttf") # Path to the font file in the source directory
set(FONT_FILE_DEST "${CMAKE_BINARY_DIR}/arial.ttf") # Destination path in the build directory

if(EXISTS ${FONT_FILE})
    configure_file(${FONT_FILE} ${FONT_FILE_DEST} COPYONLY)
else()
    message(FATAL_ERROR "Font file not found: ${FONT_FILE}")
endif()

add_executable(GenericObjectPoolWithExample main.cpp GenericObjectPool.h Particle.h)
target_link_libraries(GenericObjectPoolWithExample sfml-graphics sfml-window sfml-system)