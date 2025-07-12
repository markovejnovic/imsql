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
  Component(Component&& other) noexcept
      : renderCtx_(std::exchange(other.renderCtx_, nullptr)) {};
  auto operator=(Component&& other) noexcept -> Component& {
    if (this == &other) {
      return *this;
    }

    std::exchange(other.renderCtx_, nullptr);

    return *this;
  };

protected:
  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  constexpr Component(RenderCtx& renderCtx) : renderCtx_(&renderCtx) {
    DebugRenderBegin();
  }

  constexpr ~Component() {
    DebugRenderEnd();
  }

  [[nodiscard]] constexpr auto GetRenderCtx() const -> RenderCtx& {
    if (renderCtx_ == nullptr) {
      throw std::runtime_error("Render context is not set for the component. Are you using it "
                               "after a move?");
    }

    return *renderCtx_;
  }

private:
  void DebugRenderBegin() const {
    if (renderCtx_ == nullptr) {
      return;
    }

    renderCtx_->DbgStream() << std::string(renderCtx_->RenderDepth() * 2, ' ');
    renderCtx_->DbgStream() << "<" << label.String() << ">";
    renderCtx_->DbgStream() << "\n";
    renderCtx_->Enter();
  }

  void DebugRenderEnd() const {
    if (renderCtx_ == nullptr) {
      return;
    }

    renderCtx_->Exit();
    renderCtx_->DbgStream() << std::string(renderCtx_->RenderDepth() * 2, ' ');
    renderCtx_->DbgStream() << "</" << label.String() << ">";
    renderCtx_->DbgStream() << "\n";
  }

  RenderCtx* renderCtx_ = nullptr;
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_UI_COMPONENT_HPP
