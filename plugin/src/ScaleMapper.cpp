#include "ScaleMapper.h"
#include <algorithm>
#include <stdexcept>

namespace scalechord {

static std::vector<int> semitones_for_scale(ScaleType t) {
    switch (t) {
        // Major scale modes (Ionian mode = Major)
        case ScaleType::Ionian:
            return {0,2,4,5,7,9,11};
        
        case ScaleType::Dorian:
            return {0,2,3,5,7,9,10};
        
        case ScaleType::Phrygian:
            return {0,1,3,5,7,8,10};
        
        case ScaleType::Lydian:
            return {0,2,4,6,7,9,11};
        
        case ScaleType::Mixolydian:
            return {0,2,4,5,7,9,10};
        
        case ScaleType::Aeolian:
            return {0,2,3,5,7,8,10};  // Natural minor
        
        case ScaleType::Locrian:
            return {0,1,3,5,6,8,10};
        
        // Minor scale variants
        case ScaleType::HarmonicMinor:
            return {0,2,3,5,7,8,11};  // Minor with raised 7th
        
        case ScaleType::MelodicMinor:
            return {0,2,3,5,7,9,11};  // Minor with raised 6th & 7th
        
        // Pentatonic
        case ScaleType::MajorPentatonic:
            return {0,2,4,7,9};
        
        case ScaleType::MinorPentatonic:
            return {0,3,5,7,10};
        
        // Blues
        case ScaleType::MajorBlues:
            return {0,2,3,4,7,9};  // Major pentatonic + flat 5
        
        case ScaleType::MinorBlues:
            return {0,3,5,6,7,10};  // Minor pentatonic + flat 5
        
        // Other scales
        case ScaleType::WholeTone:
            return {0,2,4,6,8,10};  // All whole steps
        
        case ScaleType::Diminished:
            return {0,2,3,5,6,8,9,11};  // Whole-half diminished
        
        default:
            return {0,2,4,5,7,9,11};  // Default to Ionian/Major
    }
}

ScaleMapper::ScaleMapper(const MapperSettings& s)
    : settings_(s)
{
    rebuildScale();
}

void ScaleMapper::setSettings(const MapperSettings& s) {
    settings_ = s;
    lookupTableValid_ = false;  // Invalidate lookup table
    rebuildScale();
}

MapperSettings ScaleMapper::getSettings() const noexcept { return settings_; }

void ScaleMapper::rebuildScale() {
    auto base = semitones_for_scale(settings_.scale);
    scaleSemitones_.clear();
    for (int st : base) {
        // normalize within 0..11
        int v = (settings_.rootNote + st) % 12;
        if (v < 0) v += 12;
        scaleSemitones_.push_back(v);
    }
    std::sort(scaleSemitones_.begin(), scaleSemitones_.end());
}

std::vector<int> ScaleMapper::getScaleSemitones() const {
    return scaleSemitones_;
}

int ScaleMapper::quantizeToScale(int midiNote) const {
    if (midiNote < 0) midiNote = 0;
    if (midiNote > 127) midiNote = 127;

    int octave = midiNote / 12;
    int noteInOctave = midiNote % 12;

    // find nearest scale degree (in semitones) in this octave, but allow mapping up/down one octave
    int bestNote = octave * 12 + scaleSemitones_[0];
    int bestDist = 1000;
    for (int o = -1; o <= 1; ++o) {
        for (int degree : scaleSemitones_) {
            int candidate = (octave + o) * 12 + degree;
            if (candidate < 0 || candidate > 127) continue;
            int dist = std::abs(candidate - midiNote);
            if (dist < bestDist) {
                bestDist = dist;
                bestNote = candidate;
            }
        }
    }
    return bestNote;
}

int ScaleMapper::mapNote(int incomingMidiNote) const {
    if (scaleSemitones_.empty()) throw std::runtime_error("Scale not initialized");
    return quantizeToScale(incomingMidiNote);
}

void ScaleMapper::buildLookupTable() const {
    // Build lookup table for all 128 MIDI notes
    lookupTable_.resize(128);
    for (int i = 0; i < 128; ++i) {
        lookupTable_[i] = quantizeToScale(i);
    }
    lookupTableValid_ = true;
}

int ScaleMapper::mapNoteFast(int incomingMidiNote) const {
    // Validate input
    if (incomingMidiNote < 0) incomingMidiNote = 0;
    if (incomingMidiNote > 127) incomingMidiNote = 127;
    
    // Rebuild lookup table if settings have changed
    if (!lookupTableValid_) {
        buildLookupTable();
    }
    
    // O(1) lookup
    return lookupTable_[incomingMidiNote];
}

std::string ScaleMapper::scaleName(ScaleType t) {
    switch (t) {
        case ScaleType::Ionian:
            return "Major (Ionian)";
        case ScaleType::Dorian:
            return "Dorian";
        case ScaleType::Phrygian:
            return "Phrygian";
        case ScaleType::Lydian:
            return "Lydian";
        case ScaleType::Mixolydian:
            return "Mixolydian";
        case ScaleType::Aeolian:
            return "Natural Minor (Aeolian)";
        case ScaleType::Locrian:
            return "Locrian";
        case ScaleType::HarmonicMinor:
            return "Harmonic Minor";
        case ScaleType::MelodicMinor:
            return "Melodic Minor";
        case ScaleType::MajorPentatonic:
            return "Major Pentatonic";
        case ScaleType::MinorPentatonic:
            return "Minor Pentatonic";
        case ScaleType::MajorBlues:
            return "Major Blues";
        case ScaleType::MinorBlues:
            return "Minor Blues";
        case ScaleType::WholeTone:
            return "Whole Tone";
        case ScaleType::Diminished:
            return "Diminished";
        default:
            return "Unknown";
    }
}

int ScaleMapper::getScaleDegree(int semitoneInOctave) const {
    semitoneInOctave = ((semitoneInOctave % 12) + 12) % 12;
    for (size_t i = 0; i < scaleSemitones_.size(); ++i) {
        if (scaleSemitones_[i] == semitoneInOctave) {
            return static_cast<int>(i);
        }
    }
    return -1;  // Not in scale
}

std::vector<int> ScaleMapper::getChordIntervalsForDegree(int degree, int chordQuality) const {
    // Generate chord from scale degree
    // chordQuality: 0=triad, 1=7th, 2=9th, etc.
    std::vector<int> intervals;
    int numDegrees = 2 + chordQuality * 2;  // 2, 4, 6, ...
    
    for (int i = 0; i < numDegrees; ++i) {
        int idx = (degree + i * 2) % scaleSemitones_.size();
        if (i == 0) {
            intervals.push_back(scaleSemitones_[idx]);
        } else {
            int prev = intervals.back();
            int curr = scaleSemitones_[idx];
            // Handle wrap-around
            if (curr <= prev) curr += 12;
            intervals.push_back(curr);
        }
    }
    return intervals;
}

ScaleType ScaleMapper::detectScale(const std::vector<int>& pitchClasses, int& outRootNote) {
    // Simple scale detection based on pitch class distribution
    if (pitchClasses.empty()) {
        outRootNote = 0;
        return ScaleType::Ionian;
    }
    
    // Try each pitch class as root and each scale type
    int bestMatch = 0;
    int bestScore = 0;
    int bestRoot = 0;
    ScaleType bestScale = ScaleType::Ionian;
    
    for (int root = 0; root < 12; ++root) {
        for (int scaleIdx = 0; scaleIdx <= static_cast<int>(ScaleType::Diminished); ++scaleIdx) {
            auto scale = semitones_for_scale(static_cast<ScaleType>(scaleIdx));
            int score = 0;
            
            for (int pc : pitchClasses) {
                for (int degree : scale) {
                    if ((root + degree) % 12 == pc) {
                        score++;
                        break;
                    }
                }
            }
            
            if (score > bestScore) {
                bestScore = score;
                bestRoot = root;
                bestScale = static_cast<ScaleType>(scaleIdx);
            }
        }
    }
    
    outRootNote = bestRoot;
    return bestScale;
}

} // namespace scalechord
