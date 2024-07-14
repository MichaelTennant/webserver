cmake_minimum_required(VERSION 3.30.0)
project(WebServer VERSION 0.1)

if (!UNIX)
    message(WARNING "Project currently only supports unix based operating systems.")
endif()

add_executable(${PROJECT_NAME} src/main.c)

configure_file(project_metadata.h.in project_metadata.h @ONLY)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})
