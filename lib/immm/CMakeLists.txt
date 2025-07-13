cmake_minimum_required(VERSION 3.28)
project(immm LANGUAGES CXX OBJCXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OBJCXX_STANDARD 23)
set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Microsoft.GSL REQUIRED)
include(cmake/FetchImGui.cmake)
include(cmake/FetchImNodes.cmake)

file(GLOB_RECURSE IMMM_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE IMMM_HEADER_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_library(immm STATIC)
target_include_directories(immm
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_precompile_headers(immm PRIVATE
  include/immm/detail/pch.hpp
)
target_sources(immm PRIVATE
  ${IMMM_SOURCE_FILES}
  # TODO(marko): Move this file into IMMM_SOURCE_FILES. This is currently a
  # workaround to avoid issues with clang-format not supporting Objective-C++
  # files well enough.
  ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/macos.mm # TODO(marko): Not cross-platform.
)
target_compile_options(immm PRIVATE
  -Wall
  -Wextra
  -Wpedantic
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  target_compile_options(immm PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(immm PRIVATE -fsanitize=address,undefined)
endif()

target_link_libraries(immm
  PUBLIC
    "-framework Cocoa"
    "-framework Metal"
    "-framework MetalKit"
    "-framework QuartzCore"
    imgui
    imnodes
  INTERFACE
    Microsoft.GSL::GSL
)

export(TARGETS immm imgui imnodes metal_cpp FILE immmConfig.cmake)

# Linting and formatting
if (DEFINED IMMM_CLANG_TIDY)
  set(clang_tidy_outputs)

  foreach(sourcefile_long IN LISTS IMMM_SOURCE_FILES)
    get_filename_component(sourcefile ${sourcefile_long} NAME)
    set(outputfile "${CMAKE_CURRENT_BINARY_DIR}/clang-tidy-${sourcefile}.lnt")

    add_custom_command(
      OUTPUT ${outputfile}
      COMMAND ${IMMM_CLANG_TIDY} ${sourcefile_long}
        --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy
        2>&1 | tee ${outputfile}
      DEPENDS ${sourcefile_long}
      COMMENT "Running clang-tidy on ${sourcefile}..."
    )

    list(APPEND clang_tidy_outputs ${outputfile})
  endforeach()

  add_custom_target(immm_clang_tidy_check ALL
    DEPENDS ${clang_tidy_outputs}
    COMMENT "Running clang-tidy on all source files..."
  )
endif()

if (DEFINED IMMM_CLANG_FORMAT)
  add_custom_target(immm_clang_format_check ALL
    COMMAND ${IMMM_CLANG_FORMAT} --dry-run --Werror
            ${IMMM_SOURCE_FILES} ${IMMM_HEADER_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Checking code format..."
  )

  add_custom_target(immm_clang_format_fix
    COMMAND ${IMMM_CLANG_FORMAT} -i
            ${IMMM_SOURCE_FILES} ${IMMM_HEADER_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Fixing clang-format..."
  )
endif()

# Set up docs
find_package(Doxygen)
if (Doxygen_FOUND)
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  add_custom_target(immm_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM
  )
endif()

# Set up tests
find_package(GTest)
if (GTest_FOUND)
  enable_testing()
  set(SANITIZER_FLAGS "-fsanitize=address,undefined" "-fno-omit-frame-pointer")
endif()
