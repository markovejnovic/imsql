#include "imsqlite/presenters/presenter_ctx.hpp"
#define _GNU_SOURCE
#include "imsqlite/models/dg/dg.hpp"
#include "imsqlite/pch/imgui.hpp"
#include "imsqlite/pch/imnodes.hpp"
#include "imsqlite/controllers/app_controllers.hpp"
#include "imsqlite/controllers/db.hpp"
#include "imsqlite/controllers/identifier.hpp"
#include "imsqlite/controllers/spreadsheet.hpp"
#include "imsqlite/models/db.hpp"
#include "imsqlite/models/spreadsheet.hpp"
#include "imsqlite/models/theme.hpp"
#include "imsqlite/os/os.hpp"
#include "imsqlite/pch/std.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include "imsqlite/views/main_window.hpp"
#include <iostream>
#include <boost/stacktrace.hpp>

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

  auto frame = os_provider.CreateFrame("ImSQLite");

  auto ident_controller = std::make_shared<controllers::Identifier>();

  ui::RenderCtx render_ctx{
    controllers::AppControllers{
      controllers::Db{
        std::filesystem::path{ "/Users/marko/Desktop/applier/backend/development_db.db" },
        ident_controller,
      },
      controllers::Spreadsheet{ ident_controller },
    }
  };

  static const constinit models::Theme theme {};

  render_ctx.Controllers().Db.PullFreshState();

  models::dg::DG design_graph;
  design_graph.AttachDatabase(render_ctx.Controllers().Db.CModel());

  design_graph.DumpDot(std::cout);

  presenters::PresenterCtx presenter_ctx{
    .Designer = presenters::DesignerPresenter{ std::move(design_graph) },
  };

  frame.StartRendering([&](const os::RenderInfo& renderInfo) {
    views::MainWindow(render_ctx, presenter_ctx);
    render_ctx.RenderCycle();
  }, [&]() {
    // TODO(marko): Yucky hack. Wish I had a PersistenceController
    render_ctx.Controllers().Db.UpdatePersistentData(
      [&](auto& pdata) {
        //pdata.MainSheet = render_ctx.Controllers().Sheets.CModel();
      });
    render_ctx.Controllers().Db.StorePersistentData();
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
