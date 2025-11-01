// Mock JUCE AudioProcessorEditor for stub compilation
#pragma once
#include <cstdint>
namespace juce {
  class Component { public: virtual ~Component() {} virtual void resized() {} };
  class AudioProcessorEditor : public Component {   public: AudioProcessorEditor(void* proc) {}   };
}
