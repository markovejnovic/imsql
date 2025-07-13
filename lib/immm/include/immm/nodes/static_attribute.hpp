#ifndef IMMM_NODES_STATIC_ATTRIBUTE_HPP
#define IMMM_NODES_STATIC_ATTRIBUTE_HPP

#include "immm/component.hpp"
#include "immm/render_ctx.hpp"

namespace immm::nodes {

class StaticAttribute final : public Component<"nodes::StaticAttribute"> {
 public:
  explicit StaticAttribute(RenderCtx& renderCtx, int attributeId);
  ~StaticAttribute();

  StaticAttribute(const StaticAttribute&) = delete;
  auto operator=(const StaticAttribute&) -> StaticAttribute& = delete;
  StaticAttribute(StaticAttribute&&) = delete;
  auto operator=(StaticAttribute&&) -> StaticAttribute& = delete;
};

}  // namespace immm::nodes

#endif  // IMMM_NODES_STATIC_ATTRIBUTE_HPP
