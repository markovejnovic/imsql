// dumping ground of random experiments with how to architect the UI.
#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "imnodes.h"
#include "imsqlite/db/db.hpp"
#include <array>
#include <gsl/gsl>
#include <iostream>
#include <string>

namespace imsql::ui {

template <bool IsFirstPaint>
struct DesignerTabState {
  bool HasPainted = false;
  std::unordered_map<TableId, ImVec2> TableNodePositions;
};

template <
  bool IsFirstPaint
>
struct UiState {
  DesignerTabState<IsFirstPaint> DesignerTabState;
};

template <std::size_t N>
struct UiComponentLabel {
    char value[N];

    // constexpr constructor copies the string literal
    constexpr UiComponentLabel(const char (&str)[N]) {
        for (std::size_t i = 0; i < N; ++i)
            value[i] = str[i];
    }

    constexpr bool operator==(const UiComponentLabel&) const = default;
};

static constexpr bool DebugEnabled = false;
static size_t gRenderDepth = 0;

template <UiComponentLabel label>
class UiComponent {
public:
  UiComponent(const UiComponent&) = delete;
  auto operator=(const UiComponent&) -> UiComponent& = delete;
  UiComponent(UiComponent&&) = delete;
  auto operator=(UiComponent&&) -> UiComponent& = delete;

protected:
  constexpr UiComponent(const char* additionalData = nullptr) {
    if constexpr (DebugEnabled) {
      for (std::size_t i = 0; i < gRenderDepth; ++i) {
        std::cout << "  "; // Indent based on render depth
      }
      std::cout << "<" << label.value;
      if (additionalData != nullptr) {
        std::cout << " additionalData=\"" << additionalData << "\"";
      }
      std::cout << ">\n";
    }
    gRenderDepth++;
  }

  ~UiComponent() {
    gRenderDepth--;
    if constexpr (DebugEnabled) {
      for (std::size_t i = 0; i < gRenderDepth; ++i) {
        std::cout << "  "; // Indent based on render depth
      }
      std::cout << "</" << label.value << ">\n";
    }
  }

private:
};

inline auto Utf8Bytes(int value) -> std::string {
  std::string result;
  result.reserve(4);
  return ImTextCharToUtf8(result.data(), value);
}

class Window : UiComponent<"Window"> {
public:
  explicit Window(const char* title) : isRendering_(ImGui::Begin(title)) {}

  ~Window() {
    if (isRendering_) {
      // End the window only if it was successfully started.
      // This is important to avoid ImGui errors.
      ImGui::End();
    }
  }

  [[nodiscard]] constexpr auto Rendering() const -> bool {
    // TODO(marko): Do I need this flag?
    return isRendering_;
  }

  Window(const Window&) = delete;
  auto operator=(const Window&) -> Window& = delete;
  Window(Window&&) = delete;
  auto operator=(Window&&) -> Window& = delete;

private:
  bool isRendering_;
};

class NodeEditor : UiComponent<"NodeEditor"> {
public:
  NodeEditor() {
    ImNodes::BeginNodeEditor();
  }

  ~NodeEditor() {
    ImNodes::EndNodeEditor();
  }

  NodeEditor(const NodeEditor&) = delete;
  auto operator=(const NodeEditor&) -> NodeEditor& = delete;
  NodeEditor(NodeEditor&&) = delete;
  auto operator=(NodeEditor&&) -> NodeEditor& = delete;
};

class Node : UiComponent<"Node"> {
public:
  explicit Node(int nodeId) : nodeId_(nodeId) {
    ImNodes::BeginNode(nodeId);
  }

  ~Node() {
    ImNodes::EndNode();
  }

  constexpr void SetPosition(const ImVec2& position) const {
    ImNodes::SetNodeGridSpacePos(nodeId_, position);
  }

  [[nodiscard]] constexpr auto Position() const -> ImVec2 {
    return ImNodes::GetNodeGridSpacePos(nodeId_);
  }

  Node(const Node&) = delete;
  auto operator=(const Node&) -> Node& = delete;
  Node(Node&&) = delete;
  auto operator=(Node&&) -> Node& = delete;

private:
  int nodeId_;
};

class NodeTitleBar : UiComponent<"NodeTitleBar"> {
public:
  explicit NodeTitleBar(const char* title) {
    ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(255, 0, 0, 255));
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(title);
  }

  ~NodeTitleBar() {
    ImNodes::EndNodeTitleBar();
    ImNodes::PopColorStyle();
  }

  NodeTitleBar(const NodeTitleBar&) = delete;
  auto operator=(const NodeTitleBar&) -> NodeTitleBar& = delete;
  NodeTitleBar(NodeTitleBar&&) = delete;
  auto operator=(NodeTitleBar&&) -> NodeTitleBar& = delete;
};

