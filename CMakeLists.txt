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
find_package(GTest)
find_package(range-v3 REQUIRED)
add_subdirectory(third_party)

if (GTest_FOUND)
  enable_testing()
endif()

add_executable(imsql)
target_include_directories(imsql PRIVATE include)
target_precompile_headers(
  imsql PRIVATE
  "include/imsqlite/pch/std.hpp"
  "include/imsqlite/pch/imgui.hpp"
  "include/imsqlite/pch/imnodes.hpp"
  "include/imsqlite/pch/graph.hpp"
)
target_sources(imsql PRIVATE
  src/controllers/db.cpp
  src/controllers/spreadsheet.cpp
  src/main.cpp
  src/models/base_types.cpp
  src/models/db.cpp
  src/models/dg/db_node.cpp
  src/models/dg/db_table_node.cpp
  src/models/dg/dg.cpp
  src/models/dg/node.cpp
  src/models/dg/transform_node.cpp
  src/os/platform/macos.mm
  src/ui/button.cpp
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
  range-v3::range-v3
)

target_compile_options(imsql PRIVATE ${SANITIZER_FLAGS} -ftime-trace)
target_link_options(imsql PRIVATE ${SANITIZER_FLAGS})

# TODO(marko): Move imsql library stuff into a library structure
add_executable(test_dedup
  tests/test_dedup.cpp
)

if (GTest_FOUND)
  target_include_directories(test_dedup PRIVATE include)
  target_link_libraries(test_dedup PRIVATE GTest::gtest_main)

  target_compile_options(test_dedup PRIVATE ${SANITIZER_FLAGS})
  target_link_options(test_dedup PRIVATE ${SANITIZER_FLAGS})

  include(GoogleTest)
  gtest_discover_tests(test_dedup)
endif()

# Docs
find_package(Doxygen REQUIRED)
if (Doxygen_FOUND)
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  add_custom_target(doc_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM
  )
endif()
