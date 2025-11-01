# ScaleChord v0.2 → v0.3+ Implementation Roadmap

**Goal**: Complete the remaining 3 advanced features to achieve production-grade VST3 plugin.

---

## Overview: Remaining Tasks

| Task | Status | Complexity | Est. Time | Impact |
|------|--------|-----------|-----------|--------|
| Task 3: Chord Intelligence & Voice Leading | ⏳ Planned | 🔴 High | 4–6 hrs | 🌟 Essential |
| Task 6: Performance Optimization & Metrics | ⏳ Planned | 🟡 Medium | 3–4 hrs | 🌟 Essential |
| Task 7: Preset System & State Management | ⏳ Planned | 🟡 Medium | 4–5 hrs | 🌟 Critical |

---

## 📌 Task 3: Chord Intelligence & Voice Leading

### **Objective**
Enable the plugin to analyze chord progressions, suggest voice leading, and provide jazz reharmonization.

### **Deliverables**

#### A. **ChordAnalyzer** Module
Detect chord quality from generated notes.

**Header: `include/ChordAnalyzer.h`** (~100 lines)
```cpp
#pragma once
#include <vector>

enum class ChordQuality {
    Major,
    Minor,
    Dominant7,
    Major7,
    Minor7,
    HalfDim7,
    Diminished,
    Augmented,
    Sus2,
    Sus4,
    Maj9,
    Min9,
    Dom9,
    Unknown
};

enum class ChordFunction {
    Tonic,      // I
    Subdominant, // IV
    Dominant,    // V
    Relative,    // vi
    Extended
};

class ChordAnalyzer {
public:
    struct ChordInfo {
        int root;
        ChordQuality quality;
        ChordFunction function;
        std::vector<int> intervals;  // semitones from root
        float confidence;             // 0.0-1.0
    };

    ChordAnalyzer();
    
    // Analyze chord from pitch collection
    ChordInfo analyzeChord(const std::vector<int>& notes, int inC) const;
    
    // Get all possible chord interpretations (sorted by confidence)
    std::vector<ChordInfo> analyzeChordAmbiguous(
        const std::vector<int>& notes, int inC) const;
    
    // Detect function (I, IV, V, vi) based on root
    ChordFunction detectFunction(int root, int keyRoot, int scale) const;
    
    // Get Roman numeral representation
    const char* getRomanNumeral(int degree, bool major, bool isMinor) const;
    
    static const char* qualityToString(ChordQuality q);
    static const char* functionToString(ChordFunction f);
};
```

**Implementation: `src/ChordAnalyzer.cpp`** (~200 lines)
```cpp
#include "ChordAnalyzer.h"
#include <algorithm>
#include <cmath>

ChordAnalyzer::ChordInfo ChordAnalyzer::analyzeChord(
    const std::vector<int>& notes, int baseKey) const
{
    ChordInfo result;
    result.confidence = 0.0f;
    result.quality = ChordQuality::Unknown;
    
    if (notes.empty()) return result;
    
    // Normalize all notes to C (0-11 range)
    std::vector<int> normalized;
    for (int note : notes)
        normalized.push_back((note - baseKey + 12) % 12);
    
    std::sort(normalized.begin(), normalized.end());
    normalized.erase(std::unique(normalized.begin(), normalized.end()),
                     normalized.end());
    
    result.root = notes[0];
    result.intervals = normalized;
    
    // Identify chord quality by interval pattern
    if (normalized == std::vector<int>{0, 4, 7}) {
        result.quality = ChordQuality::Major;
        result.confidence = 1.0f;
    }
    else if (normalized == std::vector<int>{0, 3, 7}) {
        result.quality = ChordQuality::Minor;
        result.confidence = 1.0f;
    }
    else if (normalized == std::vector<int>{0, 4, 7, 10}) {
        result.quality = ChordQuality::Dominant7;
        result.confidence = 0.95f;
    }
    else if (normalized == std::vector<int>{0, 4, 7, 11}) {
        result.quality = ChordQuality::Major7;
        result.confidence = 0.95f;
    }
    // ... more chord patterns ...
    
    return result;
}

const char* ChordAnalyzer::qualityToString(ChordQuality q) {
    switch (q) {
        case ChordQuality::Major: return "Major";
        case ChordQuality::Minor: return "Minor";
        case ChordQuality::Dominant7: return "Dom7";
        case ChordQuality::Major7: return "Maj7";
        // ... etc ...
        default: return "?";
    }
}
```

