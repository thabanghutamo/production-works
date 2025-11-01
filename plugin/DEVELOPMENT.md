# Development Guide for ScaleChord Contributors

Welcome to the ScaleChord project! This guide explains the codebase, how to extend it, and contribution guidelines.

## 📁 Codebase Structure

```
plugin/
├── include/                    # Public headers
│   ├── ScaleMapper.h          # Scale quantization API
│   └── ChordVoicer.h          # Chord generation API
│
├── src/                        # Implementation
│   ├── ScaleMapper.cpp        # Scale quantization impl
│   ├── ChordVoicer.cpp        # Chord generation impl
│   └── demo_main.cpp          # Standalone test app
│
├── tests/                      # Unit tests
│   └── test_scale.cpp         # Test suite (add more here!)
│
├── integration/                # JUCE plugin wrapper (template)
│   ├── JUCEPluginProcessor.h
│   └── JUCEPluginProcessor.cpp
│
└── CMakeLists.txt             # Build configuration
```

## 🏗️ Core Architecture

### ScaleMapper

**Purpose**: Map any incoming MIDI note (0–127) to the nearest note in a user-selected scale.

**Key Classes**:
```cpp
enum class ScaleType {
    Major, NaturalMinor, MajorPentatonic, MinorPentatonic, Dorian, Mixolydian
};

struct MapperSettings {
    int rootNote;      // 0=C, 1=C#, ..., 11=B
    ScaleType scale;
};

class ScaleMapper {
public:
    int mapNote(int incomingMidiNote) const;  // Main API
    std::vector<int> getScaleSemitones() const;
};
```

**Algorithm**:
1. Compute scale degrees (semitones) for the selected scale and root
2. For incoming note, search across ±1 octave for nearest scale degree
3. Return the closest match (0–127)

**Example**:
```
Input: C# (61), Scale: C Major, Root: C (0)
Scale degrees: {0, 2, 4, 5, 7, 9, 11} (C, D, E, F, G, A, B)
Nearest to 61: C (60) or D (62)
Distance: |61-60|=1, |61-62|=1
Returns: 60 (C) — closest
```

### ChordVoicer

**Purpose**: Generate harmonic chord voicings from a scale-mapped note.

**Key Classes**:
```cpp
enum class VoicingType { Triad, Seventh, Open };

struct VoicerSettings {
    VoicingType voicing;
    int octaveOffset;  // -2 to +2
};

class ChordVoicer {
public:
    std::vector<int> makeChordFromNote(int baseMappedMidiNote) const;
};
```

**Algorithm**:
1. Find the index of the base note within the scale (0–6 for 7-note scales)
2. Generate chord by selecting scale degrees: root, 3rd, 5th, (7th)
3. Respect voicing type and octave offset
4. Clamp output to 0–127 MIDI range

**Example** (Triad in C Major):
```
Base note: D (62), Scale: C Major
D is index 1 in {C, D, E, F, G, A, B}
Triad: index 1, 3, 5 → D, F, A → MIDI 62, 65, 69
```

## 🔧 How to Extend

### Add a New Scale Type

1. **Update enum in `ScaleMapper.h`**:
```cpp
enum class ScaleType {
    // ... existing
    Phrygian,  // NEW
};
```

2. **Add scale degrees in `ScaleMapper.cpp`**:
```cpp
static std::vector<int> semitones_for_scale(ScaleType t) {
    switch (t) {
        // ... existing cases
        case ScaleType::Phrygian:
            return {0, 1, 3, 5, 7, 8, 10};  // Phrygian intervals
        // ...
    }
}
```

3. **Add scale name helper**:
```cpp
std::string ScaleMapper::scaleName(ScaleType t) {
    switch (t) {
        // ... existing
        case ScaleType::Phrygian: return "Phrygian";
        // ...
    }
}
```

4. **Test it**:
```cpp
// In tests/test_scale.cpp
MapperSettings ms;
ms.scale = ScaleType::Phrygian;
ScaleMapper mapper(ms);
int mapped = mapper.mapNote(60);  // verify output
```

### Add ADSR Envelope Support

Currently, the core doesn't include envelope generation (ADSR). To add it:

1. **Create `include/Envelope.h`**:
```cpp
struct ADSRSettings {
    float attack, decay, sustain, release;  // in ms
};

class Envelope {
public:
    void setSettings(const ADSRSettings& s);
    float process(float phase);  // phase in 0..1
    void noteOn();
    void noteOff();
};
```

2. **Implement in `src/Envelope.cpp`** (standard ADSR algorithm)

