set(LIB_NAME particles_lib)
set(SOURCES
    create_particles.cpp
    move_particles.cpp
    render_particles.cpp
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src/data
)

target_link_libraries(${LIB_NAME} PUBLIC data_lib common_pch)

