cmake_minimum_required (VERSION 3.15)
project (Mandelbrot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Let CMake search for a raylib-config.cmake
find_package(raylib 3.0 QUIET)

# If there's none, fetch and build raylib
if (NOT raylib_FOUND)
include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/3.0.0.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games
    set(GRAPHICS GRAPHICS_API_OPENGL_21 CACHE STRING "" FORCE) # Linux only?
    # build raylib
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()
    
add_executable(Mandelbrot mandelbrot.cpp)
target_link_libraries(Mandelbrot raylib)
if(MSVC)
  target_compile_options(Mandelbrot PRIVATE "$<$<CONFIG:DEBUG>:/W4;/WX;/Od>")
  target_compile_options(Mandelbrot PRIVATE "$<$<CONFIG:RELEASE>:/W4;/WX;/Ox>")
else()
  target_link_libraries(Mandelbrot tbb)
  target_compile_options(Mandelbrot PRIVATE "$<$<CONFIG:DEBUG>:-Werror;-Wall;-Wextra;-pedantic;-O0;-g>")
  target_compile_options(Mandelbrot PRIVATE "$<$<CONFIG:RELEASE>:-Werror;-Wall;-Wextra;-pedantic;-O3>")
endif()
