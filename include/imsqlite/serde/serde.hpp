/// @brief serialization and deserialization umbrella.
#ifndef IMSQLITE_SERDE_SERDE_HPP
#define IMSQLITE_SERDE_SERDE_HPP

#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"

#include "ui.hpp"
#include "models.hpp"

namespace imsql::serde {

template <class StreamT, class ObjectT>
void Serialize(StreamT& stream, const ObjectT& object) {
  boost::archive::text_oarchive archive(stream);
  archive << object;
}

template <class ObjectT, class StreamT>
auto Deserialize(StreamT& stream) -> ObjectT {
  boost::archive::text_iarchive archive(stream);
  ObjectT object;
  archive >> object;

  return object;
}

} // namespace imsql::serde

#endif // IMSQLITE_SERDE_SERDE_HPP
