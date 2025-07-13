if(DEFINED FETCH_METAL_CPP_INCLUDED)
  return()
endif()
set(FETCH_METAL_CPP_INCLUDED TRUE)

include(FetchContent)

FetchContent_Declare(metal_cpp
  URL https://developer.apple.com/metal/cpp/files/metal-cpp_macOS15.2_iOS18.2.zip
	EXCLUDE_FROM_ALL
)
add_library(metal_cpp INTERFACE)
target_include_directories(metal_cpp INTERFACE ${metal_cpp_SOURCE_DIR})
