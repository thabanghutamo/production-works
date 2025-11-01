// ADSR Envelope implementation
#include "Envelope.h"
#include <random>
#include <ctime>

namespace scalechord {

static std::mt19937 g_rng(static_cast<unsigned>(std::time(nullptr)));
static std::uniform_real_distribution<float> g_distribution(-1.0f, 1.0f);

Envelope::Envelope(const EnvelopeSettings& s) : settings_(s) {
    updateIncrements();
}

void Envelope::setSettings(const EnvelopeSettings& s) {
    settings_ = s;
    updateIncrements();
}

void Envelope::noteOn(int velocity, float sampleRate) {
    sampleRate_ = sampleRate;
    state_ = EnvelopeState::Attack;
    stateStartTime_ = 0.0f;
    elapsedTime_ = 0.0f;
    currentAmplitude_ = 0.0f;
    
    // Apply velocity sensitivity
    targetAmplitude_ = 1.0f;
    if (settings_.velocitySensitivity > 0.0f) {
        float velocityFactor = static_cast<float>(velocity) / 127.0f;
        targetAmplitude_ *= (1.0f - settings_.velocitySensitivity) + settings_.velocitySensitivity * velocityFactor;
    }
    
    updateIncrements();
}

void Envelope::noteOff() {
    if (state_ != EnvelopeState::Idle) {
        state_ = EnvelopeState::Release;
        stateStartTime_ = elapsedTime_;
    }
}

void Envelope::updateIncrements() {
    if (sampleRate_ <= 0) return;
    
    // Calculate increment per sample
    if (settings_.attack > 0) {
        attackIncrement_ = targetAmplitude_ / (settings_.attack * sampleRate_ / 1000.0f);
    } else {
        attackIncrement_ = targetAmplitude_;
    }
    
    if (settings_.decay > 0) {
        decayIncrement_ = (settings_.sustain - targetAmplitude_) / (settings_.decay * sampleRate_ / 1000.0f);
    }
    
    if (settings_.release > 0) {
        releaseIncrement_ = -settings_.sustain / (settings_.release * sampleRate_ / 1000.0f);
    } else {
        releaseIncrement_ = -settings_.sustain;
    }
}

float Envelope::calculateCurve(float t, float duration, bool exponential) const {
    if (duration <= 0) return 1.0f;
    
    float normalized = std::clamp(t / duration, 0.0f, 1.0f);
    
    if (exponential) {
        // Exponential curve (faster start, slower end)
        return 1.0f - std::exp(-2.0f * normalized);
    }
    
    // Linear fallback
    return normalized;
}

float Envelope::process() {
    elapsedTime_ += 1.0f / sampleRate_;
    float timeInState = elapsedTime_ - stateStartTime_;
    
    float humanizationFactor = 1.0f;
    if (settings_.humanize) {
        humanizationFactor = 1.0f + g_distribution(g_rng) * settings_.humanizeAmount;
    }
    
    switch (state_) {
        case EnvelopeState::Attack: {
            currentAmplitude_ += attackIncrement_ * humanizationFactor;
            if (currentAmplitude_ >= targetAmplitude_) {
                currentAmplitude_ = targetAmplitude_;
                state_ = EnvelopeState::Decay;
                stateStartTime_ = elapsedTime_;
            }
            break;
        }
        
        case EnvelopeState::Decay: {
            currentAmplitude_ += decayIncrement_;
            if (currentAmplitude_ <= settings_.sustain * targetAmplitude_) {
                currentAmplitude_ = settings_.sustain * targetAmplitude_;
                state_ = EnvelopeState::Sustain;
                stateStartTime_ = elapsedTime_;
            }
            break;
        }
        
        case EnvelopeState::Sustain: {
            currentAmplitude_ = settings_.sustain * targetAmplitude_;
            break;
        }
        
        case EnvelopeState::Release: {
            currentAmplitude_ += releaseIncrement_;
            if (currentAmplitude_ <= 0.0f) {
                currentAmplitude_ = 0.0f;
                state_ = EnvelopeState::Idle;
            }
            break;
        }
        
        case EnvelopeState::Idle:
        default:
            currentAmplitude_ = 0.0f;
            break;
    }
    
    return std::clamp(currentAmplitude_, 0.0f, 1.0f);
}

} // namespace scalechord
