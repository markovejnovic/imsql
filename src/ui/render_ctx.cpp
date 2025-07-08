#include "imsqlite/ui/render_ctx.hpp"
#include "boost/iostreams/device/null.hpp"
#include "boost/iostreams/stream.hpp"

namespace imsql::ui {

auto RenderCtx::DbgStream() const noexcept -> std::ostream& {
  if constexpr (IsDebugEnabled) {
    return std::cerr;
  } else {
    static boost::iostreams::stream nulls { boost::iostreams::null_sink() };
    return nulls;
  }
}

} // namespace imsql::ui
