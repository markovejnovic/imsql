#ifndef IMSQLITE_UI_TEXT_HPP
#define IMSQLITE_UI_TEXT_HPP

#include "imsqlite/ui/component.hpp"
#include <string>

namespace imsql::ui {

class Text : Component<"Text"> {
public:
  explicit Text(RenderCtx& renderCtx, const char* label);
  explicit Text(RenderCtx& renderCtx, const std::string& label);
  ~Text();

  Text(const Text&) = delete;
  auto operator=(const Text&) -> Text& = delete;
  Text(Text&&) = delete;
  auto operator=(Text&&) -> Text& = delete;
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_TEXT_HPP
