# ScaleChord VST Plugin — Architecture & Design

## Overview

**ScaleChord** is a real-time MIDI-to-MIDI VST plugin (Windows/Mac/Linux) that allows anyone to play melodic music without music theory knowledge. It quantizes incoming MIDI notes to a user-selected scale and generates harmonic chord voicings in real time.

## User Experience

1. **Pre-Play Setup**
   - Select musical key (C, C#, D, ... B)
   - Choose scale type (Major, Minor, Pentatonic, Dorian, Mixolydian, etc.)
   - Pick voicing style (Triad, Seventh, Open)
   - Optional: select genre/mood preset that adjusts voicing and octave voicing
   - Set ADSR parameters (Attack, Decay, Sustain, Release)
   - Set target MIDI output channel (to route into synth/instrument plugin)

2. **Play**
   - User presses any key on MIDI keyboard or computer keyboard
   - Plugin automatically maps that note to the nearest scale degree
   - Plugin generates a full chord (3-4 notes) from the mapped note
   - Chord notes are output to the configured MIDI channel
   - Downstream instrument plugin plays the chord

## Architecture

### Core Components

```
┌─────────────────────────────────────────────────────────────┐
│  VST Host (DAW: FL Studio, Ableton, Logic, Studio One, etc) │
└──────────────────────┬──────────────────────────────────────┘
                       │ MIDI Input
                       │
        ┌──────────────▼────────────────────┐
        │  ScaleChord Plugin (VST3/AU)       │
        ├────────────────────────────────────┤
        │ ┌─ JUCE AudioProcessor             │
        │ │  ├─ MIDI Handler                 │
        │ │  ├─ Parameter State (key, scale) │
        │ │  └─ processBlock()                │
        │ │                                  │
        │ ├─ ScaleMapper (core)              │
        │ │  └─ mapNote(incoming) → scale    │
        │ │                                  │
        │ └─ ChordVoicer (core)              │
        │    └─ makeChord(note) → [MIDI]    │
        └─────────┬──────────────────────────┘
                  │ MIDI Output
                  │
        ┌─────────▼──────────────┐
        │  Synth Plugin           │
        │  (Xfer Serum, u-he...)  │
        └────────────────────────┘
```

### Data Flow (Real Time)

1. **MIDI In:** User plays C# (note 61)
2. **Scale Mapper:** If scale is C Major, nearest scale note is C (60) or D (62) → selects D (62)
3. **Chord Voicer:** D in C Major scale = 2nd degree → generates D-F#-A (triad)
4. **MIDI Out:** Output D (62), F# (66), A (69) to target channel
5. **Result:** Downstream synth plays a D chord

### Core Library (C++17)

#### `ScaleMapper` Class
```cpp
class ScaleMapper {
    int mapNote(int incomingMidiNote) const;
    std::vector<int> getScaleSemitones() const;
    void setSettings(const MapperSettings& s);
};
```
- Stores the selected key/scale
- Quantizes any MIDI note (0–127) to the scale
- Uses nearest-neighbor search across adjacent octaves

#### `ChordVoicer` Class
```cpp
class ChordVoicer {
    std::vector<int> makeChordFromNote(int baseMappedMidiNote) const;
    void setSettings(const VoicerSettings& s);
};
```
- Uses scale degrees to construct chords (root + 2nd/3rd + 4th/5th, etc.)
- Supports Triad, Seventh, and Open voicings
- Respects octave offset for spread voicings

### Parameter/UI Layer (JUCE)

When integrated into JUCE:
- **AudioProcessorValueTreeState** exposes parameters:
  - Root Note (0–11 select)
  - Scale Type (enum)
  - Voicing (enum)
  - Octave Offset (–2 to +2)
  - Target MIDI Channel (1–16)
  - ADSR: Attack, Decay, Sustain, Release
  - Velocity Scaling (0%–100%)

- **GenericAudioProcessorEditor** or custom UI displays controls
- DAW host automation works on all parameters

## MIDI Note Tracking (Note-Off Handling)

For a production plugin, track which generated chord notes correspond to which input notes:

```cpp
struct ActiveNote {
    int inputNote;
    std::vector<int> generatedChordNotes;
    int samplePosition;
};

std::map<int, ActiveNote> activeNotes_; // per channel
```

When input Note Off arrives, send Note Off for all corresponding generated notes.

## Genres/Presets (Optional Enhancement)

Pre-configured voicing strategies:
- **Blues**: Minor pentatonic, octave-doubled root
- **Jazz**: Seventh voicings, richer spreads
- **Pop**: Simple triads, tight octave range
- **EDM**: Pentatonic, wide spreads for arpeggios
- **Classical**: Scale-aware voicings, traditional voice-leading

## Build & Integration Steps

### 1. Standalone Core (Done ✓)
- `ScaleMapper.cpp/h`
- `ChordVoicer.cpp/h`
- CMake build, unit tests pass

### 2. JUCE Integration (Template Provided)
```bash
# Assume JUCE installed at ~/JUCE
# Create a new JUCE plugin project template
# Copy ScaleMapper and ChordVoicer into the project
# Integrate JUCEPluginProcessor.h/cpp examples
# Wire parameters into AudioProcessorValueTreeState
# Build as VST3/AU
```

### 3. GUI (Framework-Specific)
- JUCE Slider for key selection
- ComboBox for scale type
- ComboBox for voicing
- Sliders for ADSR
- Meter for velocity feedback

## Key Features (User-Facing)

✓ **Zero Music Theory Required**: "Pick a key, scale, and chord type; it does the rest."
✓ **Real-Time Responsiveness**: Sub-millisecond latency on note mapping.
✓ **Flexible Output**: Send to any VST synth via MIDI channel routing.
✓ **Preset Support**: Save/recall favorite keys, scales, voicings, ADSR.
✓ **Live & Production Ready**: CPU-efficient, low latency, DAW-agnostic (VST3/AU).

## Deployment

- **Windows**: VST3 binary
- **Mac**: VST3 + AU (Audio Units)
- **Linux**: VST3
- **Installer**: Optional WiX (Windows) or DMG (Mac) packaging

## Future Enhancements

1. **Arpeggiator**: Auto-strum generated chords
2. **Scale Randomizer**: Probabilistic note mapping for variation
3. **Chord Memory**: Remember recent chords, cycle through them
4. **MIDI Learn**: Auto-detect key from incoming MIDI chord
5. **Sidechain Integration**: Trigger chords from specific notes
6. **GPU-Accelerated Reverb/Delay**: On-plugin effects

## Testing

- Unit tests for `ScaleMapper` (all scale types, edge MIDI notes)
- Unit tests for `ChordVoicer` (all voicing types)
- Integration tests in JUCE (MIDI round-trip, parameter automation)
- Perceptual tests (blind listening with users)
