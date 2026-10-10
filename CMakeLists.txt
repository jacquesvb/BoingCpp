cmake_minimum_required(VERSION 3.25)

project(boing LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#! ! ! ! ! ! !
#set this to ON instead of OFF to ship the game!
#basically this will change the RESOURCES_PATH to be the local path
#! ! ! ! ! ! !
set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)

# --- Provision raylib 6.0 if it is not already vendored -----------------------
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/raylib-6.0/CMakeLists.txt")
    message(STATUS "raylib 6.0 not found in thirdparty/ - downloading...")
    find_package(Git REQUIRED)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone
                --depth 1 --branch 6.0
                https://github.com/raysan5/raylib.git
                "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/raylib-6.0"
        RESULT_VARIABLE RAYLIB_CLONE_RESULT
        COMMAND_ECHO STDOUT)
    if(NOT RAYLIB_CLONE_RESULT EQUAL 0)
        message(FATAL_ERROR
            "Failed to download raylib 6.0. Check your network connection, "
            "or clone it manually into thirdparty/raylib-6.0.")
    endif()
endif()

# Build raylib as a static library and skip its examples.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Raylib as static libraries" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/raylib-6.0)

# MY_SOURCES is defined to be a list of all the source files for my game.
# DON'T ADD THE SOURCES BY HAND, they are added automatically with this glob.
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}")
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES})
target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(PRODUCTION_BUILD)
    # RESOURCES_PATH points at the resources folder copied next to the exe.
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE RESOURCES_PATH="./resources/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE PRODUCTION_BUILD=1)
else()
    # Useful for running from an IDE during development.
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE PRODUCTION_BUILD=0)
endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /arch:AVX2)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE _CRT_SECURE_NO_WARNINGS)

    if(PRODUCTION_BUILD)
        # Remove the console window.
        set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    endif()
else()
    target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE -mavx2 -mfma -Wall -Wextra)

    if(MINGW)
        if(PRODUCTION_BUILD)
            # Remove console on Windows under MinGW.
            target_link_options("${CMAKE_PROJECT_NAME}" PRIVATE -mwindows)
        endif()
    elseif(APPLE OR UNIX)
        if(PRODUCTION_BUILD)
            target_link_options("${CMAKE_PROJECT_NAME}" PRIVATE -s)
        endif()
    endif()
endif()

# Link raylib. Add other libraries here if needed.
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE raylib)

# Copy the resources next to the built binary so the game can also be launched
# directly from the build directory.
add_custom_command(
    TARGET "${CMAKE_PROJECT_NAME}" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/resources
    COMMENT "Copying resources to build directory"
)