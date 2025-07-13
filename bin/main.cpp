#include "immm/platform/platform_macos.hpp"
#include "models/db_model.hpp"
#include "models/dg/dg.hpp"
#include "pch.hpp"
#include "immm/render_ctx.hpp"
#include "views/db_view.hpp"

namespace {
constexpr std::pair<std::size_t, std::size_t> kDefaultDimensions{800, 600};
}

namespace imsql {

struct AppContext {
  AppContext() {
    ImGui::CreateContext();
    ImNodes::CreateContext();
  }

  ~AppContext() {
    ImNodes::DestroyContext();
    if (ImGui::GetCurrentContext() != nullptr) {
      ImGui::DestroyContext();
    }
  }

  AppContext(const AppContext&) = delete;
  auto operator=(const AppContext&) -> AppContext& = delete;
  AppContext(AppContext&&) = delete;
  auto operator=(AppContext&&) -> AppContext& = delete;
};

auto Main(int /*argc*/, char *const *const /*argv*/) -> int {
  IMGUI_CHECKVERSION();
  AppContext app_context;

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();

  immm::PlatformMacOS os_provider{};

  auto frame = os_provider.CreateFrame("ImSQLite");

  immm::RenderCtx render_ctx{};

  DbModel db_model {
    "/Users/marko/Desktop/applier/backend/development_db.db"
  };

  dg::DesignGraphModel design_graph{&db_model};

  design_graph.DumpDot(std::cout);

  DbView db_view{db_model, design_graph};

  frame.StartRendering([&](const immm::RenderInfo& renderInfo) {
    db_view(render_ctx);
    render_ctx.RenderCycle();
  }, [&]() {
  });

  return 0;
}

} // namespace imsql

auto main(int /*argc*/, char *const *const /*argv*/) -> int {
  try {
    imsql::Main(0, nullptr);
  } catch (const std::exception& e) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    std::cerr << std::format("Unchaught exception: {}\n", e.what());
    std::cerr << boost::stacktrace::stacktrace() << "\n";
    return 1;
  } catch (...) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    std::cerr << std::format("Unsupported Error Thrown.");
    std::cerr << boost::stacktrace::stacktrace() << "\n";
    return 1;
  }

  return 0;
}
