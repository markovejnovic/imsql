// dumping ground of random experiments with how to architect the UI.
#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "imnodes.h"
#include "imsqlite/db/db.hpp"
#include <array>
#include <gsl/gsl>
#include <iostream>
#include <string>

namespace imsql::ui {

template <bool IsFirstPaint>
struct DesignerTabState {
  bool HasPainted = false;
  std::unordered_map<TableId, ImVec2> TableNodePositions;
};

template <
  bool IsFirstPaint
>
struct UiState {
  DesignerTabState<IsFirstPaint> DesignerTabState;
};

} // namespace imsql::ui
