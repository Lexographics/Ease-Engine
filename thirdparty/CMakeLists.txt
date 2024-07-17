if(NOT ${TARGET_PLATFORM} STREQUAL "Web")
    set(SOWA_THIRDPARTY_INCLUDES
        ${SOWA_THIRDPARTY_INCLUDES}
        "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.3.8/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/glad/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/freetype-2.13.2/include"
    )

    set(SOWA_THIRDPARTY_SOURCES
        ${SOWA_THIRDPARTY_SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c"
    )

    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_DOCS OFF)
    set(GLFW_INSTALL OFF)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.3.8" EXCLUDE_FROM_ALL)

    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/freetype-2.13.2" EXCLUDE_FROM_ALL)

    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/openal-soft" EXCLUDE_FROM_ALL)
endif()


set(SOWA_THIRDPARTY_INCLUDES
    ${SOWA_THIRDPARTY_INCLUDES}
    "${CMAKE_CURRENT_SOURCE_DIR}/glm-0.9.9.8/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/stb"
    "${CMAKE_CURRENT_SOURCE_DIR}/tiny_obj_loader"
    "${CMAKE_CURRENT_SOURCE_DIR}/utfcpp-4.0.5/source"
    "${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp-0.8.0/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends"
    "${CMAKE_CURRENT_SOURCE_DIR}/fmt-10.2.1/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/eventpp/include"

    "${CMAKE_CURRENT_SOURCE_DIR}/lua"
    "${CMAKE_CURRENT_SOURCE_DIR}/LuaBridge3/Distribution"
    "${CMAKE_CURRENT_SOURCE_DIR}/openal-soft/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/libsndfile/include"
)

set(SOWA_THIRDPARTY_SOURCES
    ${SOWA_THIRDPARTY_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lapi.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lauxlib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lbaselib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lcode.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lcode.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lcorolib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lctype.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lctype.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/ldblib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/ldebug.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/ldo.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/ldump.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lfunc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lfunc.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lgc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lgc.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/linit.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/liolib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/llex.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lmathlib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lmem.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/loadlib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lobject.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lobject.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lopcodes.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/loslib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lparser.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lstate.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lstring.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lstrlib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/ltable.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/ltablib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/ltests.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/ltm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lua.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lundump.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lutf8lib.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lvm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/lzio.c"
    # "${CMAKE_CURRENT_SOURCE_DIR}/lua/onelua.c"
)

set(BUILD_SHARED_LIBS OFF)


set(SKIP_PERFORMANCE_COMPARISON ON)
set(BUILD_DOC OFF)
set(BUILD_SANDBOX OFF)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp-0.8.0" EXCLUDE_FROM_ALL)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/fmt-10.2.1" EXCLUDE_FROM_ALL)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/eventpp" EXCLUDE_FROM_ALL)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libsndfile" EXCLUDE_FROM_ALL)


set(SOWA_THIRDPARTY_INCLUDES ${SOWA_THIRDPARTY_INCLUDES} PARENT_SCOPE)
set(SOWA_THIRDPARTY_SOURCES ${SOWA_THIRDPARTY_SOURCES} PARENT_SCOPE)