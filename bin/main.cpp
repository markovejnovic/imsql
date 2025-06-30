#include "imgui.h"
#include "imsqlite/os/os.hpp"
#include "imsqlite/sql/db.hpp"
#include "imsqlite/ui/node_editor/node.hpp"
#include "imsqlite/ui/node_editor/node_editor.hpp"
#include "imsqlite/ui/windbg.hpp"
#include "lib/include/imsqlite/os/os.hpp"
#include <cstdio>
#include <exception>
#include <print>
#include <ranges>
#include <algorithm>
#include <thread>
#include "imnodes.h"
#include "imsqlite/ui/window.hpp"

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
    ImGui::DestroyContext();
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

  auto example_db = sql::Db::FromFile("/Users/marko/Desktop/applier/backend/development_db.db");

  os::Provider os_provider;

  namespace ui = ui;
  auto frame = os_provider.CreateFrame("Hello!", kDefaultDimensions);
  frame.StartRendering([&](const os::RenderInfo& render_info) {
    ImGui::ShowDemoWindow();
    ui::WinDbg dbg{render_info};
    {
      ui::Window window{"Table Connection Diagram"};
      ui::NodeEditor node_editor;

      int node_ctr = 0;
      int attr_ctr = 0;
      auto tables = example_db.Tables();
      for (const auto& table : tables) {
        auto node = node_editor.CreateNode(node_ctr++, table.Name().c_str());
        for (const auto& column : example_db.TableColumns(table.Name())) {
          // TODO(marko): Add more attributes.
          node.AddOutput(attr_ctr++, column.Name().c_str());
        }
      }
    }
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
