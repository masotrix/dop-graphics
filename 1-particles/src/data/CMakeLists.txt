add_library(data_lib INTERFACE
    #positions.h
    #positions.cpp
    #Velocity.cpp
)

target_include_directories(data_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(data_lib INTERFACE common_pch)

