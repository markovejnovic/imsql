#ifndef IMSQLITE_UI_INPUT_TEXT_HPP
#define IMSQLITE_UI_INPUT_TEXT_HPP

#include "imsqlite/ui/component.hpp"
#include <string>

namespace imsql::ui {

enum class InputTextOption : uint8_t {
  None = 0,
};

class InputText : Component<"InputText"> {
public:
  explicit constexpr InputText(
    RenderCtx& renderCtx,
    const char* label,
    std::string& value,
    InputTextOption options = InputTextOption::None
  );
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_INPUT_TEXT_HPP
