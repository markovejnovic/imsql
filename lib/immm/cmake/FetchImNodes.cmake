if(DEFINED FETCH_IMNODES_INCLUDED)
  return()
endif()
set(FETCH_IMNODES_INCLUDED TRUE)

include(FetchContent)
include("${CMAKE_CURRENT_LIST_DIR}/FetchImGui.cmake")

FetchContent_Declare(imnodes
	URL https://github.com/Nelarius/imnodes/archive/refs/tags/v0.5.tar.gz
	EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imnodes)

add_library(imnodes STATIC ${imnodes_SOURCE_DIR}/imnodes.cpp)
target_include_directories(imnodes PUBLIC ${imnodes_SOURCE_DIR})
target_link_libraries(imnodes PUBLIC imgui)
