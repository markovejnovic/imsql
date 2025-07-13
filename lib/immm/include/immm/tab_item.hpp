#ifndef IMMM_TAB_ITEM_HPP
#define IMMM_TAB_ITEM_HPP

#include "immm/component.hpp"
#include "immm/render_ctx.hpp"

namespace immm {

class TabItem : Component<"TabItem"> {
 public:
  explicit TabItem(RenderCtx& renderCtx, const char* tabItemName);
  explicit TabItem(RenderCtx& renderCtx, const std::string& tabItemName);
  ~TabItem();

  [[nodiscard]] constexpr auto Rendering() const -> bool { return isRendering_; }

  TabItem(const TabItem&) = delete;
  auto operator=(const TabItem&) -> TabItem& = delete;
  TabItem(TabItem&&) = delete;
  auto operator=(TabItem&&) -> TabItem& = delete;

 private:
  // TODO(marko): Is the flag necessary?
  bool isRendering_;
};

}  // namespace immm

#endif  // IMMM_TAB_ITEM_HPP
