#ifndef IMSQLITE_SERDE_MODELS_HPP
#define IMSQLITE_SERDE_MODELS_HPP

#include "imsqlite/models/base_types.hpp"
#include <cstdlib>

namespace imsql::models {

template <class Archive>
void serialize(Archive& archive, TableId& tableId, unsigned int /*version*/) {
  archive & tableId.underlyingId();
}

} // namespace imsql::models

#endif // IMSQLITE_SERDE_MODELS_HPP
