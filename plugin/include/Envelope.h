// Advanced ADSR Envelope with velocity sensitivity and humanization
#pragma once

#include <cmath>
#include <algorithm>

namespace scalechord {

enum class EnvelopeState {
    Idle,
    Attack,
    Decay,
    Sustain,
    Release
};

struct EnvelopeSettings {
    float attack = 10.0f;       // ms
    float decay = 100.0f;       // ms
    float sustain = 0.7f;       // 0.0 - 1.0
    float release = 300.0f;     // ms
    float velocitySensitivity = 0.5f;  // how much incoming velocity affects amplitude
    bool humanize = false;      // add subtle randomization
    float humanizeAmount = 0.05f;  // 5% variation
};

class Envelope {
public:
    Envelope() = default;
    explicit Envelope(const EnvelopeSettings& s);
    
    void setSettings(const EnvelopeSettings& s);
    EnvelopeSettings getSettings() const noexcept { return settings_; }
    
    // Call when a note starts
    void noteOn(int velocity = 64, float sampleRate = 44100.0f);
    
    // Call when a note ends
    void noteOff();
    
    // Process envelope, returns amplitude (0.0 - 1.0)
    // Call once per sample or at your desired time resolution
    float process();
    
    // Get current state
    EnvelopeState getState() const noexcept { return state_; }
    
    // Check if envelope is finished (idle)
    bool isFinished() const noexcept { return state_ == EnvelopeState::Idle; }
    
    // Set sample rate for time calculations
    void setSampleRate(float sampleRate) noexcept { sampleRate_ = sampleRate; }
    
private:
    EnvelopeSettings settings_;
    EnvelopeState state_ = EnvelopeState::Idle;
    
    float sampleRate_ = 44100.0f;
    float currentAmplitude_ = 0.0f;
    float targetAmplitude_ = 1.0f;
    float stateStartTime_ = 0.0f;
    float elapsedTime_ = 0.0f;
    
    float attackIncrement_ = 0.0f;
    float decayIncrement_ = 0.0f;
    float releaseIncrement_ = 0.0f;
    
    void updateIncrements();
    float calculateCurve(float t, float duration, bool exponential = true) const;
};

} // namespace scalechord
