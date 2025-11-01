# ScaleChord — Features & Roadmap

## ✅ Current Release (v0.1)

### Core Features
- [x] **Scale Mapper**: Quantize MIDI notes to any scale
  - Supported: Major, Natural Minor, Pentatonic (Major/Minor), Dorian, Mixolydian
  - Nearest-neighbor algorithm with octave awareness
  - Sub-1ms latency

- [x] **Chord Voicer**: Generate harmonies from mapped notes
  - Triad voicing (root + 3rd + 5th)
  - Seventh voicing (root + 3rd + 5th + 7th)
  - Open voicing (spread voicing with extended intervals)
  - Octave offset control

- [x] **JUCE Integration Template**
  - AudioProcessor wrapper (ready to adapt)
  - Example processBlock with MIDI handling
  - Parameter state management (outline)

- [x] **Testing**
  - Unit tests for ScaleMapper
  - Unit tests for ChordVoicer
  - Demo executable
  - CMake build system

- [x] **Documentation**
  - QUICKSTART.md (5-min guide)
  - ARCHITECTURE.md (full design)
  - MIDI_ROUTING.md (DAW integration)
  - BUILDING_VST3.md (build instructions)
  - Code comments and examples

### Limitations
- No built-in GUI (template provided, user must implement in JUCE)
- No preset/state saving (JUCE handles this)
- Note-off handling is simplified (requires book-keeping in real plugin)
- No advanced effects (reverb, delay, arpeggiator)

---

## 🎯 v0.2 (Planned: Q1 2026)

### VST3 Plugin Wrapper (Complete)
- [ ] Full JUCE integration (AudioProcessor + Editor)
- [ ] Parameter UI:
  - [ ] Root Note dropdown
  - [ ] Scale type selector
  - [ ] Voicing selector
  - [ ] ADSR sliders
  - [ ] Velocity scale control
  - [ ] Output MIDI channel selector
  - [ ] Octave offset slider
- [ ] Preset system (save/load/export)
- [ ] Build for Windows, Mac, Linux
- [ ] Code signing & packaging

### UI/UX Polish
- [ ] Visual scale degree indicator
- [ ] Chord name display (e.g., "D Major", "F# Seventh")
- [ ] Real-time MIDI monitor (show incoming/outgoing notes)
- [ ] Meter for velocity feedback
- [ ] Dark and light themes

### Platform Support
- [ ] VST3 (Windows, Mac, Linux)
- [ ] AU (Mac)
- [ ] AAX (Pro Tools) — requires Avid SDK

---

## 🚀 v0.3 (Planned: Q2 2026)

### Extended Scales
- [ ] More scale types:
  - [ ] Blues scale
  - [ ] Jazz scales (Lydian, Phrygian)
  - [ ] Harmonic/Melodic minor
  - [ ] Whole-tone scale
  - [ ] Custom user scales
- [ ] Scale analysis (auto-detect key from incoming chord)

### Genre Presets
- [ ] Pre-configured voicings:
  - [ ] **Blues**: Minor pentatonic, power chords
  - [ ] **Jazz**: Extended 7th/9th chords
  - [ ] **Pop**: Simple triads
  - [ ] **EDM**: Wide spreads, synth-friendly
  - [ ] **Classical**: Traditional voice-leading
- [ ] One-click mood selection (Bright, Dark, Melancholic, Energetic)

### Arpeggiator
- [ ] Auto-strum generated chords
- [ ] Rhythm presets (straight, swing, triplet)
- [ ] Direction control (up, down, up-down, random)
- [ ] Tempo sync with DAW

---

## ✨ v0.4 (Planned: Q3 2026)

### Advanced MIDI Features
- [ ] Chord memory (cycle through recent chords)
- [ ] MIDI learn (auto-detect key from incoming chord progression)
- [ ] Chord substitution (jazz/advanced theory hints)
- [ ] Note probability (add variation/randomness)
- [ ] Sidechain triggering (use drum channel to trigger chords)

### Effects & Processing
- [ ] On-board reverb (small room, hall, plate presets)
- [ ] Delay (sync'd to DAW tempo)
- [ ] Velocity compression
- [ ] Humanization (micro-timing random jitter)

### Performance Improvements
- [ ] GPU-accelerated parameter interpolation (if applicable)
- [ ] Multi-threaded chord generation (for very large chords)
- [ ] SIMD optimizations for real-time mapping

---

## 🎓 v0.5 (Planned: Q4 2026)

### Learning & Education
- [ ] In-plugin theory guide (explain what each scale/voicing does)
- [ ] Interactive tutorial (guided "first 5 minutes")
- [ ] Video documentation (YouTube channel)
- [ ] Music theory glossary

### Community & Sharing
- [ ] Preset library (download community presets)
- [ ] User forum / Discord
- [ ] Preset import/export (share with other users)
- [ ] Analytics (opt-in: help us understand usage patterns)

---

## 🔮 Long-Term (Post v0.5)

### Advanced Features
- [ ] Polyphonic key detection (track multiple voices)
- [ ] Machine learning chord suggestions (based on playing style)
- [ ] Integration with other plugins (send/receive MIDI from other VSTs)
- [ ] Sidechain MIDI from DAW (external sync)

### Platform Expansion
- [ ] Standalone app (no DAW required, for live jamming)
- [ ] iOS/Android app (mobile version)
- [ ] Web version (browser-based, no download)
- [ ] Hardware controller support (TouchOSC, lemur, Controllerism)

### Studio Suite
- [ ] Scale Chord Analyzer (detect scales in audio)
- [ ] Harmony Helper (suggest chord progressions)
- [ ] Melody Coach (check melody against scale)
- [ ] Ensemble Kit (multi-scale layering for orchestration)

---

## 📊 Priority Matrix (Next 12 Months)

```
High Impact, Low Effort:
  ✓ VST3 wrapper + UI (v0.2)
  ✓ Extended scales (v0.3)
  ✓ Genre presets (v0.3)
  
High Impact, High Effort:
  ○ Arpeggiator (v0.3)
  ○ Effects (v0.4)
  ○ Machine learning (post-v0.5)

Low Impact, Low Effort:
  ○ Documentation (ongoing)
  ○ Code cleanup (ongoing)

Low Impact, High Effort:
  ○ iOS app (post-v0.5)
  ○ Hardware support (post-v0.5)
```

---

## 🤝 Contributing

Community contributions are welcome! Priority areas:
1. Test coverage (edge cases, cross-platform)
2. Documentation (tutorials, videos, examples)
3. Scale/chord additions (new music theory modules)
4. GUI improvements (Figma mockups, JUCE layouts)
5. Platform-specific builds (Mac/Windows packaging)

See `CONTRIBUTING.md` (TBD) for guidelines.

---

## 📞 Feedback & Feature Requests

- **GitHub Issues**: Report bugs and request features
- **Discord**: Join our community server (link TBD)
- **Email**: support@scalechord.io (TBD)
- **Survey**: Fill out our user feedback form (TBD)

---

**Last Updated**: October 31, 2025 | **Maintainer**: thabanghutamo
