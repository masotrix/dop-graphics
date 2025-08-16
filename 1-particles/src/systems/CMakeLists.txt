add_library(systems_lib STATIC
    movement_system.cpp
)

target_include_directories(systems_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src/data
)

target_link_libraries(systems_lib PUBLIC data_lib common_pch)

