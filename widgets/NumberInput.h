#pragma once

#include "details/SingleInput.h"

namespace mc_rtc::imgui
{

struct NumberInput : public SingleInput<double>
{
  inline NumberInput(Client & client, const ElementId & id) : SingleInput<double>(client, id) {}

  ~NumberInput() override = default;

  void setupBuffer() override { buffer_ = data_; }

  double dataFromBuffer() override { return buffer_; }

  inline void draw2D() override
  {
    SingleInput::draw2D(ImGui::InputDouble, &buffer_, 0.0, 0.0, "%.6g");
    if(ImGui::IsItemHovered())
    {
      ImGui::BeginTooltip();
      ImGui::Text("%f", buffer_);
      ImGui::EndTooltip();
    }
  }

private:
  double buffer_;
};

} // namespace mc_rtc::imgui
