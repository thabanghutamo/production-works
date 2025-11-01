# 📚 ScaleChord Documentation Index

**Start here**: Pick what matches your role and needs.

---

## 👨‍🎵 For Musicians & Producers (Non-Technical)

| Document | Time | What You'll Learn |
|----------|------|------------------|
| **[PROJECT_SUMMARY.md](PROJECT_SUMMARY.md)** | 3 min | What this project is & why you should care |
| **[QUICKSTART.md](QUICKSTART.md)** | 5 min | How to build & test the core |
| **[MIDI_ROUTING.md](MIDI_ROUTING.md)** | 10 min | How to use it in FL Studio, Ableton, Logic, etc. |

→ **Then wait for** the VST3 binary release (Q1 2026)

---

## 👨‍💻 For Developers (Building the VST3 Plugin)

| Document | Time | What You'll Learn |
|----------|------|------------------|
| **[README.md](README.md)** | 2 min | Project overview & tech stack |
| **[ARCHITECTURE.md](ARCHITECTURE.md)** | 15 min | Full system design, data flow, components |
| **[BUILDING_VST3.md](BUILDING_VST3.md)** | 30 min | Step-by-step: JUCE + CMake → VST3 plugin |
| **[DEVELOPMENT.md](DEVELOPMENT.md)** | 20 min | How to extend the code & contribute |

→ **Then integrate** using templates in `integration/` folder

---

## 🛣️ For Contributors (Extending Features)

| Document | Time | What You'll Learn |
|----------|------|------------------|
| **[DEVELOPMENT.md](DEVELOPMENT.md)** | 20 min | Code structure, how to add scales/voicings |
| **[ROADMAP.md](ROADMAP.md)** | 10 min | Planned features, priorities, timeline |
| **[ARCHITECTURE.md](ARCHITECTURE.md)** | 15 min | System design (needed for context) |

→ **Then choose** a task from ROADMAP and submit a PR

---

## 📁 File Reference

### Documentation (Read First)

```
INDEX.md                    ← You are here
PROJECT_SUMMARY.md          ← Quick overview of entire project
README.md                   ← Plugin overview & features
QUICKSTART.md               ← Get it running in 5 minutes
ARCHITECTURE.md             ← Full system design & theory
BUILDING_VST3.md            ← How to build the VST3 plugin
MIDI_ROUTING.md             ← DAW integration guides
ROADMAP.md                  ← Feature roadmap (v0.1–v0.5+)
DEVELOPMENT.md              ← Contributor guide & codebase walkthrough
```

### Source Code

```
include/
  ├── ScaleMapper.h          ← Scale quantization API
  └── ChordVoicer.h          ← Chord generation API

src/
  ├── ScaleMapper.cpp        ← Quantization implementation
  ├── ChordVoicer.cpp        ← Chord generation implementation
  └── demo_main.cpp          ← Standalone demo executable

tests/
  └── test_scale.cpp         ← Unit tests (add more here!)

integration/
  ├── JUCEPluginProcessor.h  ← JUCE template (header)
  └── JUCEPluginProcessor.cpp ← JUCE template (implementation)

CMakeLists.txt              ← Build configuration
```

---

## 🎯 Reading Paths (By Role)

### Path A: "I'm a musician. When is the plugin ready?"

1. Read: [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md) (3 min)
2. Read: [MIDI_ROUTING.md](MIDI_ROUTING.md) (optional, DAW-specific)
3. **Result**: You'll know when to expect the plugin and how to use it

### Path B: "I'm a developer. How do I build the VST3?"

1. Read: [QUICKSTART.md](QUICKSTART.md) (5 min) — verify core works locally
2. Read: [BUILDING_VST3.md](BUILDING_VST3.md) (30 min) — step-by-step guide
3. Read: [ARCHITECTURE.md](ARCHITECTURE.md) (15 min) — understand the design
4. **Result**: You can wrap the core into a working VST3 plugin

### Path C: "I want to contribute. Where do I start?"

1. Read: [README.md](README.md) (2 min) — project overview
2. Read: [DEVELOPMENT.md](DEVELOPMENT.md) (20 min) — codebase walkthrough
3. Read: [ROADMAP.md](ROADMAP.md) (10 min) — see what needs work
4. Review: `src/ScaleMapper.cpp` and `src/ChordVoicer.cpp` (understand code)
5. **Result**: You can extend the plugin with new scales, voicings, or features

### Path D: "I'm learning about the design. Give me context."

1. Read: [ARCHITECTURE.md](ARCHITECTURE.md) (15 min) — full design
2. Skim: [DEVELOPMENT.md](DEVELOPMENT.md) (focus on "Core Architecture" section)
3. Review: `include/ScaleMapper.h` and `include/ChordVoicer.h` (see APIs)
4. **Result**: Deep understanding of how the plugin works

---

## ⚡ TL;DR (30 seconds)

**What is ScaleChord?**  
A VST3 plugin that maps any MIDI note to a user's selected scale and generates chords automatically.

**Current Status?**  
Core C++ library is done, tested, documented. JUCE wrapper template is ready. VST3 binary coming Q1 2026.

**How do I use it?**  
Build locally: `cd plugin && mkdir build && cd build && cmake .. && make && ctest`

**Want to contribute?**  
Read DEVELOPMENT.md, pick a task from ROADMAP.md, and submit a PR.

---

## 🔗 Quick Links

- **Build**: `mkdir build && cd build && cmake .. && make`
- **Test**: `ctest -V`
- **Demo**: `./scalechord_demo`
- **Next Step**: Read the document matching your role above ↑

---

**Last Updated**: October 31, 2025  
**Questions?** Check the relevant document above.
