// Chord voicer - generate chord notes (MIDI) from a root and scale
#pragma once

#include <vector>
#include "ScaleMapper.h"

namespace scalechord {

enum class VoicingType {
    Triad,      // root + 3rd + 5th
    Seventh,    // root + 3rd + 5th + 7th
    Open,       // spread voicing
};

struct VoicerSettings {
    VoicingType voicing = VoicingType::Triad;
    int octaveOffset = 0; // how to shift generated chord
};

class ChordVoicer {
public:
    explicit ChordVoicer(const ScaleMapper& mapper);

    void setSettings(const VoicerSettings& s);
    VoicerSettings getSettings() const noexcept;

    // Given a base MIDI note (already mapped to the scale), return a vector of MIDI notes
    // representing the chord voicing (sorted low->high).
    std::vector<int> makeChordFromNote(int baseMappedMidiNote) const;

private:
    const ScaleMapper& mapper_;
    VoicerSettings settings_;
};

} // namespace scalechord
