#ifndef IMMM_BUTTON_HPP
#define IMMM_BUTTON_HPP

#include "immm/component.hpp"
#include "immm/render_ctx.hpp"
namespace immm {

class Button final : public Component<"Button"> {
 public:
  static auto CreateAndIsClicked(RenderCtx& renderCtx, const char* label)
      -> std::pair<Button, bool>;
  static auto CreateAndIsClicked(RenderCtx& renderCtx, const std::string& label)
      -> std::pair<Button, bool>;

 private:
  explicit Button(RenderCtx& renderCtx);
};

}  // namespace immm

#endif  // IMMM_BUTTON_HPP
