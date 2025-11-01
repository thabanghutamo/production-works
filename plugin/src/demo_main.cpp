#include <iostream>
#include "../include/ScaleMapper.h"
#include "../include/ChordVoicer.h"

using namespace scalechord;

int main() {
    MapperSettings ms;
    ms.rootNote = 0; // C
    ms.scale = ScaleType::Ionian;  // Major scale

    ScaleMapper mapper(ms);
    ChordVoicer voicer(mapper);

    VoicerSettings vs; vs.voicing = VoicingType::Seventh; vs.octaveOffset = 0;
    voicer.setSettings(vs);

    int input = 61; // D
    int mapped = mapper.mapNote(input);
    auto chord = voicer.makeChordFromNote(mapped);

    std::cout << "Input: " << input << " mapped-> " << mapped << " chord: ";
    for (int n : chord) std::cout << n << " ";
    std::cout << "\n";
    return 0;
}
