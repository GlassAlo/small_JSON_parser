cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME parser_json)
project(${PROJECT_NAME})

set(SRC_FILES
    main.cpp
)
add_executable(${PROJECT_NAME} ${SRC_FILES})
