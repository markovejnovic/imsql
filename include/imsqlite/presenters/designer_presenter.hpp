#ifndef IMSQLITE_PRESENTERS_DESIGNER_PRESENTER_HPP
#define IMSQLITE_PRESENTERS_DESIGNER_PRESENTER_HPP

#include "imsqlite/models/dg/dg.hpp"
#include "imsqlite/models/dg/operator_node.hpp"
#include <type_traits>

namespace imsql::presenters {

struct DesignerPresenter {
public:
  explicit DesignerPresenter(models::dg::DG initialModel) noexcept
    : model_(std::move(initialModel)) {}

  [[nodiscard]] constexpr auto Model() const noexcept -> const models::dg::DG& {
    return model_;
  }

  constexpr void OnPaint() noexcept {
    hasPainted_ = true;
  }

  [[nodiscard]] constexpr auto IsFirstPaint() const noexcept -> bool {
    return !hasPainted_;
  }

  /// @brief Add an operator type to the design graph.
  template <class T>
  constexpr void AddOperatorNode() {
    static_assert(std::is_base_of_v<models::dg::OperatorNode, T>,
                  "T must be derived from models::dg::OperatorNode.");
    model_.AddOperatorNode<T>();
  }

  constexpr void AddEdge(models::dg::DG::VertexType source,
                         models::dg::DG::VertexType target) {
    model_.AddEdge(source, target);
  }

  /// @brief Invoked when the user adds a new node to the design graph.
  void OnUserAddedNode();

private:
  models::dg::DG model_;

  bool hasPainted_ = false;
};

} // namespace imsql::presenters

#endif // IMSQLITE_PRESENTERS_DESIGNER_PRESENTER_HPP
