#ifndef IMMM_NODES_EDITOR_HPP
#define IMMM_NODES_EDITOR_HPP

#include "immm/component.hpp"

namespace immm::nodes {

class Editor final : Component<"Editor"> {
 public:
  explicit Editor(RenderCtx& renderCtx);
  ~Editor();

  Editor(const Editor&) = delete;
  auto operator=(const Editor&) -> Editor& = delete;
  Editor(Editor&&) = delete;
  auto operator=(Editor&&) -> Editor& = delete;
};

}  // namespace immm::nodes

#endif  // IMMM_NODES_EDITOR_HPP
