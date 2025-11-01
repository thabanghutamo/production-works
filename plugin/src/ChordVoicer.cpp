#include "ChordVoicer.h"
#include <algorithm>

namespace scalechord {

ChordVoicer::ChordVoicer(const ScaleMapper& mapper) : mapper_(mapper) {}

void ChordVoicer::setSettings(const VoicerSettings& s) { settings_ = s; }
VoicerSettings ChordVoicer::getSettings() const noexcept { return settings_; }

std::vector<int> ChordVoicer::makeChordFromNote(int baseMappedMidiNote) const {
    // We'll use scale degrees to find chord tones. This simplified approach assumes
    // baseMappedMidiNote is within the selected scale. We'll construct chords by
    // moving up scale degrees (3rd and 5th, etc.).

    // get semitones of scale relative to root (0..11)
    auto scaleSemis = mapper_.getScaleSemitones();

    if (scaleSemis.empty()) return {};

    int baseOctave = baseMappedMidiNote / 12;
    int basePitchClass = baseMappedMidiNote % 12;

    // find index of basePitchClass in scaleSemis (best match)
    int baseIndex = 0;
    for (size_t i = 0; i < scaleSemis.size(); ++i) {
        if (scaleSemis[i] == basePitchClass) { baseIndex = static_cast<int>(i); break; }
    }

    std::vector<int> chord;
    auto pushDegree = [&](int degreeStep) {
        // degreeStep: 0=root, 1=next scale degree, etc.
        int degreeIndex = (baseIndex + degreeStep) % scaleSemis.size();
        int octaveShift = (baseIndex + degreeStep) / static_cast<int>(scaleSemis.size());
        int midi = (baseOctave + octaveShift + settings_.octaveOffset) * 12 + scaleSemis[degreeIndex];
        if (midi < 0) midi = 0;
        if (midi > 127) midi = 127;
        chord.push_back(midi);
    };

    if (settings_.voicing == VoicingType::Triad) {
        pushDegree(0);
        pushDegree(2);
        pushDegree(4);
    } else if (settings_.voicing == VoicingType::Seventh) {
        pushDegree(0);
        pushDegree(2);
        pushDegree(4);
        pushDegree(6);
    } else { // Open or other
        pushDegree(0);
        pushDegree(2);
        // spread the 5th an octave up
        int degreeIndex = (baseIndex + 4) % scaleSemis.size();
        int octaveShift = ((baseIndex + 4) / static_cast<int>(scaleSemis.size())) + 1;
        int midi = (baseOctave + octaveShift + settings_.octaveOffset) * 12 + scaleSemis[degreeIndex];
        chord.push_back(midi);
    }

    std::sort(chord.begin(), chord.end());
    return chord;
}

} // namespace scalechord
