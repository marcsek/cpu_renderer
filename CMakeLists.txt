cmake_minimum_required(VERSION 3.14.4)

# generate compile_command.json for IDE
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
    
project(graphics C)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")

set(SrcVendor
  vendors/stb/stb_include.c
  vendors/stb/stb_image.h
)

add_subdirectory(dependencies/minifb)
add_subdirectory(dependencies/notstd)

add_executable(${PROJECT_NAME} ${SOURCES} ${SrcVendor})

# link dependencies
target_link_libraries(${PROJECT_NAME} minifb)
target_link_libraries(${PROJECT_NAME} notstd)
