#pragma once

#include <cstdint>
#include <string>

// Conditional JUCE headers - graceful fallback if JUCE not available
#ifdef JUCE_VERSION
    #include <juce_audio_processors/juce_audio_processors.h>
    #include <juce_gui_basics/juce_gui_basics.h>
    using RectInt = juce::Rectangle<int>;
#else
    namespace juce {
        // Define all classes in dependency order
        class Colour { 
            public: 
                Colour() {} 
                explicit Colour(uint32_t c) {} 
                Colour interpolatedWith(const Colour&, float) const { return *this; }
        };
        
        class Font { 
            public: 
                Font() {}
                explicit Font(float s) {} 
                Font boldened() const { return *this; }
        };
        
        class String {
            public: 
                String() {} 
                explicit String(int i) {} 
                explicit String(const char* c) {}
                String(const String&, float f, int places) {}
                explicit String(const std::string& s) {}
        };
        
        class RectInt {
            public:
                RectInt() {}
                int getWidth() const { return 0; }
                int getHeight() const { return 0; }
                int getX() const { return 0; }
                int getY() const { return 0; }
                int getBottomY() const { return 0; }
                RectInt removeFromLeft(int w) const { return RectInt(); }
                RectInt removeFromRight(int w) const { return RectInt(); }
                RectInt removeFromTop(int h) const { return RectInt(); }
                RectInt removeFromBottom(int h) const { return RectInt(); }
                RectInt withSizeKeepingPosition(int w, int h) const { return RectInt(); }
        };
        
        class Graphics { 
            public: 
                Graphics() {}
                virtual ~Graphics() {} 
                void fillAll(const Colour&) {}
                void setColour(const Colour&) {}
                void drawRect(const RectInt&, int) {}
                void setFont(const Font&) {}
                void drawText(const String&, const RectInt&, int) {}
                void drawText(const String&, int, int, int, int, int) {}  // x,y,w,h format
                void fillRect(int, int, int, int) {}
                void drawLine(int, int, int, int, float) {}
        };
        
        class Slider { 
            public: 
                enum DragMode { RotaryVerticalDrag = 0, NoTextBox = 1 };
                Slider() {}
                explicit Slider(int mode, int textMode) {}
                void setRange(double a, double b, double c) {}
                void setValue(double v, void* n = nullptr) {}
                void setColour(int id, const Colour& c) {}
                void setBounds(int x, int y, int w, int h) {}
                enum { thumbColourId = 0, rotarySliderFillColourId = 1, rotarySliderOutlineColourId = 2 };
        };
        
        class Label { 
            public: 
                Label() {}
                explicit Label(const String& name) {}
                Label(const String& name, const String& text) {}
                void setFont(const Font& f) {}
                void setColour(int id, const Colour& c) {}
                void setJustificationType(int j) {}
                void setBounds(int x, int y, int w, int h) {}
                enum { textColourId = 0 };
        };
        
        class AudioProcessorEditor { 
            public: 
                AudioProcessorEditor() {}
                explicit AudioProcessorEditor(void* p) {} 
                virtual ~AudioProcessorEditor() {}
                virtual void paint(Graphics&) {}
                virtual void resized() {}
                void setSize(int w, int h) {}
                void setResizable(bool a, bool b) {}
                void addAndMakeVisible(Slider&) {}
                RectInt getLocalBounds() const { return RectInt(); }
                int getWidth() const { return 0; }
                int getHeight() const { return 0; }
        };
        
        class Timer { 
            public: 
                Timer() {}
                virtual ~Timer() {}
                virtual void timerCallback() {}
                void startTimer(int ms) {}
                void stopTimer() {}
        };
        
        struct SliderAttachment {
            SliderAttachment() {}
            template<typename... Args>
            SliderAttachment(Args&&...) {}
        };
        
        class AudioProcessorValueTreeState {
            public:
                using SliderAttachment = juce::SliderAttachment;
        };
        
        enum Justification { centred = 0, left = 1, centredTop = 2 };
        struct dontSendNotification { };
    }
    using RectInt = juce::RectInt;
#endif

#include <memory>
#include <vector>
#include "PerformanceDashboard.h"
#include "PresetManager.h"

class PluginProcessor;



// ============================================================================
// UI Theme & Color Constants
// ============================================================================
namespace ScaleChordTheme {
    constexpr uint32_t Background = 0xFF1a1a1a;      // Dark background
    constexpr uint32_t Panel = 0xFF2d2d2d;           // Panel background
    constexpr uint32_t SliderThumb = 0xFF0099ff;     // Bright blue
    constexpr uint32_t SpectrumLow = 0xFF00ffff;     // Cyan
    constexpr uint32_t SpectrumMid = 0xFF00ff00;     // Green
    constexpr uint32_t SpectrumHigh = 0xFFff0000;    // Red
    constexpr uint32_t TextPrimary = 0xFFffffff;     // White
    constexpr uint32_t TextSecondary = 0xFF888888;   // Gray
    constexpr uint32_t HealthGood = 0xFF44ff44;      // Green
    constexpr uint32_t HealthWarning = 0xFFffaa00;   // Orange
    constexpr uint32_t HealthBad = 0xFFff4444;       // Red
}

