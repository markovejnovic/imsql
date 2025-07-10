#ifndef IMSQLITE_UI_BUTTON_HPP
#define IMSQLITE_UI_BUTTON_HPP

#include "imsqlite/ui/component.hpp"
#include "imsqlite/ui/render_ctx.hpp"
namespace imsql::ui {

class Button final : public Component<"Button"> {
public:
  static auto CreateAndIsClicked(RenderCtx& renderCtx,
                                 const char* label) -> std::pair<Button, bool>;
  static auto CreateAndIsClicked(RenderCtx& renderCtx,
                                 const std::string& label) -> std::pair<Button, bool>;

private:
  explicit Button(RenderCtx& renderCtx);
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_BUTTON_HPP
