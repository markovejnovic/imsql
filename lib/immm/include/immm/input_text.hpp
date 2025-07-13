#ifndef IMMM_INPUT_TEXT_HPP
#define IMMM_INPUT_TEXT_HPP

#include "immm/component.hpp"
#include "immm/detail/pch.hpp"

namespace immm {

enum class InputTextOption : uint8_t {
  None = 0,
};

class InputText : Component<"InputText"> {
 public:
  explicit constexpr InputText(RenderCtx& renderCtx, const char* label, std::string& value,
                               InputTextOption options = InputTextOption::None);
};

}  // namespace immm

#endif  // IMMM_INPUT_TEXT_HPP
