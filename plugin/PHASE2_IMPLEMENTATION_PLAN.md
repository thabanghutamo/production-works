# Phase 2: Core Editor Implementation Plan
## Task 8 UI Framework - Part 2 of 7

**Status**: ⏳ READY TO IMPLEMENT  
**Estimated Time**: 1-2 hours  
**Prerequisites**: JUCE 7.0+ (optional, can use mock mode)  
**Date**: November 1-2, 2025

---

## 📋 Phase 2 Objectives

1. Create functional JUCE AudioProcessorEditor subclass
2. Implement basic window layout and rendering
3. Add event handling framework
4. Create parameter binding infrastructure
5. Integrate PerformanceDashboard metrics display

---

## 🏗️ Implementation Strategy

### Step 1: Update PluginEditor.h (Header)

**Current State** (91 lines, stub):
```cpp
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
class ScaleChordPlugin;

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    PluginEditor(ScaleChordPlugin* ownerFilter);
    ~PluginEditor();
    
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ScaleChordPlugin* processor;
};
```

**Target State** (250+ lines, functional):
- Inherit from `juce::AudioProcessorEditor`
- Add member variables for UI components (sliders, spectrum, etc.)
- Add layout and rendering methods
- Add parameter listener callbacks
- Add performance metric updates

**Changes**:
```cpp
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PerformanceDashboard.h"
#include "PresetManager.h"

class ScaleChordPlugin;

// ========================================
// UI Color Scheme
// ========================================
namespace UIColors {
    constexpr auto Background = 0xFF1a1a1a;
    constexpr auto SliderThumb = 0xFF0099ff;
    constexpr auto SpectrumLow = 0xFF00ffff;
    constexpr auto SpectrumMid = 0xFF00ff00;
    constexpr auto SpectrumHigh = 0xFFff0000;
    constexpr auto TextPrimary = 0xFFffffff;
    constexpr auto TextSecondary = 0xFF888888;
}

// ========================================
// PluginEditor - Main UI Class
// ========================================
class PluginEditor : public juce::AudioProcessorEditor,
                     public juce::Timer
{
public:
    explicit PluginEditor(ScaleChordPlugin* ownerFilter);
    ~PluginEditor() override;
    
    // ===== Component Lifecycle =====
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // ===== Timer Callbacks =====
    void timerCallback() override;
    
    // ===== Parameter Updates =====
    void updateParameterDisplay(int paramId, float value);
    void updateMetricsDisplay();
    
    // ===== Preset Management =====
    void loadPreset(const PresetManager::Preset& preset);
    void saveCurrentPreset(const std::string& name);

private:
    // ===== Audio Processor Reference =====
    ScaleChordPlugin* processor;
    
    // ===== UI Component Containers =====
    std::vector<juce::Slider> parameterSliders;
    std::vector<juce::Label> parameterLabels;
    
    // ===== Spectrum Analyzer =====
    std::vector<float> spectrumBands;  // 32 bands
    
    // ===== Metrics Dashboard =====
    struct MetricsDisplay {
        int voiceCount = 0;
        float cpuUsage = 0.0f;
        float latencyMs = 0.0f;
        float healthScore = 0.0f;
    } metrics;
    
    // ===== Internal Methods =====
    void initializeSliders();
    void initializeLabels();
    void createLayout();
    void bindParametersToAPVTS();
    void updateSpectrumAnalyzer();
    void drawSpectrumBands(juce::Graphics& g);
    void drawMetricsPanel(juce::Graphics& g);
    void drawPresetBrowser(juce::Graphics& g);
    
    // ===== Layout Constants =====
    static constexpr int WINDOW_WIDTH = 1000;
    static constexpr int WINDOW_HEIGHT = 700;
    static constexpr int SLIDER_WIDTH = 60;
    static constexpr int SLIDER_HEIGHT = 200;
    static constexpr int SPECTRUM_BANDS = 32;
    
    // ===== Parameter Indices (from PluginProcessor) =====
    enum ParamIds {
        ParamKeyId = 0,
        ParamScaleId = 1,
        ParamVoicingId = 2,
        ParamOctaveId = 3,
        ParamAttackId = 4,
        ParamDecayId = 5,
        ParamSustainId = 6,
        ParamReleaseId = 7,
        ParamEffectStrengthId = 8,
        ParamReverbLevelId = 9,
        ParamDelayLevelId = 10,
        ParamChorusLevelId = 11,
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
```

