#pragma once

#include "Widget.h"

namespace mc_rtc::imgui
{

struct ArrayInput : public Widget
{
  inline ArrayInput(Client & client, const ElementId & id) : Widget(client, id) {}

  ~ArrayInput() override = default;

  inline void data(const std::vector<std::string> & labels, const Eigen::VectorXd & data)
  {
    if(!busy_)
    {
      labels_ = labels;
      data_ = data;
      buffer_ = data_;
    }
  }

  inline void draw2D() override
  {
    ImGui::Text("%s", id.name.c_str());
    ImGui::BeginTable(label("", "_table_data").c_str(), data_.size(), ImGuiTableFlags_SizingStretchProp);
    if(labels_.size())
    {
      for(size_t i = 0; i < std::min<size_t>(labels_.size(), data_.size()); ++i)
      {
        const auto & l = labels_[i];
        ImGui::TableNextColumn();
        ImGui::Text("%s", l.c_str());
      }
    }
    ImGui::TableNextRow();
    
    bool entered = false;
    bool deactivated = false;
    busy_ = false;

    for(int i = 0; i < data_.size(); ++i)
    {
      ImGui::TableNextColumn();
      entered |= ImGui::InputDouble(label("", i).c_str(), &buffer_[i], 0.0, 0.0, "%.6g", ImGuiInputTextFlags_EnterReturnsTrue);
      busy_ |= ImGui::IsItemActive();
      deactivated |= ImGui::IsItemDeactivated();
      entered |= (ImGui::IsItemDeactivatedAfterEdit() && (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)));
    }
    ImGui::EndTable();

    if(entered)
    {
      if(buffer_ != data_)
      {
        data_ = buffer_;
        client.send_request(id, data_);
      }
    }
    else if(deactivated && !busy_)
    {
      buffer_ = data_;
    }
  }

private:
  bool busy_ = false;
  std::vector<std::string> labels_;
  Eigen::VectorXd data_;
  Eigen::VectorXd buffer_;
};

} // namespace mc_rtc::imgui
