cmake_minimum_required(VERSION 3.28)
project(imsql LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SANITIZER_FLAGS "-fsanitize=address,undefined" "-fno-omit-frame-pointer")

find_package(Microsoft.GSL REQUIRED)
find_package(Boost REQUIRED serialization iostreams)
find_package(SQLiteCpp REQUIRED)
add_subdirectory(lib/immm)

file(GLOB_RECURSE IMMM_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/bin/*.cpp"
)

add_executable(imsql)
target_include_directories(imsql PRIVATE bin)
target_precompile_headers(
  imsql PRIVATE
  bin/pch.hpp
)
target_sources(imsql PRIVATE
  ${IMMM_SOURCE_FILES}
)
target_link_libraries(imsql PRIVATE
  immm
  ${Boost_LIBRARIES}
  Microsoft.GSL::GSL
  SQLiteCpp
)

target_compile_options(imsql PRIVATE ${SANITIZER_FLAGS} -ftime-trace)
target_link_options(imsql PRIVATE ${SANITIZER_FLAGS})

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
