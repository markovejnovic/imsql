#ifndef IMSQLITE_VIEWS_MAIN_WINDOW_HPP
#define IMSQLITE_VIEWS_MAIN_WINDOW_HPP

#include "imsqlite/models/app_state.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::views {

auto MainWindow(ui::RenderCtx& ctx, const models::AppState& appState) -> models::AppState;

} // namespace imsql::views

#endif // IMSQLITE_VIEWS_MAIN_WINDOW_HPP