class NodeInputAttribute : UiComponent<"NodeInputAttribute"> {
public:
  explicit NodeInputAttribute(int attributeId) {
    ImNodes::BeginInputAttribute(attributeId);
  }

  ~NodeInputAttribute() {
    ImNodes::EndInputAttribute();
  }

  NodeInputAttribute(const NodeInputAttribute&) = delete;
  auto operator=(const NodeInputAttribute&) -> NodeInputAttribute& = delete;
  NodeInputAttribute(NodeInputAttribute&&) = delete;
  auto operator=(NodeInputAttribute&&) -> NodeInputAttribute& = delete;
};

class NodeOutputAttribute : UiComponent<"NodeOutputAttribute"> {
public:
  explicit NodeOutputAttribute(int attributeId) {
    ImNodes::BeginOutputAttribute(attributeId);
  }

  ~NodeOutputAttribute() {
    ImNodes::EndOutputAttribute();
  }

  NodeOutputAttribute(const NodeOutputAttribute&) = delete;
  auto operator=(const NodeOutputAttribute&) -> NodeOutputAttribute& = delete;
  NodeOutputAttribute(NodeOutputAttribute&&) = delete;
  auto operator=(NodeOutputAttribute&&) -> NodeOutputAttribute& = delete;
};

class NodeStaticAttribute : UiComponent<"NodeStaticAttribute"> {
public:
  explicit NodeStaticAttribute(int attributeId) {
    ImNodes::BeginStaticAttribute(attributeId);
  }

  ~NodeStaticAttribute() {
    ImNodes::EndStaticAttribute();
  }

  NodeStaticAttribute(const NodeStaticAttribute&) = delete;
  auto operator=(const NodeStaticAttribute&) -> NodeStaticAttribute& = delete;
  NodeStaticAttribute(NodeStaticAttribute&&) = delete;
  auto operator=(NodeStaticAttribute&&) -> NodeStaticAttribute& = delete;
};

class TabBar : UiComponent<"TabBar"> {
public:
  explicit TabBar(const char* tabBarName)
  // TODO(marko): Is the flag necessary?
    : isRendering_(ImGui::BeginTabBar(tabBarName)) {}

  ~TabBar() {
    if (isRendering_) {
      // End the tab bar only if it was successfully started.
      // This is important to avoid ImGui errors.
      ImGui::EndTabBar();
    }
  }

  [[nodiscard]] constexpr auto Rendering() const -> bool {
    return isRendering_;
  }

  TabBar(const TabBar&) = delete;
  auto operator=(const TabBar&) -> TabBar& = delete;
  TabBar(TabBar&&) = delete;
  auto operator=(TabBar&&) -> TabBar& = delete;

private:
  bool isRendering_;
};

class TabItem : UiComponent<"TabItem"> {
public:
  explicit TabItem(const char* tabName)
    : UiComponent(tabName)
    , isRendering_(ImGui::BeginTabItem(tabName)) {}

  ~TabItem() {
    if (isRendering_) {
      ImGui::EndTabItem();
    }
  }

  [[nodiscard]] constexpr auto Rendering() const -> bool {
    return isRendering_;
  }

  TabItem(const TabItem&) = delete;
  auto operator=(const TabItem&) -> TabItem& = delete;
  TabItem(TabItem&&) = delete;
  auto operator=(TabItem&&) -> TabItem& = delete;

private:
  bool isRendering_ = true; // Assume rendering is successful.
};

class InputText : UiComponent<"InputText"> {
public:
  explicit InputText(
    const char* label,
    std::string& value,
    ImGuiInputTextFlags flags = 0
  ) {
    // This number is picked because it's likely all implementations of the STL
    // will fit SSO under this limit.
    static constexpr std::size_t kInitialCapacity = 15;
    value.reserve(kInitialCapacity);

    const auto callback = [](ImGuiInputTextCallbackData* data) -> int {
      auto* str = static_cast<std::string*>(data->UserData);
      if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        // TODO(marko): This feels costly -- ImGui will call CallbackResize
        // regardless of if we need a resize or not. It seems they should do
        // some culling on their end but they don't. I wonder, perhaps, if we
        // should do culling here? Ultimately I imagine resize() is something
        // like
        // if (this->size() >= new_size) {
        //   return;
        // }
        //
        // So perhaps there is no need to do anything here ourselves. Idk,
        // feels weird this is called all the time, esp. considering that the
        // pointer jump may involve a trampoline.
        //
        // Whatever, problem for later.
        str->resize(gsl::narrow<int>(data->BufTextLen * 2));
        data->Buf = str->data();
        data->BufSize = gsl::narrow<int>(str->capacity() + 1);
      }

      return 0;
    };

    ImGui::InputText(
      label,
      value.data(),
      value.capacity(),
      flags | ImGuiInputTextFlags_CallbackResize,
      +callback,
      static_cast<void*>(&value)
    );
  }
};

} // namespace imsql::ui
