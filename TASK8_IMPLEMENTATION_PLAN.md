# Task 8: UI Framework - Implementation Plan

**Date**: November 1, 2025  
**Task**: Build JUCE-based plugin editor with comprehensive UI  
**Status**: STARTING  
**Estimated Duration**: 6-8 hours  
**Timeline**: This week to v1.0

---

## 🎯 Task 8 Objectives

### Primary Goals
1. ✅ Create JUCE AudioProcessorEditor subclass
2. ✅ Build parameter control sliders (12 parameters)
3. ✅ Implement preset browser with filtering
4. ✅ Add spectrum visualization (from PerformanceDashboard)
5. ✅ Integrate real-time metrics display
6. ✅ Full parameter automation support
7. ✅ Professional UI/UX design

---

## 📋 Architecture Overview

### Component Structure

```
PluginEditor (AudioProcessorEditor)
├── Parameter Control Section
│   ├── 12 Parameter Sliders (with labels)
│   ├── Value displays
│   └── Automation indicators
├── Preset Management Section
│   ├── Preset browser
│   ├── Category filter
│   ├── Save/Load controls
│   └── Recent presets
├── Spectrum Visualization
│   ├── 32-band spectrum display
│   ├── Real-time updates
│   └── Peak detection
└── Metrics Dashboard
    ├── Voice meter
    ├── CPU indicator
    ├── Latency display
    └── Health score

```

### Parameter Layout (12 parameters)

**Basic Parameters**:
1. Key (0-11: C-B)
2. Scale (0-14: 15 different scales)
3. Voicing (0-4: 5 voicing types)
4. Octave Offset (-2 to +2)
5. ADSR - Attack (0-1000 ms)
6. ADSR - Decay (0-1000 ms)
7. ADSR - Sustain (0-1 level)
8. ADSR - Release (0-2000 ms)

**Effect Parameters**:
9. Effects Master (0-1: dry/wet)
10. Reverb Amount (0-1)
11. Delay Time (50-500 ms)
12. Chorus Depth (0-1)

---

## 🎨 UI Layout Design

### Window Size
- **Default**: 1000 x 700 pixels
- **Min**: 800 x 600 pixels
- **Max**: 1400 x 900 pixels
- **Aspect Ratio**: 10:7 (maintained)

### Layout Grid

