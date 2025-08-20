set(LIB_NAME camera_lib)
set(SOURCES
    create_camera.cpp
    poll_event.cpp
)

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src/data
)

target_link_libraries(${LIB_NAME} PUBLIC data_lib common_pch)

