if(DEFINED FETCH_IMGUI_INCLUDED)
  return()
endif()
set(FETCH_IMGUI_INCLUDED TRUE)

include(FetchContent)
include("${CMAKE_CURRENT_LIST_DIR}/FetchMetalCpp.cmake")

find_package(glfw3 REQUIRED)

add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)

FetchContent_Declare(imgui
	URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9.tar.gz
	EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_metal.mm
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw metal_cpp)
