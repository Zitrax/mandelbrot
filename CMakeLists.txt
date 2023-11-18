cmake_minimum_required(VERSION 3.15)
project(Mandelbrot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)

add_executable(Mandelbrot mandelbrot.cpp)

target_link_libraries(Mandelbrot raylib)

if(MSVC)
  target_compile_options(Mandelbrot PRIVATE "$<$<CONFIG:DEBUG>:/W4;/WX;/Od>")
  target_compile_options(Mandelbrot PRIVATE "$<$<CONFIG:RELEASE>:/W4;/WX;/Ox>")
else()
  target_link_libraries(Mandelbrot TBB::tbb)
  target_compile_options(Mandelbrot PRIVATE "$<$<CONFIG:DEBUG>:-Werror;-Wall;-Wextra;-pedantic;-O0;-g>")
  target_compile_options(Mandelbrot PRIVATE "$<$<CONFIG:RELEASE>:-Werror;-Wall;-Wextra;-pedantic;-O3>")
endif()