#### B. **VoiceLeading** Module
Optimize transitions between chords.

**Header: `include/VoiceLeading.h`** (~80 lines)
```cpp
#pragma once
#include <vector>
#include "ChordAnalyzer.h"

class VoiceLeading {
public:
    struct VoiceLeadingResult {
        std::vector<int> previousChord;
        std::vector<int> nextChord;
        float smoothnessScore;  // 0-100, higher = smoother
        int totalDistance;      // total semitones moved
        bool commonTones;       // if true, at least one tone in common
    };

    VoiceLeading();
    
    // Find optimal voicing of second chord given first chord
    std::vector<int> optimizeVoicing(
        const std::vector<int>& currentChord,
        const std::vector<int>& targetChord,
        int octaveRange = 2) const;
    
    // Score voice leading smoothness (0-100)
    float scoreVoiceLeading(
        const std::vector<int>& from,
        const std::vector<int>& to) const;
    
    // Suggest smoother voicing for chord transition
    VoiceLeadingResult suggestSmoothVoicing(
        const std::vector<int>& current,
        const std::vector<int>& next) const;
};
```

#### C. **JazzReharmonizer** Module
Suggest jazz chord substitutions.

**Header: `include/JazzReharmonizer.h`** (~70 lines)
```cpp
#pragma once
#include <vector>
#include "ChordAnalyzer.h"

class JazzReharmonizer {
public:
    struct Substitution {
        int degree;          // II, IV, V, VII
        ChordQuality originalQuality;
        ChordQuality substituteQuality;
        const char* name;    // e.g., "ii-V-I"
        float musicality;    // 0-100
    };

    JazzReharmonizer();
    
    // Get substitutions for a chord degree
    std::vector<Substitution> getSubstitutions(
        int scaleDegree, 
        bool majorKey) const;
    
    // Common jazz moves
    std::vector<int> tritoneSubstitution(const std::vector<int>& dominantChord) const;
    std::vector<int> secondaryDominant(int degree, bool majorKey) const;
    std::vector<int> parallelVoicing(const std::vector<int>& referenceChord) const;
};
```

### **Implementation Steps**

1. **Create ChordAnalyzer.h/cpp** (200 lines)
   - Implement interval pattern matching for 15 chord types
   - Add chord function detection
   - Test with existing demo

2. **Create VoiceLeading.h/cpp** (150 lines)
   - Implement smoothness scoring (minimize voice movement)
   - Test with chord progressions

3. **Create JazzReharmonizer.h/cpp** (100 lines)
   - Tritone substitution
   - Secondary dominants
   - Common jazz movement patterns

4. **Update CMakeLists.txt** (add 3 new source files)

5. **Create test cases** in `test_scale.cpp`:
   ```cpp
   TEST_CASE("ChordAnalyzer detects major chord") {
       ChordAnalyzer analyzer;
       std::vector<int> chord{60, 64, 67};  // C-E-G
       auto result = analyzer.analyzeChord(chord, 60);
       REQUIRE(result.quality == ChordQuality::Major);
   }
   ```

6. **Create demo** `voice_leading_demo.cpp`:
   - Play C → F → G progression
   - Show recommended smooth voicings
   - Suggest jazz substitutions

### **Estimated Time**: 4–6 hours

---

## 📊 Task 6: Performance Optimization & Metrics

### **Objective**
Profile core algorithms, add performance monitoring, and optimize for real-time constraints.

### **Deliverables**

#### A. **PerformanceMetrics** Module
Track latency and CPU usage.

**Header: `include/PerformanceMetrics.h`** (~70 lines)
```cpp
#pragma once
#include <chrono>
#include <vector>
#include <deque>

class PerformanceMetrics {
public:
    struct Measurement {
        std::chrono::microseconds duration;
        size_t memoryUsed;
        int notesProcessed;
    };

    PerformanceMetrics(int historySize = 1000);
    
    // Start/stop timing
    void startMeasurement();
    void endMeasurement(int notesProcessed = 1);
    
    // Statistics
    float getAverageLatencyMs() const;
    float getMaxLatencyMs() const;
    float getMinLatencyMs() const;
    float getP95LatencyMs() const;
    
    // Throughput
    float getNotesPerSecond() const;
    float getCPUUsagePercent(float targetBlockTime) const;
    
    // Reset
    void reset();

private:
    std::deque<Measurement> history_;
    std::chrono::steady_clock::time_point startTime_;
};
```

