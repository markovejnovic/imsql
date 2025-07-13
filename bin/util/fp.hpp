#ifndef IMSQLITE_UTIL_FP_HPP
#define IMSQLITE_UTIL_FP_HPP

namespace imsql::util {

template<class... Ts>
struct Overloads : Ts... { using Ts::operator()...; };

} // namespace imsql::util

#endif // IMSQLITE_UTIL_FP_HPP
