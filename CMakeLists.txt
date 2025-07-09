cmake_minimum_required(VERSION 3.28)
project(imsql LANGUAGES C CXX OBJCXX)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_OBJCXX_STANDARD 23)
set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)
set(SANITIZER_FLAGS "-fsanitize=address,undefined" "-fno-omit-frame-pointer")

find_package(glfw3 REQUIRED)
find_package(Boost REQUIRED serialization iostreams)
add_subdirectory(third_party)

add_executable(imsql)
target_include_directories(imsql PRIVATE include)
target_sources(imsql PRIVATE
  src/controllers/db.cpp
  src/main.cpp
  src/models/base_types.cpp
  src/models/db.cpp
  src/os/platform/macos.mm
  src/ui/input_text.cpp
  src/ui/nodes/editor.cpp
  src/ui/nodes/input_attribute.cpp
  src/ui/nodes/link.cpp
  src/ui/nodes/node.cpp
  src/ui/nodes/output_attribute.cpp
  src/ui/nodes/static_attribute.cpp
  src/ui/nodes/title_bar.cpp
  src/ui/render_ctx.cpp
  src/ui/tab_bar.cpp
  src/ui/tab_item.cpp
  src/ui/text.cpp
  src/ui/window.cpp
  src/views/main_window.cpp
  src/views/spreadsheet_designer.cpp
)
target_sources(imsql PRIVATE
  FILE_SET modules TYPE CXX_MODULES FILES
  #src/models/db.cpp
)
target_link_libraries(imsql PRIVATE
  "-framework Cocoa"
  "-framework Metal"
  "-framework MetalKit"
  "-framework QuartzCore"
  ${Boost_LIBRARIES}
  Microsoft.GSL::GSL
  SQLiteCpp
  glfw
  imgui
  imnodes
  metal_cpp
)

target_compile_options(imsql PRIVATE ${SANITIZER_FLAGS})
target_link_options(imsql PRIVATE ${SANITIZER_FLAGS})