### Step 2: Implement PluginEditor.cpp (Implementation)

**File Structure** (500-600 lines target):
1. Constructor (50 lines)
2. Destructor (5 lines)
3. paint() - main rendering (100 lines)
4. resized() - layout management (80 lines)
5. timerCallback() - real-time updates (50 lines)
6. Slider initialization (60 lines)
7. Label initialization (40 lines)
8. Parameter binding (50 lines)
9. Spectrum analyzer (40 lines)
10. Metrics display (40 lines)

**Key Implementations**:

```cpp
// Constructor
PluginEditor::PluginEditor(ScaleChordPlugin* ownerFilter)
    : AudioProcessorEditor(ownerFilter),
      processor(ownerFilter)
{
    setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setResizable(true, true);
    
    // Initialize spectrum data
    spectrumBands.resize(SPECTRUM_BANDS, 0.0f);
    
    // Initialize sliders
    initializeSliders();
    initializeLabels();
    bindParametersToAPVTS();
    
    // Start UI update timer (60 FPS)
    startTimer(16);  // ~16ms = 60 FPS
}

// Paint method - main rendering
void PluginEditor::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(UIColors::Background));
    
    // Layout: 3 columns
    // Left: Parameter sliders (300 px)
    // Center: Spectrum analyzer (400 px)
    // Right: Metrics dashboard (300 px)
    
    // Left Panel - Parameters
    juce::Rectangle<int> leftPanel(0, 0, 300, getHeight());
    g.setColour(juce::Colour(UIColors::TextPrimary));
    g.drawText("PARAMETERS", leftPanel, juce::Justification::centredTop);
    
    // Center Panel - Spectrum
    juce::Rectangle<int> centerPanel(300, 0, 400, getHeight());
    drawSpectrumBands(g);
    
    // Right Panel - Metrics
    juce::Rectangle<int> rightPanel(700, 0, getWidth() - 700, getHeight());
    drawMetricsPanel(g);
}

// Resized - layout management
void PluginEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Layout: 3 columns
    auto leftArea = bounds.removeFromLeft(getWidth() / 3);
    auto rightArea = bounds.removeFromRight(getWidth() / 3);
    auto centerArea = bounds;
    
    // Arrange sliders vertically in left area
    int yOffset = 40;
    const int sliderSpacing = 60;
    for (auto& slider : parameterSliders) {
        slider.setBounds(leftArea.withY(yOffset).reduced(10).withHeight(40));
        yOffset += sliderSpacing;
    }
}

// Timer callback - real-time updates
void PluginEditor::timerCallback()
{
    updateMetricsDisplay();
    updateSpectrumAnalyzer();
    repaint();
}
```

### Step 3: Create Parameter Infrastructure

**APVTS (AudioProcessorValueTreeState) Setup**:
- Create 12 parameters (Key, Scale, Voicing, Octave, ADSR, Effects)
- Bind each slider to APVTS
- Set normalized ranges (0.0 - 1.0)
- Add value change listeners

### Step 4: Integrate PerformanceDashboard

**Metrics Display**:
- Voice count (0-16 active voices)
- CPU usage percentage (0-100%)
- Latency in milliseconds
- Health score (0-1.0, color-coded)
- Per-effect CPU breakdown

### Step 5: Spectrum Analyzer Integration

**32-Band Frequency Display**:
- Get spectrum data from PerformanceDashboard
- Render as vertical bars with gradient
- Colors: Cyan (low) → Green (mid) → Red (high)
- Update frequency: 60 FPS

---

