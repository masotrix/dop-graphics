cmake_minimum_required(VERSION 3.16)
project(DataOrientedGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opcional: modo Debug o Release por defecto
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Añadir SFML (requiere que SFML esté instalado en el sistema)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Incluir directorios para headers
#include_directories(
#  ${PROJECT_SOURCE_DIR} 
#  ${PROJECT_SOURCE_DIR}/data
#  ${PROJECT_SOURCE_DIR}/systems
#)

# Agrupar todos los archivos fuente automáticamente
file(GLOB_RECURSE SOURCES
  "${PROJECT_SOURCE_DIR}/main.cpp"
)

# Ejecutable principal
add_executable(${PROJECT_NAME} ${SOURCES})

# Linkear SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Opcional: organiza en carpetas dentro del IDE (Visual Studio, etc)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES})
