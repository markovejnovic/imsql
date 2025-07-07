include(FetchContent)

# Necessary for imnode
add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)

FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.2.0"
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)

FetchContent_Declare(imgui
	URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9.tar.gz
	EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(imnodes
	URL https://github.com/Nelarius/imnodes/archive/refs/tags/v0.5.tar.gz
	EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imnodes)

FetchContent_Declare(sqlitecpp
  URL https://github.com/SRombauts/SQLiteCpp/archive/refs/tags/3.3.3.tar.gz
	EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(sqlitecpp)

FetchContent_Declare(googletest
  URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(googletest)

add_library(metal_cpp INTERFACE)
target_include_directories(metal_cpp INTERFACE metal-cpp)

find_package(glfw3 REQUIRED)

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

add_library(imnodes STATIC ${imnodes_SOURCE_DIR}/imnodes.cpp)
target_include_directories(imnodes PUBLIC ${imnodes_SOURCE_DIR})
target_link_libraries(imnodes PUBLIC imgui)
