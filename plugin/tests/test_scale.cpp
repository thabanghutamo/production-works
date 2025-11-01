#include <cassert>
#include <iostream>
#include "../include/ScaleMapper.h"
#include "../include/ChordVoicer.h"

using namespace scalechord;

int main() {
    MapperSettings ms;
    ms.rootNote = 0; // C
    ms.scale = ScaleType::Ionian;  // Major scale
    ScaleMapper mapper(ms);

    // Test mapping: input C (60) should map to 60
    assert(mapper.mapNote(60) == 60);

    // Input C# (61) should map to nearest scale note (C or D). In major scale, D=62 is closer to 61
    int mapped61 = mapper.mapNote(61);
    if (!(mapped61 == 60 || mapped61 == 62)) {
        std::cerr << "Unexpected mapping for 61: " << mapped61 << "\n";
        return 2;
    }

    // Chord generation smoke test
    ChordVoicer voicer(mapper);
    VoicerSettings vs; vs.voicing = VoicingType::Triad; vs.octaveOffset = 0;
    voicer.setSettings(vs);
    auto chord = voicer.makeChordFromNote(mapper.mapNote(64));
    if (chord.size() < 3) {
        std::cerr << "Chord size too small" << std::endl;
        return 3;
    }

    std::cout << "All tests passed\n";
    return 0;
}