```
┌─────────────────────────────────────────────────────────┐
│  ScaleChord VST3 Plugin v1.0                       [_][□][×] │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌──────────────────┐  ┌──────────────────────────┐   │
│  │  Spectrum Graph  │  │  Metrics Dashboard      │   │
│  │  (32-band)       │  │  • Voices: 8/16         │   │
│  │                  │  │  • CPU: 12.3%           │   │
│  │                  │  │  • Latency: 2.1ms       │   │
│  │                  │  │  • Health: 92%          │   │
│  └──────────────────┘  └──────────────────────────┘   │
│                                                         │
│  ┌──────────────────────────────────────────────────┐  │
│  │  Parameters                                      │  │
│  │  Key: C [────○────] │ Scale: Major [────○────]  │  │
│  │  Voicing: Triad [────○────] │ Octave: 0 [──○──] │  │
│  │  Attack: 10ms [────○────] │ Decay: 50ms [──○──] │  │
│  │  Sustain: 1.0 [────○────] │ Release: 100ms [─○─]│  │
│  │  Effects: 0.5 [────○────] │ Reverb: 0.3 [──○──] │  │
│  │  Delay: 200ms [────○────] │ Chorus: 0.5 [──○──] │  │
│  └──────────────────────────────────────────────────┘  │
│                                                         │
│  ┌──────────────────────────────────────────────────┐  │
│  │  Presets                                         │  │
│  │  [Category ▼] [Recent ▼] [Save] [Load] [New]   │  │
│  │  ┌────────────────────────────────────────────┐ │  │
│  │  │ Default                                    │ │  │
│  │  │ Bright Blues                               │ │  │
│  │  │ Smooth Jazz                                │ │  │
│  │  │ Ambient Pad                                │ │  │
│  │  │ Bright Lead                                │ │  │
│  │  └────────────────────────────────────────────┘ │  │
│  └──────────────────────────────────────────────────┘  │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

---

## 🔧 Implementation Phases

### Phase 1: Core Editor Structure (1 hour)
- [ ] Create PluginEditor.h header
- [ ] Create PluginEditor.cpp implementation
- [ ] Set up APVTS (AudioProcessorValueTreeState)
- [ ] Define parameter layout
- [ ] Initialize JUCE components

### Phase 2: Parameter Controls (1.5 hours)
- [ ] Create slider components for all 12 parameters
- [ ] Add labels and value displays
- [ ] Implement parameter attachment (APVTS binding)
- [ ] Add automation indicators
- [ ] Style sliders professionally

### Phase 3: Spectrum Visualization (1 hour)
- [ ] Query PerformanceDashboard for spectrum data
- [ ] Create spectrum component with 32 bands
- [ ] Implement real-time updates
- [ ] Add frequency axis labels
- [ ] Add dB scale display

### Phase 4: Metrics Dashboard (1 hour)
- [ ] Create metrics display panel
- [ ] Voice meter implementation
- [ ] CPU indicator bar
- [ ] Latency display with warning threshold
- [ ] Health score visualization

### Phase 5: Preset Browser (1.5 hours)
- [ ] List presets from PresetManager
- [ ] Add category filtering
- [ ] Add sorting options
- [ ] Implement load/save dialogs
- [ ] Show recent presets

### Phase 6: Polish & Testing (1 hour)
- [ ] Professional styling
- [ ] Responsive layout
- [ ] Performance optimization
- [ ] Testing in different sizes
- [ ] Final adjustments

---

## 📁 Files to Create/Modify

### New Files

**PluginEditor.h** (400-500 lines)
- AudioProcessorEditor subclass
- Component declarations
- Timer for UI updates
- Data structures for state

**PluginEditor.cpp** (600-700 lines)
- Constructor and destructor
- resized() implementation
- paint() methods for custom drawing
- Button/slider callbacks
- Timer callback for updates
- Helper methods

### Modified Files

**CMakeLists.txt**
- Add PluginEditor.cpp to sources
- Add PluginEditor.h to includes

**JUCEPluginProcessor.h/cpp**
- Add getEditor() override
- Ensure parameter structure aligned

---

## 🎨 Color Scheme

### Professional Audio Plugin Palette
- **Background**: Dark gray (#1a1a1a)
- **Panels**: Medium gray (#2d2d2d)
- **Accents**: Bright cyan (#00d4ff)
- **Text**: Light gray (#e0e0e0)
- **Highlights**: Gold (#ffd700)
- **Warnings**: Red (#ff4444)
- **Success**: Green (#44ff44)

### Component Styling
- **Sliders**: Modern thin design with highlight
- **Buttons**: Rounded rectangles with hover effects
- **Text**: Clear sans-serif font (Helvetica/Arial)
- **Meters**: Gradient bars (cool → warm)

---

## 🔗 Integration Points

### With PluginProcessor
```cpp
// Get reference to processor
auto* processor = dynamic_cast<PluginProcessor*>(getAudioProcessor());

// Access dashboard for metrics
auto& dashboard = processor->getDashboard();
PerformanceDashboard::DashboardSnapshot snap = dashboard->getSnapshot();

// Access preset manager
auto& presets = processor->getPresetManager();
std::vector<std::string> names = presets.getPresetNames();
```

### With APVTS
```cpp
// Create parameter layout
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterInt>(
        "key", "Key", 0, 11, 0));
    layout.add(std::make_unique<juce::AudioParameterInt>(
        "scale", "Scale", 0, 14, 0));
    // ... more parameters
    return layout;
}

// In editor, attach sliders
std::unique_ptr<juce::SliderAttachment> keySliderAttachment;
keySliderAttachment = 
    std::make_unique<juce::SliderAttachment>(
        processorRef.getAPVTS(), "key", keySlider);