## 🎨 UI Layout Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│  ScaleChord VST3 Plugin (1000x700)                              │
├─────────────────────┬──────────────┬──────────────────────────┤
│   PARAMETERS        │   SPECTRUM   │      METRICS             │
│   (300px wide)      │  ANALYZER    │     (300px wide)         │
│                     │ (32 bands)   │                          │
│ [Key] ─────●────   │              │ Voices:    4/16          │
│ [Scale] ──●──────  │              │ CPU:       3.2%          │
│ [Voicing] ─●──────  │   ▁▂▃▄▅▆▇█ │ Latency:   0.8 ms        │
│ [Octave] ────●────  │   ▁▂▃▄▅▆▇█ │ Health:    ▓▓▓▓░ 88%     │
│ [Attack] ────●────  │   ▁▂▃▄▅▆▇█ │                          │
│ [Decay] ─────●───   │   ▁▂▃▄▅▆▇█ │ Effects CPU:             │
│ [Sustain] ──●──────  │   ▁▂▃▄▅▆▇█ │ Reverb:    0.8%          │
│ [Release] ───●────  │   ▁▂▃▄▅▆▇█ │ Delay:     0.6%          │
│ [Fx Strength] ─●──  │   ▁▂▃▄▅▆▇█ │ Chorus:    0.4%          │
│ [Reverb] ────●────  │   ▁▂▃▄▅▆▇█ │                          │
│ [Delay] ─────●────  │   ▁▂▃▄▅▆▇█ │ Status: ✓ HEALTHY        │
│ [Chorus] ────●────  │   ▁▂▃▄▅▆▇█ │                          │
├─────────────────────┴──────────────┴──────────────────────────┤
│ [Load] [Save] [Delete] [Category: All ▼] [Preset Name...]   │
└─────────────────────────────────────────────────────────────────┘
```

---

## 📝 Implementation Checklist

### Header File (PluginEditor.h)
- [ ] Add color scheme constants
- [ ] Add class declaration with proper inheritance
- [ ] Add component member variables (sliders, labels)
- [ ] Add spectrum and metrics data members
- [ ] Add method declarations
- [ ] Add ParamIds enum
- [ ] Add layout constants

### Implementation File (PluginEditor.cpp)
- [ ] Constructor: Initialize JUCE components
- [ ] Destructor: Clean up resources
- [ ] initializeSliders(): Create all 12 sliders
- [ ] initializeLabels(): Create parameter labels
- [ ] bindParametersToAPVTS(): Connect to audio parameters
- [ ] paint(): Main rendering (background, spectrum, metrics)
- [ ] resized(): Layout management
- [ ] timerCallback(): Update metrics and spectrum
- [ ] updateMetricsDisplay(): Query PerformanceDashboard
- [ ] updateSpectrumAnalyzer(): Get spectrum data
- [ ] drawSpectrumBands(): Render frequency bars
- [ ] drawMetricsPanel(): Render metrics display

### Integration
- [ ] Update CMakeLists.txt for JUCE audio_utils module
- [ ] Update PluginProcessor.cpp to expose parameters
- [ ] Test with mock headers (no JUCE)
- [ ] Test with full JUCE when available

---

## 🔄 Expected Outcomes

**After Phase 2 Completion**:
- ✅ Functional JUCE AudioProcessorEditor
- ✅ 12 parameter sliders visible and interactive
- ✅ Real-time metrics display working
- ✅ Basic spectrum analyzer rendering
- ✅ Parameter APVTS binding functional
- ✅ 60 FPS UI refresh rate
- ✅ Build remains clean (0 warnings/errors)
- ✅ All tests still passing

**UI Responsiveness**:
- Parameter changes: < 1ms response
- Metrics updates: 60 FPS (16ms between frames)
- Spectrum updates: 60 FPS
- CPU overhead: < 2% (target < 5% for full UI)

---

## 🚀 Next Phase (Phase 3)

After Phase 2 completion, proceed to Phase 3 (1-2 hours):
- Add interactive controls to spectrum analyzer
- Add frequency selection overlay
- Add scale/chord highlighting
- Implement drag-and-drop preset browser

---

## 📖 Files to Modify/Create

| File | Status | Lines | Purpose |
|------|--------|-------|---------|
| PluginEditor.h | UPDATE | 250+ | Header with UI components |
| PluginEditor.cpp | UPDATE | 500+ | Full implementation |
| CMakeLists.txt | MINOR | N/A | Add JUCE audio_utils if needed |
| PluginProcessor.h | REVIEW | N/A | Check parameter definitions |
| PluginProcessor.cpp | REVIEW | N/A | Check parameter handling |

---

## ⏱️ Timeline

- **Phase 2 Start**: Now (after JUCE setup)
- **Phase 2 Estimated**: 1-2 hours
- **Phase 2 End**: November 2, 2025
- **Phases 3-7**: November 3-4, 2025
- **v1.0 Release**: November 6-7, 2025

---

**Status**: ⏳ READY TO BEGIN  
**Next Action**: Set up JUCE (via setup_juce.sh) then type `continue`
