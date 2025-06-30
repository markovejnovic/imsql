project(
  'imsql',
  'cpp',
  'objcpp',
  default_options: {
    'c_std': 'c2x',
    'cpp_std': 'c++2a',
    'default_library': 'static',
  },
)

objcpp = meson.get_compiler('objcpp')

gflw_proj = subproject('glfw')
microsoft_gsl_proj = subproject('microsoft-gsl')
metal_cpp_proj = subproject('metal-cpp')
imgui_proj = subproject('imgui')
imnodes_proj = subproject('imnodes')
sqlitecpp_proj = subproject('sqlitecpp')

imsql_lib = static_library(
  'imsql_lib',
  ['lib/src/os/platform/macos.mm'],
  ['lib/src/ui/window.cpp'],
  ['lib/src/ui/windbg.cpp'],
  ['lib/src/ui/node_editor/node_editor.cpp'],
  ['lib/src/ui/node_editor/node.cpp'],
  ['lib/src/sql/db.cpp'],
  dependencies: [
    gflw_proj.get_variable('glfw_dep'),
    metal_cpp_proj.get_variable('metal_cpp_dep'),
    microsoft_gsl_proj.get_variable('microsoft_gsl_dep'),
    imgui_proj.get_variable('imgui_dep'),
    imnodes_proj.get_variable('imnodes_dep'),
    sqlitecpp_proj.get_variable('sqlitecpp_dep'),
  ],
  include_directories: [
    'lib/include',
  ],
  objcpp_args: [
    '-std=c++2a',
    '-DIMGUI_IMPL_METAL_CPP',
    '-DIMGUI_DEFINE_MATH_OPERATORS',
  ],
  link_args: [
    '-framework', 'Cocoa',
    '-framework', 'Metal',
    '-framework', 'MetalKit',
    '-framework', 'QuartzCore',
  ],
)

imsql_dep = declare_dependency(
  include_directories: include_directories('lib/include'),
  link_with: imsql_lib,
  dependencies: [
    gflw_proj.get_variable('glfw_dep'),
    metal_cpp_proj.get_variable('metal_cpp_dep'),
    microsoft_gsl_proj.get_variable('microsoft_gsl_dep'),
    imgui_proj.get_variable('imgui_dep'),
    imnodes_proj.get_variable('imnodes_dep'),
    sqlitecpp_proj.get_variable('sqlitecpp_dep'),
  ],
)

executable(
  'imsql',
  ['bin/main.cpp'],
  dependencies: [
    imgui_proj.get_variable('imgui_dep'),
    metal_cpp_proj.get_variable('metal_cpp_dep'),
    imsql_dep,
  ],
  cpp_args: [
    '-DIMGUI_IMPL_METAL_CPP',
    '-DIMGUI_DEFINE_MATH_OPERATORS',
    '-std=c++2a',
    '-Og',
    '-g',
  ],
  link_args: [
    '-framework', 'Cocoa',
    '-framework', 'Metal',
    '-framework', 'MetalKit',
    '-framework', 'QuartzCore',
  ],
)
