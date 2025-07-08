#ifndef IMSQLITE_UI_NODES_TITLE_BAR_HPP
#define IMSQLITE_UI_NODES_TITLE_BAR_HPP

#include "imsqlite/ui/component.hpp"

namespace imsql::ui::nodes {

class TitleBar : Component<"nodes::TitleBar"> {
public:
  explicit TitleBar(RenderCtx& renderCtx, const char* title);
  explicit TitleBar(RenderCtx& renderCtx, const std::string& title);
  ~TitleBar();

  TitleBar(const TitleBar&) = delete;
  auto operator=(const TitleBar&) -> TitleBar& = delete;
  TitleBar(TitleBar&&) = delete;
  auto operator=(TitleBar&&) -> TitleBar& = delete;
};

} // namespace imsql::ui::nodes

#endif // IMSQLITE_UI_NODES_TITLE_BAR_HPP
