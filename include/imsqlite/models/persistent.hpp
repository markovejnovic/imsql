#ifndef IMSQLITE_MODELS_PERSISTENT_HPP
#define IMSQLITE_MODELS_PERSISTENT_HPP

#include "base_types.hpp"
#include "imsqlite/ui/base_types.hpp"
#include <boost/unordered_map.hpp>

namespace imsql::models {

/// @brief Data stored across multiple executions.
struct Persistent {
  boost::unordered_map<TableId, ui::Vec2> TableNodePositions;

  template <class Archive>
  void serialize(Archive& archive, const unsigned int /*version*/) {
    archive & TableNodePositions;
  }
};

} // namespace imsql::models

#endif // IMSQLITE_MODELS_PERSISTENT_HPP
