#ifndef IMSQLITE_COLLECTION_HPP
#define IMSQLITE_COLLECTION_HPP

namespace imsql::collection {

template <class T, class K>
[[nodiscard]] constexpr auto Contains(
  const T& haystack,
  const K& needle
) -> bool {
  return haystack.find(needle) != T::npos;
}

} // namespace imsql::collection

#endif // IMSQLITE_COLLECTION_HPP
