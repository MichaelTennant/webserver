cmake_minimum_required(VERSION 3.30.0)
project(WebServer VERSION 0.1)

if(!UNIX)
    message(FATAL_ERROR "Project currently only supports unix based operating systems.")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Undefined behavour may occour when executing 32-bit project binaries.")
endif()

add_executable(${PROJECT_NAME} src/main.c)

configure_file(project_metadata.h.in project_metadata.h @ONLY)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})