#### B. **OptimizedScaleMapper**
Fast MIDI quantization with SSE optimization (if applicable).

**Performance Goals**:
- ✓ Quantize 16 notes in < 0.01 ms
- ✓ Binary search instead of linear scan
- ✓ Cache scale intervals

```cpp
// In ScaleMapper.h: add optimized path
int mapMidiNoteFast(int midiNote) const;  // Uses cached lookup table

// In ScaleMapper.cpp:
int ScaleMapper::mapMidiNoteFast(int midiNote) const {
    // Lookup-table based: O(1) instead of O(log n)
    static std::array<int, 128> lookupTable;
    if (lookupTableDirty_) {
        for (int i = 0; i < 128; ++i)
            lookupTable[i] = mapMidiNote(i);
        lookupTableDirty_ = false;
    }
    return lookupTable[midiNote];
}
```

#### C. **Benchmarking Suite**
Compare performance before/after optimization.

**File: `tests/benchmark.cpp`** (~150 lines)
```cpp
#include "ScaleMapper.h"
#include "ChordVoicer.h"
#include "Envelope.h"
#include "PerformanceMetrics.h"
#include <benchmark/benchmark.h>

static void BM_ScaleMapperQuantize(benchmark::State& state) {
    MapperSettings ms;
    ms.scale = ScaleType::Harmonic Minor;
    ScaleMapper mapper(ms);
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(mapper.mapMidiNote(61));
    }
}
BENCHMARK(BM_ScaleMapperQuantize);

static void BM_ChordVoicerGenerate(benchmark::State& state) {
    ChordVoicerSettings cvs;
    cvs.quality = VoicingQuality::Seventh;
    ChordVoicer voicer(cvs);
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(voicer.generateChord(60, 2));
    }
}
BENCHMARK(BM_ChordVoicerGenerate);

static void BM_EnvelopeProcess(benchmark::State& state) {
    EnvelopeSettings es;
    Envelope env(es);
    env.noteOn(100, 44100);
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(env.process());
    }
}
BENCHMARK(BM_EnvelopeProcess);
```

### **Implementation Steps**

1. **Create PerformanceMetrics.h/cpp** (100 lines)
   - Implement timing/statistics
   - Build into existing code

2. **Add fast paths** in ScaleMapper/ChordVoicer
   - Add lookup table caching
   - Profile before/after

3. **Create benchmark suite** (150 lines)
   - Link Google Benchmark library
   - Compare original vs. optimized

4. **Update CMakeLists.txt**:
   ```cmake
   find_package(benchmark REQUIRED)
   add_executable(scalechord_benchmark tests/benchmark.cpp)
   target_link_libraries(scalechord_benchmark benchmark::benchmark)
   ```

5. **Generate performance report**:
   ```bash
   ./scalechord_benchmark --benchmark_out=perf_results.json
   ```

### **Expected Results**

| Component | Original | Optimized | Improvement |
|-----------|----------|-----------|-------------|
| ScaleMapper | 0.05 ms | 0.01 ms | **80% faster** |
| ChordVoicer | 0.03 ms | 0.02 ms | **33% faster** |
| Envelope | 0.001 ms | 0.001 ms | — (already optimal) |
| **Total chord generation** | **0.08 ms** | **0.04 ms** | **50% faster** |

### **Estimated Time**: 3–4 hours

---

## 💾 Task 7: Preset System & State Management

### **Objective**
Enable users to save/load plugin configurations, create preset libraries, and persist state across DAW sessions.

### **Deliverables**

#### A. **PresetManager** Module
Save/load plugin settings.

**Header: `include/PresetManager.h`** (~80 lines)
```cpp
#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class PresetManager {
public:
    struct Preset {
        std::string name;
        std::string description;
        std::string category;  // "Jazz", "Blues", "Classical", etc.
        std::string createdDate;
        nlohmann::json settings;  // All parameters as JSON
        int version = 1;
    };

    PresetManager(const std::string& presetDirectory);
    
    // Save/load
    bool savePreset(const Preset& preset);
    bool loadPreset(const std::string& filename, Preset& outPreset);
    
    // List presets
    std::vector<std::string> listPresets(const std::string& category = "") const;
    std::vector<std::string> listCategories() const;
    
    // Export/import
    bool exportPresets(const std::string& filename);
    bool importPresets(const std::string& filename);
    
    // Delete
    bool deletePreset(const std::string& filename);

private:
    std::string presetDirectory_;
};
```

