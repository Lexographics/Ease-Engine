cmake_minimum_required(VERSION 3.0.0)
project(
  Sowa
  VERSION 1.0
  LANGUAGES CXX C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

message("Building in ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "-O1 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOWA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
set(SOWA_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src/")

set(SOWA_THIRDPARTY_SOURCES "")
set(SOWA_THIRDPARTY_INCLUDES "")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty" EXCLUDE_FROM_ALL)

add_library(thirdparty ${SOWA_THIRDPARTY_SOURCES})
target_include_directories(thirdparty PRIVATE ${SOWA_THIRDPARTY_INCLUDES})

add_executable(sowa ${SOWA_SOURCES})
target_include_directories(
  sowa
  PUBLIC ${SOWA_INCLUDES}
  PRIVATE ${SOWA_THIRDPARTY_INCLUDES})

target_compile_options(sowa PRIVATE "-Werror")
target_link_libraries(
  sowa
  PRIVATE
    thirdparty
    glfw
)
