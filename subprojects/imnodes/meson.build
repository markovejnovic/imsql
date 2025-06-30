project('imnodes', 'cpp')

imgui_proj = subproject('imgui')

imnodes_lib = static_library(
  'imnodes_lib',
  ['imnodes.cpp'],
  include_directories: ['.'],
  dependencies: [
    imgui_proj.get_variable('imgui_dep'),
  ],
  cpp_args: [
    '-DIMGUI_DEFINE_MATH_OPERATORS',
  ],
)

imnodes_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with: imnodes_lib,
  dependencies: [
    imgui_proj.get_variable('imgui_dep'),
  ],
)
