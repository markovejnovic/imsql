#include "imgui.h"
#include "imsqlite/controllers/db.hpp"
#include "imsqlite/models/app_state.hpp"
#include "imsqlite/models/db.hpp"
#include "imsqlite/os/os.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include "imsqlite/views/main_window.hpp"
#include <cstdio>
#include <exception>
#include <filesystem>
#include <print>
#include <thread>
#include "imnodes.h"

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

  os::Provider os_provider;

  auto frame = os_provider.CreateFrame("Hello!", kDefaultDimensions);

  controllers::Db db_controller {
    std::filesystem::path{
      "/Users/marko/Desktop/applier/backend/development_db.db"
    },
  };

  models::AppState app_state = {
    .Database = db_controller.PullFreshState(),
  };

  ui::RenderCtx render_ctx{};
  frame.StartRendering([&](const os::RenderInfo& render_info) {
    app_state = views::MainWindow(render_ctx, app_state);
    render_ctx.RenderCycle();
  }, [&]() {
    db_controller.StorePersistentData(app_state.Database);
  });

  return 0;
}

} // namespace imsql

auto main(int /*argc*/, char *const *const /*argv*/) -> int {
  try {
    imsql::Main(0, nullptr);
  } catch (const std::exception& e) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    (void)fprintf(stderr, "Uncaught exception: %s\n", e.what());
    return 1;
  } catch (...) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    (void)fprintf(stderr, "Unsupported error.\n");
    return 1;
  }

  return 0;
}