#### B. **SettingsSerializer** Module
Convert plugin state to/from JSON.

**Header: `include/SettingsSerializer.h`** (~60 lines)
```cpp
#pragma once
#include <nlohmann/json.hpp>
#include "ScaleMapper.h"
#include "Envelope.h"

class SettingsSerializer {
public:
    // Serialize all settings to JSON
    static nlohmann::json serializeAll(
        const MapperSettings& mapperSettings,
        const EnvelopeSettings& envelopeSettings,
        // ... other settings
        const std::string& pluginVersion);
    
    // Deserialize from JSON
    static bool deserializeAll(
        const nlohmann::json& json,
        MapperSettings& outMapperSettings,
        EnvelopeSettings& outEnvelopeSettings
        // ... other settings
    );
    
    // Convert individual settings
    static nlohmann::json serializeScale(const MapperSettings& settings);
    static nlohmann::json serializeEnvelope(const EnvelopeSettings& settings);
};
```

#### C. **Preset Library**
Bundled presets for different genres.

**Directory: `presets/`**
```
presets/
├── Jazz/
│   ├── smooth_reharmonization.json
│   ├── tritone_substitution.json
│   └── modal_interchange.json
├── Blues/
│   ├── pentatonic_blues.json
│   ├── shuffle_blues.json
│   └── jazz_blues.json
├── Electronic/
│   ├── ambient_pad.json
│   ├── arpeggio_synth.json
│   └── bass_chords.json
├── Classical/
│   ├── baroque_progression.json
│   ├── romantic_voicing.json
│   └── contemporary_modal.json
└── Songwriting/
    ├── pop_progression.json
    ├── singer_songwriter.json
    └── indie_chord_chart.json
```

**Preset Format** (JSON):
```json
{
  "version": 1,
  "name": "Smooth Reharmonization",
  "category": "Jazz",
  "description": "Voice-led jazz changes with smooth transitions",
  "createdDate": "2025-01-15",
  "settings": {
    "scale": "Dorian",
    "rootNote": 60,
    "voicing": "Seventh",
    "attack": 10,
    "decay": 100,
    "sustain": 0.7,
    "release": 300,
    "humanize": true,
    "arpeggiator": {
      "enabled": false
    },
    "humanizer": {
      "velocityVariation": 0.05,
      "timingVariation": 0.02
    }
  }
}
```

#### D. **DAW Integration**
Use JUCE's ValueTreeState for automation and state management.

**PluginProcessor.cpp Enhancement**:
```cpp
void ScaleChordProcessor::setupValueTree() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "scale", "Scale", scaleNames, 0));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "attack", "Attack",
        juce::NormalisableRange<float>(0.0f, 1000.0f, 1.0f, 0.5f),
        10.0f));
    
    // ... add all parameters ...
    
    valueTree_ = std::make_unique<juce::AudioProcessorValueTreeState>(
        *this, nullptr, "PARAMS", std::move(layout));
}

void ScaleChordProcessor::getStateInformation(juce::MemoryBlock& data) {
    auto state = valueTree_->copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, data);
}

void ScaleChordProcessor::setStateInformation(const void* data, int size) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size));
    if (xml->hasTagName(valueTree_->state.getType()))
        valueTree_->replaceState(juce::ValueTree::fromXml(*xml));
}
```

### **Implementation Steps**

1. **Add nlohmann/json** to CMakeLists.txt:
   ```cmake
   find_package(nlohmann_json 3.2.0 REQUIRED)
   ```

2. **Create SettingsSerializer.h/cpp** (100 lines)
   - Serialize each module to JSON
   - Test with round-trip (save → load → verify)

3. **Create PresetManager.h/cpp** (150 lines)
   - File I/O for presets
   - Directory scanning
   - Import/export functionality

4. **Create preset templates** (10 JSON files)
   - Jazz, Blues, Electronic, Classical, Songwriting
   - Document each preset's intended use

