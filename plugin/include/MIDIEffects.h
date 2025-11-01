// Advanced MIDI effects: Arpeggiator, Humanizer, Probability
#pragma once

#include <vector>
#include <cstdint>

namespace scalechord {

enum class ArpeggiatorMode {
    Up,           // Play lowest to highest
    Down,         // Play highest to lowest
    UpDown,       // Up then down
    Random,       // Random order
    Hold,         // Static chord (no arpeggio)
    Strum,        // Quick strum-like effect
};

enum class ArpeggiatorSync {
    Tempo16th,    // 16th note
    Tempo8th,     // 8th note
    TempoQuarter, // Quarter note
    TempoHalf,    // Half note
    Freerun,      // No sync to tempo
};

struct ArpeggiatorSettings {
    ArpeggiatorMode mode = ArpeggiatorMode::Up;
    ArpeggiatorSync sync = ArpeggiatorSync::Tempo16th;
    float tempoHz = 2.0f;  // fallback for Freerun, in Hz (notes per second)
    int octaveRange = 1;   // how many octaves to span
    bool restartOnNewNote = true;
    float swing = 0.0f;    // 0.5 = 50% swing
};

class Arpeggiator {
public:
    Arpeggiator() = default;
    explicit Arpeggiator(const ArpeggiatorSettings& s);
    
    void setSettings(const ArpeggiatorSettings& s);
    ArpeggiatorSettings getSettings() const noexcept { return settings_; }
    
    // Update chord notes
    void setChordNotes(const std::vector<int>& notes);
    
    // Process: returns note that should be played now (or -1 if none)
    int process(float sampleRate, float tempoHz = 120.0f);
    
    // Reset internal state
    void reset();
    
    // Get current step
    int getCurrentStep() const noexcept { return currentStep_; }
    
private:
    ArpeggiatorSettings settings_;
    std::vector<int> chordNotes_;
    int currentStep_ = 0;
    float phaseFractional_ = 0.0f;  // 0.0 - 1.0
    float notePhase_ = 0.0f;
    
    int getNextNote();
    float getStepDuration(float sampleRate, float tempoHz);
};

struct HumanizerSettings {
    bool enabled = false;
    float timingVariation = 0.02f;  // ±2% of note timing
    float velocityVariation = 0.05f;  // ±5% of velocity
    float tuneDeviation = 0.02f;  // ±2 cents
};

class Humanizer {
public:
    Humanizer() = default;
    explicit Humanizer(const HumanizerSettings& s);
    
    void setSettings(const HumanizerSettings& s);
    HumanizerSettings getSettings() const noexcept { return settings_; }
    
    // Humanize a velocity value
    int humanizeVelocity(int velocity);
    
    // Humanize note timing (returns delay in samples)
    float humanizeNoteDelay(float sampleRate);
    
    // Humanize note pitch (returns cents deviation)
    float humanizePitch();
    
private:
    HumanizerSettings settings_;
};

struct NoteProbabilitySettings {
    float probability = 1.0f;  // 0.0 - 1.0 (probability to play each note)
    bool skipEveryNth = false;
    int skipEveryNthStep = 2;  // play every 2nd note, etc.
};

class NoteProbability {
public:
    NoteProbability() = default;
    explicit NoteProbability(const NoteProbabilitySettings& s);
    
    void setSettings(const NoteProbabilitySettings& s);
    NoteProbabilitySettings getSettings() const noexcept { return settings_; }
    
    // Check if this note should play
    bool shouldPlayNote(int stepIndex);
    
    // Reset
    void reset() { currentStep_ = 0; }
    
private:
    NoteProbabilitySettings settings_;
    int currentStep_ = 0;
};

} // namespace scalechord
