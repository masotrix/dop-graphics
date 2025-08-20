set(LIB_NAME light_lib)
set(SOURCES
    create_light.cpp
    move_light.cpp
    render_light.cpp
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src/data
)

target_link_libraries(${LIB_NAME} PUBLIC data_lib common_pch)

