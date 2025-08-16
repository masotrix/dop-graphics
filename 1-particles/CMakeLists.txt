cmake_minimum_required(VERSION 3.16)
project(DOP_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

#add_compile_options(-Wall -Wextra -Wpedantic)

# Carpeta de includes globales (opcional), para interfaz publica
#include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(common_pch INTERFACE)
target_precompile_headers(common_pch INTERFACE ${PROJECT_SOURCE_DIR}/src/pch.h)
target_include_directories(common_pch INTERFACE ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src/data)
add_subdirectory(src/systems)

add_executable(run src/main.cpp)
target_link_libraries(run PRIVATE data_lib systems_lib common_pch)
target_link_libraries(run PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_options(run PRIVATE
    -O3 -march=native -mavx2 -mfma -funroll-loops -DNDEBUG)
    #-O3 -march=native -mavx2 -mfma -funroll-loops -flto -DNDEBUG)

set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/assets)
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} ${CMAKE_BINARY_DIR}/assets)
add_dependencies(run copy_assets)


# Tests opcionales
#option(ENABLE_TESTS "Build tests" ON)
#if (ENABLE_TESTS)
#    enable_testing()
#    add_subdirectory(tests)
#endif()
