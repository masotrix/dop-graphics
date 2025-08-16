cmake_minimum_required(VERSION 3.16)
project(DataOrientedGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opcional: modo Debug o Release por defecto
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Añadir SFML (requiere que SFML esté instalado en el sistema)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Incluir directorios para headers
#include_directories(
#  ${PROJECT_SOURCE_DIR} 
#  ${PROJECT_SOURCE_DIR}/data
#  ${PROJECT_SOURCE_DIR}/systems
#)

# Agrupar todos los archivos fuente automáticamente
#file(GLOB_RECURSE SOURCES
#  "${PROJECT_SOURCE_DIR}/*.cpp"
#)
set(SOURCES
  "${PROJECT_SOURCE_DIR}/main.cpp"
)

# Ejecutable principal
#add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(run ${SOURCES})

# Linkear SFML
#target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
target_link_libraries(run sfml-graphics sfml-window sfml-system)


# Optimización para Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  #target_compile_options(${PROJECT_NAME} PRIVATE
  target_compile_options(run PRIVATE
    -O3
    -march=native
    -mavx2
    -mfma
    -funroll-loops
    -flto              # link time optimization
    -DNDEBUG           # deshabilitar assert
  )
endif()

target_compile_features(run PRIVATE cxx_std_17)

# Opcional: organiza en carpetas dentro del IDE (Visual Studio, etc)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES})
