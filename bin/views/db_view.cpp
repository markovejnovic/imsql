#include "immm/tab_bar.hpp"
#include "immm/tab_item.hpp"
#include "immm/window.hpp"
#include "db_view.hpp"

namespace imsql {

void DbView::operator()(immm::RenderCtx& ctx) {
  const auto* title = dbModel_->Path().c_str();

  immm::Window window = immm::Window::CreateCoveringWindow(ctx, title);
  if (!window.Rendering()) {
    return;
  }

  immm::TabBar tab_bar{ctx, std::format("{}_tabbar", title)};
  if (!tab_bar.Rendering()) {
    return;
  }

  {
    immm::TabItem spreadsheet_tab{ctx, "Spreadsheet"};
    if (spreadsheet_tab.Rendering()) {
      SpreadsheetTableView_(ctx);
    }
  }

  {
    immm::TabItem designer_tab{ctx, "Designer"};
    if (designer_tab.Rendering()) {
      designerView_(ctx);
    }
  }
}

} // namespace imsql
