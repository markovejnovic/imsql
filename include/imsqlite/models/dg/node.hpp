#ifndef IMSQLITE_MODELS_DG_NODE_HPP
#define IMSQLITE_MODELS_DG_NODE_HPP

namespace imsql::models::dg {

/// @brief A node represents a single entity in the design graph.
///
/// @details
/// If the design graph looks like this:
/// ```
/// | Bar     |        | Foo       |
/// |     a > |--|     | > x       |
/// |         |  ----->| > y   z > |
/// ```
///
/// Then the entities labelled `Foo` and `Bar` are nodes. Contrast that with the entities `a`, `x`,
/// `y`, and `z` which are vertices.
class Node {
public:
  virtual ~Node();

  /// @brief Get the name of this node. In the example above, `Foo` and `Bar` would be the names.
  [[nodiscard]] virtual auto Name() const noexcept -> std::string_view = 0;
};

} // namespace imsql::models::dg

#endif // IMSQLITE_MODELS_DG_NODE_HPP
