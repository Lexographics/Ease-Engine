cmake_minimum_required(VERSION 3.0.0)
project(
  Sowa
  VERSION 1.0
  LANGUAGES CXX C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

message("Building in ${CMAKE_BUILD_TYPE}")
message("Platform: ${CMAKE_SYSTEM_NAME}")
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(TARGET_PLATFORM "Linux")
  add_definitions(-DSW_LINUX)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
  set(TARGET_PLATFORM "Web")
  add_definitions(-DSW_WEB)
else()
  message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()


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

if(${TARGET_PLATFORM} STREQUAL "Web")
  add_compile_options("-s" "USE_FREETYPE=1")
endif()

add_executable(sowa ${SOWA_SOURCES})
target_include_directories(
  sowa
  PUBLIC ${SOWA_INCLUDES}
)
  
target_include_directories(
  sowa
  SYSTEM
  PUBLIC ${SOWA_THIRDPARTY_INCLUDES}
)

# target_compile_options(sowa PRIVATE "-Werror")
target_link_libraries( sowa PUBLIC thirdparty PRIVATE freetype yaml-cpp fmt sndfile )
if(NOT ${TARGET_PLATFORM} STREQUAL "Web")
  target_link_libraries( sowa PRIVATE glfw OpenAL )
endif()


if(${TARGET_PLATFORM} STREQUAL "Web")
  set_target_properties(sowa
    PROPERTIES
      OUTPUT_NAME "index"
      SUFFIX ".html"
      LINK_FLAGS
      # " --bind -sUSE_GLFW=3 -sWASM=1 -sFULL_ES3=1 --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/res/@res --shell-file ${CMAKE_SOURCE_DIR}/layout.html"
      " --bind -sUSE_GLFW=3 -sWASM=1 -sFULL_ES3=1 -sASSERTIONS -sALLOW_MEMORY_GROWTH --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/res/@res --shell-file ${CMAKE_SOURCE_DIR}/layout.html"
      # "-sEXPORTED_FUNCTIONS=_main -sEXPORTED_RUNTIME_METHODS=ccall,cwrap -lidbfs.js -sASYNCIFY=1 -sASSERTIONS -sASYNCIFY_STACK_SIZE=16384"
      )
endif()