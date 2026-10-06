#pragma once

#include "../Widget.h"

namespace mc_rtc::imgui
{

template<typename DataT>
struct SingleInput : public Widget
{
  inline SingleInput(Client & client, const ElementId & id) : Widget(client, id) {}

  ~SingleInput() override = default;

  inline void data(const DataT & data)
  {
    if(!busy_)
    {
      data_ = data;
      setupBuffer();
    }
  }

  virtual void setupBuffer() {}

  virtual DataT dataFromBuffer() = 0;

  template<typename ImGuiFn, typename... Args>
  void draw2D(ImGuiFn fn, Args &&... args)
  {
    ImGui::BeginTable(label("", "Table").c_str(), 3, ImGuiTableFlags_SizingStretchProp);
    ImGui::TableNextColumn();
    ImGui::Text("%s", id.name.c_str());
    ImGui::TableNextColumn();
    ImGui::Text("");

    ImGui::TableNextColumn();
    bool entered = fn(label("", "Input").c_str(), std::forward<Args>(args)..., ImGuiInputTextFlags_EnterReturnsTrue);
    busy_ = ImGui::IsItemActive();

    if(entered || (ImGui::IsItemDeactivatedAfterEdit() && (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter))))
    {
      auto nData = dataFromBuffer();
      if(nData != data_)
      {
        data_ = nData;
        client.send_request(id, data_);
      }
    }
    else if(ImGui::IsItemDeactivated())
    {
      setupBuffer(); // Revert to data_ when focus is lost without enter
    }
    ImGui::EndTable();
  }

protected:
  bool busy_ = false;
  DataT data_;
};

} // namespace mc_rtc::imgui
