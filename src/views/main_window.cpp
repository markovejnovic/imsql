#include "imsqlite/views/main_window.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include "imsqlite/ui/tab_bar.hpp"
#include "imsqlite/ui/tab_item.hpp"
#include "imsqlite/ui/window.hpp"
#include "imsqlite/views/spreadsheet_designer.hpp"
#include "imsqlite/pch/std.hpp"

namespace imsql::views {

void MainWindow(ui::RenderCtx& ctx, imsql::presenters::PresenterCtx& presenterCtx) {
  const auto title = ctx.Controllers().Db.CModel().Path();

  ui::Window window{ctx, title};
  if (!window.Rendering()) {
    return;
  }

  ui::TabBar tab_bar{ctx, std::format("{}_tabbar", title)};
  if (!tab_bar.Rendering()) {
    return;
  }

  {
    ui::TabItem spreadsheet_tab{ctx, "Spreadsheet"};
    if (spreadsheet_tab.Rendering()) {
    }
  }

  {
    ui::TabItem designer_tab{ctx, "Designer"};
    if (designer_tab.Rendering()) {
      SpreadsheetDesigner(ctx, presenterCtx.Designer);
    }
  }
}

} // namespace imsql::views