// ============================================================================
// PluginEditor - Main UI Class with JUCE Integration
// ============================================================================
class PluginEditor : public juce::AudioProcessorEditor,
                     public juce::Timer
{
public:
    explicit PluginEditor(PluginProcessor* ownerFilter);
    ~PluginEditor() override;

    // ===== Component Lifecycle =====
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // ===== Timer Callbacks for Real-Time Updates =====
    void timerCallback() override;

    // ===== Parameter Updates =====
    void updateParameterDisplay(int paramId, float value);
    void updateMetricsDisplay();

private:
    // ===== Audio Processor Reference =====
    PluginProcessor* processor;

    // ===== UI Component Containers =====
    std::vector<std::unique_ptr<juce::Slider>> paramSliders;
    std::vector<std::unique_ptr<juce::Label>> paramLabels;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;

    // ===== Spectrum Analyzer Data =====
    std::vector<float> spectrumBands;     // 32 frequency bands (current)
    std::vector<float> spectrumPeaks;     // 32 frequency peak holds
    std::vector<int> peakHoldCounters;    // Samples held at peak for each band
    static constexpr int SPECTRUM_BANDS = 32;
    static constexpr float SPECTRUM_DECAY = 0.85f;     // Decay factor per frame
    static constexpr float SPECTRUM_RISE = 1.2f;       // Rise factor when new data arrives
    static constexpr int PEAK_HOLD_FRAMES = 30;        // Frames to hold peak (500ms at 60 FPS)
    static constexpr float FREQUENCY_FLOOR = 20.0f;    // Lowest frequency (Hz)
    static constexpr float FREQUENCY_CEIL = 20000.0f;  // Highest frequency (Hz)

    // ===== Metrics Display Data =====
    struct MetricsDisplay {
        int voiceCount = 0;
        float cpuUsage = 0.0f;
        float latencyMs = 0.0f;
        float healthScore = 0.0f;
        std::string statusText = "READY";
    } metrics;

    // ===== Internal Methods =====
    void initializeSliders();
    void initializeLabels();
    void bindParametersToAPVTS();
    void updateSpectrumAnalyzer();
    void updateSpectrumWithPeaks();
    void drawSpectrumBands(juce::Graphics& g, const RectInt& area);
    void drawSpectrumPeaks(juce::Graphics& g, const RectInt& area);
    void drawMetricsPanel(juce::Graphics& g, const RectInt& area);
    void drawParameterSection(juce::Graphics& g, const RectInt& area);
    void layoutPanels(int totalWidth, int totalHeight, RectInt& leftPanel, 
                      RectInt& centerPanel, RectInt& rightPanel);
    juce::Colour getSpectrumColour(float normalized);
    juce::Colour getHealthColour(float healthScore);

    // ===== Layout Constants (Responsive) =====
    static constexpr int WINDOW_WIDTH = 1000;
    static constexpr int WINDOW_HEIGHT = 700;
    static constexpr int MIN_WIDTH = 800;
    static constexpr int MIN_HEIGHT = 600;
    static constexpr int SLIDER_WIDTH = 50;
    static constexpr int SLIDER_HEIGHT = 180;
    static constexpr int LABEL_HEIGHT = 20;
    static constexpr int PANEL_MARGIN = 10;
    static constexpr int SPECTRUM_HEIGHT = 150;
    static constexpr int METRICS_HEIGHT = 180;
    
    // Panel width ratios (left:center:right = 3:4:3 by default)
    static constexpr float LEFT_PANEL_RATIO = 0.3f;
    static constexpr float CENTER_PANEL_RATIO = 0.4f;
    static constexpr float RIGHT_PANEL_RATIO = 0.3f;

    // ===== Parameter IDs (must match PluginProcessor) =====
    enum ParameterIds {
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
        NumParams = 12
    };

    // ===== Parameter Names and Labels =====
    static constexpr const char* parameterNames[NumParams] = {
        "Key", "Scale", "Voicing", "Octave",
        "Attack", "Decay", "Sustain", "Release",
        "Effect", "Reverb", "Delay", "Chorus"
    };

private:
    // Prevent copying
    PluginEditor(const PluginEditor&) = delete;
    PluginEditor& operator=(const PluginEditor&) = delete;
};
