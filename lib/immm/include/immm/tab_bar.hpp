#ifndef IMMM_TAB_BAR_HPP
#define IMMM_TAB_BAR_HPP

#include "immm/component.hpp"
#include "immm/render_ctx.hpp"

namespace immm {

class TabBar : Component<"TabBar"> {
 public:
  explicit TabBar(RenderCtx& renderCtx, const char* tabBarName);
  explicit TabBar(RenderCtx& renderCtx, const std::string& tabBarName);
  ~TabBar();

  [[nodiscard]] constexpr auto Rendering() const -> bool { return isRendering_; }

  TabBar(const TabBar&) = delete;
  auto operator=(const TabBar&) -> TabBar& = delete;
  TabBar(TabBar&&) = delete;
  auto operator=(TabBar&&) -> TabBar& = delete;

 private:
  // TODO(marko): Is the flag necessary?
  bool isRendering_;
};

}  // namespace immm

#endif  // IMMM_TAB_BAR_HPP
