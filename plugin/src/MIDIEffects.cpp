// MIDI Effects implementation
#include "MIDIEffects.h"
#include <random>
#include <ctime>
#include <cmath>
#include <algorithm>

namespace scalechord {

static std::mt19937 g_rng_effects(static_cast<unsigned>(std::time(nullptr)));
static std::uniform_real_distribution<float> g_dist_effects(0.0f, 1.0f);

// ============ ARPEGGIATOR ============

Arpeggiator::Arpeggiator(const ArpeggiatorSettings& s) : settings_(s) {}

void Arpeggiator::setSettings(const ArpeggiatorSettings& s) {
    settings_ = s;
}

void Arpeggiator::setChordNotes(const std::vector<int>& notes) {
    chordNotes_ = notes;
    std::sort(chordNotes_.begin(), chordNotes_.end());
    if (settings_.restartOnNewNote) {
        reset();
    }
}

void Arpeggiator::reset() {
    currentStep_ = 0;
    phaseFractional_ = 0.0f;
    notePhase_ = 0.0f;
}

float Arpeggiator::getStepDuration(float sampleRate, float tempoHz) {
    if (settings_.sync == ArpeggiatorSync::Freerun) {
        return sampleRate / settings_.tempoHz;
    }
    
    // Convert sync to multiplier
    float multiplier = 1.0f;
    switch (settings_.sync) {
        case ArpeggiatorSync::Tempo16th: multiplier = 1.0f; break;
        case ArpeggiatorSync::Tempo8th:  multiplier = 2.0f; break;
        case ArpeggiatorSync::TempoQuarter: multiplier = 4.0f; break;
        case ArpeggiatorSync::TempoHalf: multiplier = 8.0f; break;
        default: break;
    }
    
    return (sampleRate * 60.0f / tempoHz) * multiplier;
}

int Arpeggiator::getNextNote() {
    if (chordNotes_.empty()) return -1;
    
    std::vector<int> sequence;
    
    // Build arpeggio sequence based on mode
    if (settings_.mode == ArpeggiatorMode::Up) {
        for (int oct = 0; oct < settings_.octaveRange; ++oct) {
            for (int note : chordNotes_) {
                sequence.push_back(note + oct * 12);
            }
        }
    } else if (settings_.mode == ArpeggiatorMode::Down) {
        for (int oct = settings_.octaveRange - 1; oct >= 0; --oct) {
            for (auto it = chordNotes_.rbegin(); it != chordNotes_.rend(); ++it) {
                sequence.push_back(*it + oct * 12);
            }
        }
    } else if (settings_.mode == ArpeggiatorMode::UpDown) {
        for (int oct = 0; oct < settings_.octaveRange; ++oct) {
            for (int note : chordNotes_) {
                sequence.push_back(note + oct * 12);
            }
        }
        for (int oct = settings_.octaveRange - 2; oct >= 0; --oct) {
            for (auto it = chordNotes_.rbegin(); it != chordNotes_.rend(); ++it) {
                sequence.push_back(*it + oct * 12);
            }
        }
    } else if (settings_.mode == ArpeggiatorMode::Random) {
        sequence = chordNotes_;
        std::shuffle(sequence.begin(), sequence.end(), g_rng_effects);
    } else if (settings_.mode == ArpeggiatorMode::Strum) {
        // Quick strum over all notes
        sequence = chordNotes_;
    } else { // Hold
        sequence = chordNotes_;
    }
    
    if (sequence.empty()) return -1;
    return sequence[currentStep_ % sequence.size()];
}

int Arpeggiator::process(float sampleRate, float tempoHz) {
    if (chordNotes_.empty() || settings_.mode == ArpeggiatorMode::Hold) {
        return -1;
    }
    
    float stepDuration = getStepDuration(sampleRate, tempoHz);
    notePhase_ += 1.0f / stepDuration;
    
    if (notePhase_ >= 1.0f) {
        notePhase_ -= 1.0f;
        currentStep_++;
        return getNextNote();
    }
    
    return -1;
}

// ============ HUMANIZER ============

Humanizer::Humanizer(const HumanizerSettings& s) : settings_(s) {}

void Humanizer::setSettings(const HumanizerSettings& s) {
    settings_ = s;
}

int Humanizer::humanizeVelocity(int velocity) {
    if (!settings_.enabled || settings_.velocityVariation <= 0.0f) {
        return velocity;
    }
    
    float variation = velocity * settings_.velocityVariation;
    float randomOffset = g_dist_effects(g_rng_effects) * 2.0f - 1.0f;
    int humanized = static_cast<int>(velocity + randomOffset * variation);
    
    return std::clamp(humanized, 0, 127);
}

float Humanizer::humanizeNoteDelay(float sampleRate) {
    if (!settings_.enabled || settings_.timingVariation <= 0.0f) {
        return 0.0f;
    }
    
    float randomOffset = g_dist_effects(g_rng_effects) * 2.0f - 1.0f;
    return randomOffset * settings_.timingVariation * sampleRate * 0.01f;
}

float Humanizer::humanizePitch() {
    if (!settings_.enabled || settings_.tuneDeviation <= 0.0f) {
        return 0.0f;
    }
    
    float randomOffset = g_dist_effects(g_rng_effects) * 2.0f - 1.0f;
    return randomOffset * settings_.tuneDeviation * 100.0f;  // cents
}

// ============ NOTE PROBABILITY ============

NoteProbability::NoteProbability(const NoteProbabilitySettings& s) : settings_(s) {}

void NoteProbability::setSettings(const NoteProbabilitySettings& s) {
    settings_ = s;
}

bool NoteProbability::shouldPlayNote(int stepIndex) {
    currentStep_ = stepIndex;
    
    // Check skip pattern
    if (settings_.skipEveryNth) {
        if (stepIndex % settings_.skipEveryNthStep != 0) {
            return false;
        }
    }
    
    // Check probability
    return g_dist_effects(g_rng_effects) <= settings_.probability;
}

} // namespace scalechord
