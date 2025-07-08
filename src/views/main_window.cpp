#include "imsqlite/views/main_window.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include "imsqlite/ui/tab_bar.hpp"
#include "imsqlite/ui/tab_item.hpp"
#include "imsqlite/ui/window.hpp"
#include "imsqlite/views/spreadsheet_designer.hpp"
#include <format>

namespace imsql::views {

auto MainWindow(ui::RenderCtx& ctx, const models::AppState& appState) -> models::AppState {
  const auto title = appState.Database.Path();
  models::AppState new_app_state = appState;

  ui::Window window{ctx, title};
  if (!window.Rendering()) {
    return new_app_state;
  }

  ui::TabBar tab_bar{ctx, std::format("{}_tabbar", title)};
  if (!tab_bar.Rendering()) {
    return new_app_state;
  }

  {
    ui::TabItem spreadsheet_tab{ctx, "Spreadsheet"};
    if (spreadsheet_tab.Rendering()) {
    }
  }

  {
    ui::TabItem designer_tab{ctx, "Designer"};
    if (designer_tab.Rendering()) {
      SpreadsheetDesigner(ctx, new_app_state);
    }
  }

  return new_app_state;
}

} // namespace imsql::views
