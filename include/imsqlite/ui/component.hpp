#ifndef IMSQLITE_UI_UI_COMPONENT_HPP
#define IMSQLITE_UI_UI_COMPONENT_HPP

#include "detail/component_label.hpp"
#include "render_ctx.hpp"
#include <cstdlib>
#include <string>

namespace imsql::ui {

template <detail::ComponentLabel label>
class Component {
public:
  Component(const Component&) = delete;
  auto operator=(const Component&) -> Component& = delete;
  Component(Component&&) = delete;
  auto operator=(Component&&) -> Component& = delete;

protected:
  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  constexpr Component(RenderCtx& renderCtx) : renderCtx_(renderCtx) {
    DebugRenderBegin(renderCtx_);
    renderCtx_.Enter();
  }

  constexpr ~Component() {
    renderCtx_.Exit();
    DebugRenderEnd(renderCtx_);
  }

private:
  void DebugRenderBegin(const RenderCtx& renderCtx) const {
    renderCtx.DbgStream() << std::string(" ", renderCtx.RenderDepth() * 2);
    renderCtx.DbgStream() << "<" << label.String() << ">";
    renderCtx.DbgStream() << "\n";
  }

  void DebugRenderEnd(const RenderCtx& renderCtx) const {
    renderCtx.DbgStream() << std::string(" ", renderCtx.RenderDepth() * 2);
    renderCtx.DbgStream() << "</" << label.String() << ">";
    renderCtx.DbgStream() << "\n";
  }

  RenderCtx& renderCtx_;
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_UI_COMPONENT_HPP
