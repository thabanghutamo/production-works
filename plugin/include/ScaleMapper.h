// Lightweight scale mapper - header-only interface
#pragma once

#include <vector>
#include <string>

namespace scalechord {

enum class ScaleType {
    // Major scale modes (7 church modes)
    Ionian = 0,          // Major
    Dorian = 1,
    Phrygian = 2,
    Lydian = 3,
    Mixolydian = 4,
    Aeolian = 5,         // Natural Minor
    Locrian = 6,
    
    // Minor scales
    HarmonicMinor = 7,   // Minor with raised 7th
    MelodicMinor = 8,    // Minor with raised 6th & 7th (ascending)
    
    // Pentatonic scales
    MajorPentatonic = 9,
    MinorPentatonic = 10,
    
    // Blues scales
    MajorBlues = 11,     // Major pentatonic + flat 5
    MinorBlues = 12,     // Minor pentatonic + flat 5
    
    // Other popular scales
    WholeTone = 13,      // All intervals = 2 semitones
    Diminished = 14,     // Whole-half alternating
};

struct MapperSettings {
    int rootNote = 0; // 0 = C, 1 = C#, ... 11 = B
    ScaleType scale = ScaleType::Ionian;
};

class ScaleMapper {
public:
    ScaleMapper() = default;
    explicit ScaleMapper(const MapperSettings& s);

    void setSettings(const MapperSettings& s);
    MapperSettings getSettings() const noexcept;

    // Map an incoming MIDI note (0-127) to the nearest note in the selected scale.
    // Returns a MIDI note (0-127).
    int mapNote(int incomingMidiNote) const;

    // Optimized lookup table version - call buildLookupTable() first
    // Much faster than mapNote() for repeated calls with same settings
    int mapNoteFast(int incomingMidiNote) const;
    
    // Build the internal lookup table for mapNoteFast()
    void buildLookupTable() const;

    // Return the scale degrees in semitones relative to root (sorted ascending, within one octave)
    std::vector<int> getScaleSemitones() const;
    
    // Get scale degree index (0-6 for heptatonic, etc.) for a given semitone
    int getScaleDegree(int semitoneInOctave) const;
    
    // Get the chord quality and intervals from a given scale degree
    // Returns intervals as semitone offsets from root
    std::vector<int> getChordIntervalsForDegree(int degree, int chordQuality = 0) const;
    
    // Detect scale from a set of pitch classes
    static ScaleType detectScale(const std::vector<int>& pitchClasses, int& outRootNote);

    static std::string scaleName(ScaleType t);

private:
    MapperSettings settings_;
    std::vector<int> scaleSemitones_; // computed from settings
    mutable std::vector<int> lookupTable_;  // Cache for mapNoteFast() - all 128 MIDI notes
    mutable bool lookupTableValid_ = false;  // Flag to detect when settings change

    void rebuildScale();
    int quantizeToScale(int midiNote) const;
};

} // namespace scalechord