```

### With PerformanceDashboard
```cpp
// Update metrics in timer callback
void PluginEditor::timerCallback()
{
    auto snapshot = processor->getDashboard().getSnapshot();
    
    voiceLabel.setText(
        std::to_string(snapshot.voices.activeVoiceCount) + "/16",
        juce::dontSendNotification);
        
    cpuLabel.setText(
        PerformanceDashboard::formatPercent(snapshot.cpu.currentCPU),
        juce::dontSendNotification);
        
    // Update spectrum
    updateSpectrumDisplay(snapshot.spectrum);
}
```

---

## 🧪 Testing Strategy

### Functional Testing
- [ ] All 12 parameters respond to changes
- [ ] Parameter automation working
- [ ] Presets load/save correctly
- [ ] Spectrum updates in real-time
- [ ] Metrics display accurate

### UI Testing
- [ ] Window resizes responsively
- [ ] All controls visible at min size
- [ ] Layout scales at max size
- [ ] Text readable at all sizes
- [ ] Colors consistent with design

### Performance Testing
- [ ] Editor paint() < 5ms
- [ ] Timer callback < 1ms
- [ ] No UI stuttering at high CPU
- [ ] Smooth parameter movement
- [ ] Real-time spectrum updates smooth

### Integration Testing
- [ ] Works with PluginProcessor
- [ ] Dashboard metrics accessible
- [ ] PresetManager integration functional
- [ ] APVTS properly connected
- [ ] Automation working

---

## 📚 JUCE Components to Use

### Sliders
```cpp
juce::Slider paramSlider;
paramSlider.setSliderStyle(juce::Slider::LinearHorizontal);
paramSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
paramSlider.setRange(0, 100);
paramSlider.setValue(50);
```

### Labels
```cpp
juce::Label paramLabel;
paramLabel.setText("Parameter", juce::dontSendNotification);
paramLabel.setColour(juce::Label::textColourId, juce::Colours::white);
```

### Buttons
```cpp
juce::TextButton saveButton("Save");
saveButton.setClickingTogglesState(false);
saveButton.onClick = [this] { savePreset(); };
```

### ListBox
```cpp
juce::ListBox presetList(nullptr, this);
presetList.setModel(this);
presetList.setMultipleSelectionEnabled(false);
```

### Custom Components
```cpp
class SpectrumAnalyzer : public juce::Component
{
public:
    void paint(juce::Graphics& g) override;
    void setSpectrumData(const std::array<float, 32>& data);
private:
    std::array<float, 32> spectrumData;
};
```

---

## 🚀 Quick Start Commands

```bash
# After implementation, build
cd plugin/build
cmake ..
cmake --build . -j4

# Run plugin processor with editor
./scalechord_demo

# Test in DAW (if available)
# Copy .vst3 to DAW plugin directory
```

---

## ✅ Completion Checklist

- [ ] PluginEditor.h created (400+ lines)
- [ ] PluginEditor.cpp implemented (600+ lines)
- [ ] 12 parameter sliders working
- [ ] Parameter values display correctly
- [ ] APVTS attached and functional
- [ ] Spectrum visualization working
- [ ] Metrics dashboard displaying
- [ ] Preset browser functional
- [ ] Window responsive to resizing
- [ ] Professional styling applied
- [ ] All controls accessible
- [ ] Performance acceptable
- [ ] Integration verified
- [ ] Testing complete
- [ ] Documentation written

---

## 📊 Expected Outcome

**By End of Task 8**:
- ✅ Complete professional UI for ScaleChord plugin
- ✅ All 12 parameters controllable from UI
- ✅ Real-time visualization of audio analysis
- ✅ Preset management fully functional
- ✅ Performance metrics visible to user
- ✅ Ready for DAW integration testing
- ✅ 90% project completion (9 of 10 tasks)

**Then**: Integration Testing → v1.0 Release

---

**Status**: ✅ **READY TO IMPLEMENT**

Next: Create PluginEditor.h structure
