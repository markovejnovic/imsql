#ifndef IMSQLITE_SERDE_MODELS_HPP
#define IMSQLITE_SERDE_MODELS_HPP

#include "imsqlite/models/base_types.hpp"
#include "imsqlite/models/persistent.hpp"
#include <cstdlib>
#include <string>

namespace imsql::models {

template <class Archive>
void serialize(Archive& archive, TableId& tableId, unsigned int /*version*/) {
  archive & tableId.underlyingId();
}

template <class Archive>
void serialize(Archive& archive, ColumnId& columnId, unsigned int /*version*/) {
  archive & columnId.underlyingId();
}

template <class Archive>
void serialize(Archive& archive, Spreadsheet& spreadsheet, const unsigned int version) {
  if (version >= 0) {
    archive & spreadsheet.columnNames_;
  }

  if (version >= 1) {
    throw std::runtime_error(
      "Unsupported version for Spreadsheet serialization: " + imsql::strof(version));
  }
}

} // namespace imsql::models

#endif // IMSQLITE_SERDE_MODELS_HPP
