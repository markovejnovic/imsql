#ifndef IMMM_TEXT_HPP
#define IMMM_TEXT_HPP

#include "immm/component.hpp"

namespace immm {

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

}  // namespace immm

#endif  // IMMM_TEXT_HPP
