cmake_minimum_required(VERSION 3.16)
project(DOP_Project LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Buscar librerias externas ---
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
add_library(sqlite3 STATIC third_party/sqlite3.c)
set_target_properties(sqlite3 PROPERTIES LINKER_LANGUAGE C)
target_include_directories(sqlite3 PUBLIC third_party)

# --- Opciones de compilación ---
#add_compile_options(-Wall -Wextra -Wpedantic)

# --- Precompiled headers (PCH) ---
add_library(common_pch INTERFACE)
target_precompile_headers(common_pch INTERFACE ${PROJECT_SOURCE_DIR}/src/pch.h)
target_include_directories(common_pch INTERFACE ${PROJECT_SOURCE_DIR}/src)

# --- Subdirectorios ---
add_subdirectory(src/data)
add_subdirectory(src/particles)
add_subdirectory(src/light)
add_subdirectory(src/database)
add_subdirectory(src/camera)

# --- Ejecutable principal ---
add_executable(run src/main.cpp)

# --- Enlazar bibliotecas ---
target_link_libraries(run PRIVATE
    data_lib
    particles_lib
    light_lib
    database_lib
    camera_lib
    common_pch
    sfml-graphics sfml-window sfml-system
    sqlite3
 )

# --- Optimizaciones ---
target_compile_options(run PRIVATE
    -O3 -march=native -mavx2 -mfma -funroll-loops -DNDEBUG)
    #-O3 -march=native -mavx2 -mfma -funroll-loops -flto -DNDEBUG)


# --- Copiar assets ---
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