5. **Update JUCE PluginProcessor**
   - Wire in ValueTreeState
   - Save/load state to DAW

6. **Test preset workflow**:
   ```cpp
   PresetManager pm("~/.config/scalechord/presets");
   
   // Save custom preset
   PresetManager::Preset preset;
   preset.name = "My Jazz Voicing";
   preset.category = "Jazz";
   preset.settings = SettingsSerializer::serializeAll(...);
   pm.savePreset(preset);
   
   // List available
   auto categories = pm.listCategories();  // ["Jazz", "Blues", ...]
   auto jazzPresets = pm.listPresets("Jazz");
   
   // Load and apply
   PresetManager::Preset loaded;
   pm.loadPreset("jazz/smooth_reharmonization.json", loaded);
   SettingsSerializer::deserializeAll(loaded.settings, ...);
   ```

### **Expected Directory Structure**

```
~/.config/scalechord/
├── presets/               (factory + user presets)
├── state.json             (last session state)
└── recently_used.json     (MRU list)

/workspaces/production-works/plugin/
├── presets/               (factory presets bundled with plugin)
├── include/
│   ├── PresetManager.h
│   └── SettingsSerializer.h
└── src/
    ├── PresetManager.cpp
    └── SettingsSerializer.cpp
```

### **Estimated Time**: 4–5 hours

---

## 📅 Implementation Schedule

### **Week 1: Chord Intelligence** (Task 3)
- **Days 1–2**: ChordAnalyzer + unit tests
- **Days 3–4**: VoiceLeading + demo
- **Day 5**: JazzReharmonizer + integration

### **Week 2: Performance & Presets** (Tasks 6 & 7)
- **Days 1–2**: PerformanceMetrics + benchmarks (Task 6)
- **Days 3–4**: Optimization + testing
- **Days 5–6**: PresetManager + SettingsSerializer (Task 7)
- **Days 7**: Preset library + DAW integration

### **Week 3: Final Integration & Release**
- **Days 1–2**: JUCE plugin UI refinement
- **Days 3–4**: Full system testing
- **Day 5**: VST3 validation + code signing

---

## 🧪 Testing Strategy

### **Unit Tests**
```bash
# All existing tests + new test suites
ctest -V

# Individual suites
./test_scale               # Core library
./test_chord_intelligence  # Task 3
./test_performance         # Task 6
./test_presets            # Task 7
```

### **Integration Tests**
```bash
# DAW compatibility
# JUCE plugin validation
# Preset save/load round-trip
# Performance profiling
```

### **UAT (User Acceptance Test)**
```
1. Open plugin in Reaper/Studio One
2. Load preset → settings apply
3. Play MIDI → chord appears
4. Save session → preset persists
5. Load session → state restored
```

---

## 🎯 Success Criteria

| Task | Criteria |
|------|----------|
| **Task 3** | ✅ Detect chord quality from any voicing ✅ Smooth voice leading (< 10 semitone avg movement) ✅ Generate jazz substitutions |
| **Task 6** | ✅ 50% performance improvement ✅ Latency < 1ms per chord ✅ CPU < 5% on 44.1kHz |
| **Task 7** | ✅ Save/load presets ✅ 15+ bundled presets ✅ DAW automation support |
| **Overall** | ✅ Plugin passes VST3 validator ✅ Zero audio glitches ✅ Real-time safe ✅ Production-ready |

---

## 🚀 Deployment

### **v0.3 Release Deliverables**
- ✅ VST3 binary (Windows, macOS, Linux)
- ✅ AU binary (macOS only)
- ✅ Preset library (15+ presets)
- ✅ User manual + video tutorial
- ✅ API documentation
- ✅ Example projects (Reaper, Studio One, Ableton)

### **Distribution**
- GitHub releases
- Plugin marketplaces (Blue Cat's, Plugin Boutique)
- Company website

---

## 📚 References

- **Music Theory**: https://en.wikipedia.org/wiki/Harmony
- **Voice Leading**: https://en.wikipedia.org/wiki/Voice_leading
- **JUCE**: https://docs.juce.com
- **VST3**: https://steinbergmedia.github.io/vst3_dev_portal/

---

**Status**: Roadmap complete. Ready for Task 3 implementation.  
**Next Action**: Begin ChordAnalyzer implementation → chord detection → voice leading optimization.
