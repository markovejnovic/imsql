#ifndef IMSQLITE_VIEWS_SPREADSHEET_DESIGNER_HPP
#define IMSQLITE_VIEWS_SPREADSHEET_DESIGNER_HPP

#include "imsqlite/presenters/designer_presenter.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::views {

void SpreadsheetDesigner(ui::RenderCtx& ctx, imsql::presenters::DesignerPresenter& presenter);

} // namespace imsql::views

#endif // IMSQLITE_VIEWS_SPREADSHEET_DESIGNER_HPP
