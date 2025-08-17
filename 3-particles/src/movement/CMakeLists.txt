set(LIB_NAME movement_lib)
set(SOURCES
    movement.cpp
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(movement_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src/data
)

target_link_libraries(movement_lib PUBLIC data_lib common_pch)

