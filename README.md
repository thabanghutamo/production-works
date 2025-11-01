# Production Works — Music & Audio Projects

This repository hosts production-grade tools for audio and music creation, starting with **ScaleChord**: a real-time VST plugin for non-musicians to play music live and in production.

## 🎵 ScaleChord VST Plugin

**Problem**: Musicians without music theory struggle to play live or produce convincingly.

**Solution**: A VST3 plugin that maps any keyboard input to a user-selected scale and generates harmonic chords in real time.

### Quick Facts

- **Language**: C++17
- **Plugin Type**: VST3 (also AU/AAX via JUCE)
- **Platforms**: Windows, Mac, Linux
- **Status**: Core library complete, tested, ready for VST wrapper
- **Zero Theory Required**: Select key → select scale → play → profit

### Get Started

```bash
cd plugin/
mkdir -p build && cd build
cmake ..
cmake --build . -j
ctest -V               # Run tests
./scalechord_demo      # See it in action
```

**See Also**:
- `plugin/QUICKSTART.md` — Quick start guide
- `plugin/ARCHITECTURE.md` — Full design spec
- `plugin/README.md` — Build & integration

### What's Included

- ✅ **ScaleMapper** — Quantize incoming MIDI to any scale
- ✅ **ChordVoicer** — Generate triads, sevenths, and open voicings
- ✅ **Unit Tests** — Comprehensive test coverage
- ✅ **JUCE Integration Template** — Ready-to-adapt AudioProcessor
- ✅ **Documentation** — Architecture, usage, deployment

### Future Enhancements

- Arpeggiator mode
- Preset library (Blues, Jazz, Pop, EDM genres)
- MIDI chord detection (auto-key discovery)
- Effects (reverb, delay)
- AAX support for Pro Tools
- Graphical visualization of scales/chords

## 📁 Repository Structure

```
production-works/
├── README.md                       # This file
├── plugin/                         # ScaleChord VST plugin
│   ├── QUICKSTART.md              # Quick start
│   ├── ARCHITECTURE.md            # Design spec
│   ├── CMakeLists.txt             # Build config
│   ├── include/
│   │   ├── ScaleMapper.h
│   │   └── ChordVoicer.h
│   ├── src/
│   │   ├── ScaleMapper.cpp
│   │   ├── ChordVoicer.cpp
│   │   └── demo_main.cpp
│   ├── tests/
│   │   └── test_scale.cpp
│   └── integration/
│       └── JUCEPluginProcessor.* (JUCE template)
└── (future projects...)
```

## 🛠️ Requirements

- **CMake** 3.16+
- **C++17 Compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- (Optional) **JUCE** for VST plugin building

## 📖 Learn More

Each project in this repo includes:
- Quick start guide (5 min to build & test)
- Full architecture documentation
- Integration templates for real-world deployment

## 🎯 Vision

**production-works** is a collection of professional-grade, open-source tools for musicians and producers. Each tool solves a real problem, ships with solid tests, and integrates easily into existing workflows.

---

**First Release**: ScaleChord VST Plugin | **Status**: Ready for JUCE integration & final QA