#ifndef IMSQLITE_UI_NODES_EDITOR_HPP
#define IMSQLITE_UI_NODES_EDITOR_HPP

#include "imsqlite/ui/component.hpp"

namespace imsql::ui::nodes {

class Editor final : Component<"Editor"> {
public:
  explicit Editor(RenderCtx& renderCtx);
  ~Editor();

  Editor(const Editor&) = delete;
  auto operator=(const Editor&) -> Editor& = delete;
  Editor(Editor&&) = delete;
  auto operator=(Editor&&) -> Editor& = delete;
};

} // namespace imsql::ui::nodes

#endif // IMSQLITE_UI_NODES_EDITOR_HPP
