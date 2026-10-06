#pragma once

#include "details/SingleInput.h"

namespace mc_rtc::imgui
{

struct StringInput : public SingleInput<std::string>
{
  inline StringInput(Client & client, const ElementId & id) : SingleInput<std::string>(client, id)
  {
    buffer_.resize(256, '\0');
  }

  ~StringInput() override = default;

  void setupBuffer() override
  {
    buffer_.resize(std::max<size_t>(256, data_.size() + 1));
    std::memcpy(&buffer_[0], data_.c_str(), data_.size() + 1);
  }

  std::string dataFromBuffer() override { return {buffer_.data(), strnlen(buffer_.data(), buffer_.size())}; }

  inline void draw2D() override
  {
    auto InputText = [](const char * label, char * buffer, size_t len, int flags)
    { return ImGui::InputText(label, buffer, len, flags); };
    SingleInput::draw2D(InputText, buffer_.data(), buffer_.size());
  }

private:
  std::vector<char> buffer_;
};

} // namespace mc_rtc::imgui
