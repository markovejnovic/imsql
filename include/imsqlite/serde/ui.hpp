#ifndef IMSQLITE_SERDE_UI_HPP
#define IMSQLITE_SERDE_UI_HPP

#include "imsqlite/ui/base_types.hpp"

namespace imsql::ui {

template <class Archive>
void serialize(Archive& archive, Vec2& vec, unsigned int /*version*/) {
  archive & vec.x_;
  archive & vec.y_;
}

} // namespace imsql::ui

#endif // IMSQLITE_SERDE_UI_HPP
