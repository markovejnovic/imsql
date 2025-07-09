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
  }

  constexpr ~Component() {
    DebugRenderEnd(renderCtx_);
  }

private:
  void DebugRenderBegin(const RenderCtx& renderCtx) const {
    renderCtx.DbgStream() << std::string(renderCtx.RenderDepth() * 2, ' ');
    renderCtx.DbgStream() << "<" << label.String() << ">";
    renderCtx.DbgStream() << "\n";
    renderCtx_.Enter();
  }

  void DebugRenderEnd(const RenderCtx& renderCtx) const {
    renderCtx_.Exit();
    renderCtx.DbgStream() << std::string(renderCtx.RenderDepth() * 2, ' ');
    renderCtx.DbgStream() << "</" << label.String() << ">";
    renderCtx.DbgStream() << "\n";
  }

  RenderCtx& renderCtx_;
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_UI_COMPONENT_HPP
