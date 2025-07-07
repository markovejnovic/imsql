cmake_minimum_required(VERSION 3.28)
project(imsql LANGUAGES C CXX OBJCXX)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_OBJCXX_STANDARD 23)
set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)
set(SANITIZER_FLAGS "-fsanitize=address,undefined" "-fno-omit-frame-pointer")

find_package(glfw3 REQUIRED)
find_package(Boost REQUIRED serialization)
add_subdirectory(third_party)

add_executable(imsql)
#target_sources(imsql PRIVATE
#  bin/main.cpp
#)
target_sources(imsql PRIVATE
  FILE_SET modules TYPE CXX_MODULES FILES
  src/models/db.cpp
)
target_link_libraries(imsql PRIVATE
  imgui
  imnodes
  SQLiteCpp
  ${Boost_LIBRARIES}
)

#
## Set language standards
#
## Force static library default
#
#add_subdirectory(third_party)
#
#find_package(glfw3 REQUIRED)
#find_package(Boost REQUIRED serialization)
#
## Build the imsql library
#add_library(imsql_lib STATIC
#    lib/src/os/platform/macos.mm
#)
#target_link_libraries(imsql_lib PUBLIC
#  Microsoft.GSL::GSL imgui glfw
#)
#
#target_include_directories(imsql_lib PUBLIC lib/include)
#
#target_compile_options(imsql_lib PRIVATE
#    -std=c++23
#    -DIMGUI_IMPL_METAL_CPP
#    -DIMGUI_DEFINE_MATH_OPERATORS
#    -ftime-trace
#)
#
#target_link_libraries(imsql_lib PUBLIC
#    glfw
#    metal_cpp
#    imgui
#    imnodes
#    SQLiteCpp
#    ${Boost_LIBRARIES}
#)
#
## Build the executable
#add_executable(imsql bin/main.cpp)
#
#target_compile_options(imsql PRIVATE
#    -DIMGUI_IMPL_METAL_CPP
#    -DIMGUI_DEFINE_MATH_OPERATORS
#    -std=c++23
#    -Og
#    -g
#    -ftime-trace
#)
#target_include_directories(imsql PRIVATE
#    bin
#)
#
#target_compile_options(imsql PRIVATE ${SANITIZER_FLAGS})
#target_link_options(imsql PRIVATE ${SANITIZER_FLAGS})
#
#target_link_libraries(imsql PRIVATE imsql_lib)
#target_link_libraries(imsql PRIVATE "-framework Cocoa")
#target_link_libraries(imsql PRIVATE "-framework Metal")
#target_link_libraries(imsql PRIVATE "-framework MetalKit")
#target_link_libraries(imsql PRIVATE "-framework QuartzCore")
