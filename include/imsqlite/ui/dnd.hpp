/// @todo This library is broken for some reason.
#ifndef IMSQLITE_UI_DND_DND_SOURCE_HPP
#define IMSQLITE_UI_DND_DND_SOURCE_HPP

#include "imgui.h"
#include "imsqlite/ui/component.hpp"
#include "imsqlite/pch/std.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui {

enum class DndSourceFlag : uint8_t {
  NoPreviewTooltip = 1U << 0U,
  NoDisableHover = 1U << 1U,
  NoHoldToOpenOthers = 1U << 2U,
};

enum class DndAcceptFlag : uint16_t {
  AcceptBeforeDelivery = 1U << 10U,
  AcceptNoDrawDefaultRect = 1U << 11U,
  AcceptNoPreviewTooltip = 1U << 12U,
  AcceptPeekOnly = DndAcceptFlag::AcceptBeforeDelivery | DndAcceptFlag::AcceptNoDrawDefaultRect,
};

namespace detail {

constexpr auto FoldSourceFlags(
  std::initializer_list<DndSourceFlag> flags
) noexcept -> ImGuiDragDropFlags {
  auto cast_view = flags
    | std::views::transform([] (auto& flag) { return static_cast<ImGuiDragDropFlags>(flag); });
  return std::ranges::fold_left(cast_view, ImGuiDragDropFlags_None, std::bit_or{});
}

template <class T>
class DndSource : public ui::Component<"dnd::DndSource"> {
public:
  template <class WithinSelectionFrameF>
  explicit DndSource(
    ui::RenderCtx& ctx,
    const char* type,
    const T& data,
    WithinSelectionFrameF&& withinSelectionFrameFunc,
    std::initializer_list<DndSourceFlag> flags = {}
  ) : ui::Component<"dnd::DndSource">(ctx) {
    if (ImGui::BeginDragDropSource(detail::FoldSourceFlags(flags)))
    {
      ImGui::SetDragDropPayload(type, &data, sizeof(T));
      std::invoke(std::forward<WithinSelectionFrameF>(withinSelectionFrameFunc));
      isSelected_ = true;
    }
  }

  ~DndSource() {
    if (isSelected_) {
      ImGui::EndDragDropSource();
    }
  }

  DndSource(const DndSource&) = delete;
  auto operator=(const DndSource&) -> DndSource& = delete;
  DndSource(DndSource&&) = delete;
  auto operator=(DndSource&&) -> DndSource& = delete;

private:
  bool isSelected_ = false;
};

template <class T>
class DndTarget : public ui::Component<"dnd::DndTarget"> {
public:
  explicit DndTarget(ui::RenderCtx& ctx, const char* type)
      : ui::Component<"dnd::DndTarget">(ctx) {
    if (ImGui::BeginDragDropTarget()) {
      const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type);
      isSelected_ = true;

      printf("You dropped something of type '%s'.\n", type);
    }
  }

  ~DndTarget() {
    if (isSelected_) {
      ImGui::EndDragDropTarget();
    }
  }

  DndTarget(const DndTarget&) = delete;
  auto operator=(const DndTarget&) -> DndTarget& = delete;
  DndTarget(DndTarget&&) = delete;
  auto operator=(DndTarget&&) -> DndTarget& = delete;

private:
  bool isSelected_ = false;
};

} // namespace detail

template <class T>
class Dnd : public ui::Component<"dnd::Dnd"> {
    static_assert(std::is_trivial_v<T>, "Non-trivial types are not supported by Dnd.");
public:
  explicit Dnd(ui::RenderCtx& ctx, const char* type)
      : ui::Component<"dnd::Dnd">(ctx), type_(type) {}

  template <class WithinSelectionFrameF>
  [[nodiscard]] constexpr auto Source(
    const T& data,
    WithinSelectionFrameF&& withinSelectionFrameFunc,
    std::initializer_list<DndSourceFlag> flags = {}
  ) const -> detail::DndSource<T> {
    return detail::DndSource<T>(GetRenderCtx(), type_, data, withinSelectionFrameFunc, flags);
  }

  [[nodiscard]] constexpr auto Target() const -> detail::DndTarget<T> {
    return detail::DndTarget<T>(GetRenderCtx(), type_);
  }

private:
  const char* type_;
};


} // namespace imsql::ui

#endif // IMSQLITE_UI_DND_DND_SOURCE_HPP
