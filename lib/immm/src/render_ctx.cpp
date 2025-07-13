#include "immm/render_ctx.hpp"
#include <iostream>
#include "boost/iostreams/device/null.hpp"
#include "boost/iostreams/stream.hpp"

namespace immm {

auto RenderCtx::DbgStream() const -> std::ostream& {
  if constexpr (kIsDebugEnabled) {
    return std::cerr;
  } else {
    static boost::iostreams::stream nulls{boost::iostreams::null_sink()};
    return nulls;
  }
}

}  // namespace immm
