#ifndef IMSQLITE_CONTROLLERS_IDENTIFIER_HPP
#define IMSQLITE_CONTROLLERS_IDENTIFIER_HPP

#include <cstdlib>

namespace imsql::controllers {

class Identifier {
public:
  template <class T>
  [[nodiscard]] constexpr auto New() noexcept -> T {
    return T{objectCounter_++};
  }

private:
  std::size_t objectCounter_;
};

} // namespace imsql::controllers

#endif // IMSQLITE_CONTROLLERS_IDENTIFIER_HPP
