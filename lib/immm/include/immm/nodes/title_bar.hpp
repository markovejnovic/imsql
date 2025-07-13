#ifndef IMMM_NODES_TITLE_BAR_HPP
#define IMMM_NODES_TITLE_BAR_HPP

#include "immm/component.hpp"

namespace immm::nodes {

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

}  // namespace immm::nodes

#endif  // IMMM_NODES_TITLE_BAR_HPP
