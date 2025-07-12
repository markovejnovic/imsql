#ifndef IMSQLITE_DSALG_FP_HPP
#define IMSQLITE_DSALG_FP_HPP

namespace imsql::dsalg {

template<class... Ts>
struct Overloads : Ts... { using Ts::operator()...; };

} // namespace imsql::dsalg

#endif // IMSQLITE_DSALG_FP_HPP
