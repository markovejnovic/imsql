#include "imgui.h"
#include "imsqlite/_controller/dbg.hpp"
#include "imsqlite/db/db.hpp"
#include "imsqlite/deprecated.hpp"
#include "imsqlite/os/os.hpp"
#include "imsqlite/ui/sheet_window.hpp"
#include "imsqlite/controllers/db.hpp"
#include <cstdio>
#include <exception>
#include <filesystem>
#include <print>
#include "imnodes.h"
#include "imsqlite/_model/sheet.hpp"

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

  namespace ui = ui;
  auto frame = os_provider.CreateFrame("Hello!", kDefaultDimensions);

  controllers::Db db_controller {
    std::filesystem::path{
      "/Users/marko/Desktop/applier/backend/development_db.db"
    },
  }

  const std::optional<PersistentAppData> persistent_app_data
    = db_controller.LoadPersistentAppData();

  // TODO(marko): Clean up this junk.
  // This flag is a pretty bad hack. Ideally StartRendering would give an
  // "initial paint" callback.
  std::optional<std::pair<ui::UiState<false>, model::Sheet>> renderState;

  frame.StartRendering([&](const os::RenderInfo& render_info) {
    if (!renderState.has_value()) {
      auto [uiState, sheetState] = ui::SheetWindow(
        ui::UiState<true>{
          .DesignerTabState = ui::DesignerTabState<true>{
            // TODO(marko): Monads would be nice here.
            .TableNodePositions =
              persistent_app_data.has_value()
                ? persistent_app_data->TableNodePositions
                : std::unordered_map<TableId, ImVec2>{},
          },
        },
        model::Sheet{
            initial_spreadsheet,
            {
              model::SheetRow{
                {model::SheetCellInteger{42}, model::SheetCellText{"Hello"}}
              },
              model::SheetRow{
                {model::SheetCellReal{3.14}, model::SheetCellText{"World"}}
              },
            },
            db_controller.DbState(),
        }
      );

      renderState = std::make_pair(uiState, sheetState);
    }

    dbg_controller.ObserveFrameTime(render_info.FrameTime());
    dbg_controller.View().Render();

    renderState = ui::SheetWindow(renderState->first, renderState->second);

    ImGui::ShowDemoWindow();
  }, [&]() {
    db_controller.SavePersistentAppData(PersistentAppData{
      .TableNodePositions =
        renderState->first.DesignerTabState.TableNodePositions,
    });
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
