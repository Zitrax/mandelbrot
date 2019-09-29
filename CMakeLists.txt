cmake_minimum_required (VERSION 3.15)
project (Mandelbrot)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Let CMake search for a raylib-config.cmake
find_package(raylib 2.5 QUIET)

# If there's none, fetch and build raylib
if (NOT raylib_FOUND)
include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/2.5.0.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games
    # build raylib
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()
    
add_executable(Mandelbrot mandelbrot.cpp)
target_link_libraries(Mandelbrot raylib)
