#ifndef IMSQLITE_MODELS_PERSISTENT_HPP
#define IMSQLITE_MODELS_PERSISTENT_HPP

#include "base_types.hpp"
#include "imsqlite/models/spreadsheet.hpp"
#include "imsqlite/ui/base_types.hpp"
#include "imsqlite/std.hpp"

namespace imsql::models {

/// @brief Data stored across multiple executions.
struct Persistent {
  boost::unordered_map<TableId, ui::Vec2> TableNodePositions;
  boost::optional<ui::Vec2> SpreadsheetNodePosition = boost::none;

  models::Spreadsheet MainSheet;

  template <class Archive>
  void serialize(Archive& archive, const unsigned int version) {
    if (version >= 0) {
      archive & TableNodePositions;
      archive & SpreadsheetNodePosition;
    }

    if (version >= 1) {
      archive & MainSheet;
    }

    if (version >= 2) {
      throw std::runtime_error(
        "Unsupported version for Persistent serialization: " + imsql::strof(version));
    }
  }
};

} // namespace imsql::models

BOOST_CLASS_VERSION(imsql::models::Persistent, 1);

#endif // IMSQLITE_MODELS_PERSISTENT_HPP