3. **Integrate in `JUCEPluginProcessor.cpp`**:
```cpp
float velocity = mapper_.mapNote(...);
velocity *= envelope_.process(phase);  // apply envelope
```

### Add Velocity Scaling

Current code: all notes use full incoming velocity (0–127).

To add velocity scaling (0–100%):

1. **Add parameter in `PluginProcessor`**:
```cpp
float velocityScale = apvts->getParameter("velocityScale")->getValue();
```

2. **Scale velocity when sending MIDI**:
```cpp
int scaledVelocity = (int)(incomingVelocity * velocityScale);
```

## 🧪 Testing

### Run Existing Tests

```bash
cd build
ctest -V
```

### Add a New Test

Edit `tests/test_scale.cpp`:

```cpp
void test_new_scale() {
    MapperSettings ms;
    ms.rootNote = 5;  // F
    ms.scale = ScaleType::Dorian;
    ScaleMapper mapper(ms);

    // Test that F Dorian contains: F, G, A, Bb, C, D, E
    assert(mapper.mapNote(65) == 65);  // F is in scale
    // ... more assertions
}

int main() {
    // ... existing tests
    test_new_scale();
    std::cout << "All tests passed\n";
    return 0;
}
```

Rebuild and run:
```bash
cd build
cmake --build .
ctest -V
```

### Code Quality

- **Header guards**: Use `#pragma once`
- **Namespacing**: Use `namespace scalechord { ... }`
- **Comments**: Explain *why*, not *what*
- **No raw pointers**: Prefer `std::vector`, `std::unique_ptr`
- **Const correctness**: Mark read-only methods `const`

## 🎵 Music Theory Reference

### Scale Degrees (Semitones from Root)

| Scale | Intervals (semitones) |
|-------|-----------|
| Major | 0, 2, 4, 5, 7, 9, 11 |
| Natural Minor | 0, 2, 3, 5, 7, 8, 10 |
| Dorian | 0, 2, 3, 5, 7, 9, 10 |
| Mixolydian | 0, 2, 4, 5, 7, 9, 10 |
| Major Pentatonic | 0, 2, 4, 7, 9 |
| Minor Pentatonic | 0, 3, 5, 7, 10 |

### Chord Construction

| Chord Type | Scale Degrees | Notes |
|-----------|---|---|
| Triad | 0, 2, 4 | root, 3rd, 5th |
| Seventh | 0, 2, 4, 6 | root, 3rd, 5th, 7th |
| Open | 0, 2, 4 (spread) | spread across octaves |

## 📝 Commit Message Guidelines

Follow conventional commits:

```
feat: Add Phrygian scale support
fix: Correct octave boundary for MIDI 127
docs: Update ARCHITECTURE.md with new algorithm
test: Add edge case for empty scale
```

## 🚀 Submitting Changes

1. **Fork** the repo
2. **Create a branch**: `git checkout -b feat/my-feature`
3. **Make changes** and test locally
4. **Commit**: `git commit -m "feat: description"`
5. **Push**: `git push origin feat/my-feature`
6. **Open a PR** with clear description

## ⚖️ Code Review Checklist

Before submitting a PR, verify:

- [ ] Tests pass: `ctest -V`
- [ ] Code compiles: `cmake --build .`
- [ ] No compiler warnings
- [ ] Comments explain complex logic
- [ ] MIDI values are clamped (0–127)
- [ ] Scale initialization is correct
- [ ] Memory is properly managed (no leaks)

## 🐛 Known Issues & TODOs

Check `ROADMAP.md` for planned features. Known limitations:

- **Note-off handling**: JUCE wrapper needs track which generated notes belong to which input note (not yet implemented)
- **Edge case**: Empty scale (should never happen, but add validation)
- **Performance**: No optimization for real-time audio buffer processing yet

## 💡 Ideas for Contribution

**Easy (Good First Issues)**:
- [ ] Add more test cases
- [ ] Improve code comments
- [ ] Add scale name helper for new scales
- [ ] Update documentation

**Medium**:
- [ ] Add envelope class (ADSR)
- [ ] Implement velocity scaling
- [ ] Add chord name display logic

**Hard**:
- [ ] Complete JUCE integration (GUI, parameters)
- [ ] Performance profiling and optimization
- [ ] Machine learning chord suggestions

## 📞 Questions?

- Check the **ARCHITECTURE.md** for system design
- Review existing code in `src/` and `include/`
- Ask in GitHub Issues or PR comments

---

**Happy coding!** 🎵
