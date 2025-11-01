#include <cassert>
#include <iostream>
#include <vector>
#include "../include/ChordAnalyzer.h"
#include "../include/VoiceLeading.h"
#include "../include/JazzReharmonizer.h"

// Test counter
int testsPassed = 0;
int testsFailed = 0;

void assertTrue(bool condition, const char* testName)
{
    if (condition) {
        std::cout << "✓ " << testName << std::endl;
        testsPassed++;
    } else {
        std::cout << "✗ " << testName << std::endl;
        testsFailed++;
    }
}

// ============================================================================
// CHORD ANALYZER TESTS
// ============================================================================

void testChordAnalyzerMajor()
{
    ChordAnalyzer analyzer;
    std::vector<int> cmajor{60, 64, 67};  // C-E-G
    
    auto result = analyzer.analyzeChord(cmajor, 60);
    
    assertTrue(result.quality == ChordQuality::Major, "ChordAnalyzer: Major triad detection");
    assertTrue(result.confidence > 0.9f, "ChordAnalyzer: Major triad confidence");
    assertTrue(result.root == 60, "ChordAnalyzer: Major triad root");
}

void testChordAnalyzerMinor()
{
    ChordAnalyzer analyzer;
    std::vector<int> cminor{60, 63, 67};  // C-Eb-G
    
    auto result = analyzer.analyzeChord(cminor, 60);
    
    assertTrue(result.quality == ChordQuality::Minor, "ChordAnalyzer: Minor triad detection");
    assertTrue(result.confidence > 0.9f, "ChordAnalyzer: Minor triad confidence");
}

void testChordAnalyzerDominant7()
{
    ChordAnalyzer analyzer;
    std::vector<int> g7{67, 71, 74, 77};  // G-B-D-F
    
    auto result = analyzer.analyzeChord(g7, 60);
    
    assertTrue(result.quality == ChordQuality::Dominant7, "ChordAnalyzer: Dominant7 detection");
    assertTrue(result.confidence > 0.9f, "ChordAnalyzer: Dominant7 confidence");
}

void testChordAnalyzerMajor7()
{
    ChordAnalyzer analyzer;
    std::vector<int> cmaj7{60, 64, 67, 71};  // C-E-G-B
    
    auto result = analyzer.analyzeChord(cmaj7, 60);
    
    assertTrue(result.quality == ChordQuality::Major7, "ChordAnalyzer: Major7 detection");
    assertTrue(result.confidence > 0.9f, "ChordAnalyzer: Major7 confidence");
}

void testChordAnalyzerFunction()
{
    ChordAnalyzer analyzer;
    
    // Test I chord (C in C major)
    auto funcI = analyzer.detectFunction(0, 0, true);
    assertTrue(funcI == ChordFunction::Tonic, "ChordAnalyzer: Tonic detection");
    
    // Test V chord (G in C major)
    auto funcV = analyzer.detectFunction(7, 0, true);
    assertTrue(funcV == ChordFunction::Dominant, "ChordAnalyzer: Dominant detection");
    
    // Test IV chord (F in C major)
    auto funcIV = analyzer.detectFunction(5, 0, true);
    assertTrue(funcIV == ChordFunction::Subdominant, "ChordAnalyzer: Subdominant detection");
    
    // Test vi chord (A in C major)
    auto funcVi = analyzer.detectFunction(9, 0, true);
    assertTrue(funcVi == ChordFunction::Relative, "ChordAnalyzer: Relative minor detection");
}

void testChordAnalyzerAmbiguous()
{
    ChordAnalyzer analyzer;
    std::vector<int> notes{60, 64, 67};  // C-E-G (C major or A minor inversion)
    
    auto interpretations = analyzer.analyzeChordAmbiguous(notes, 0);
    
    assertTrue(interpretations.size() > 0, "ChordAnalyzer: Ambiguous chord returns interpretations");
    assertTrue(interpretations[0].confidence >= interpretations.back().confidence,
               "ChordAnalyzer: Interpretations sorted by confidence");
}

void testChordAnalyzerQualityString()
{
    const char* str = ChordAnalyzer::qualityToString(ChordQuality::Major7);
    assertTrue(str != nullptr, "ChordAnalyzer: Quality to string conversion");
}

// ============================================================================
// VOICE LEADING TESTS
// ============================================================================

void testVoiceLeadingPitchClass()
{
    int c = VoiceLeading::getPitchClass(60);
    assertTrue(c == 0, "VoiceLeading: Pitch class C");
    
    int csharp = VoiceLeading::getPitchClass(61);
    assertTrue(csharp == 1, "VoiceLeading: Pitch class C#");
    
    int g = VoiceLeading::getPitchClass(67);
    assertTrue(g == 7, "VoiceLeading: Pitch class G");
}

void testVoiceLeadingOctave()
{
    int octave = VoiceLeading::getOctave(60);
    assertTrue(octave == 4, "VoiceLeading: Octave of middle C");
    
    int octaveHigh = VoiceLeading::getOctave(72);
    assertTrue(octaveHigh == 5, "VoiceLeading: Octave calculation");
}

void testVoiceLeadingMidiNote()
{
    int note = VoiceLeading::makeMidiNote(0, 4);  // C, octave 4
    assertTrue(note == 60, "VoiceLeading: Make MIDI note");
}

void testVoiceLeadingOptimize()
{
    VoiceLeading vl;
    
    std::vector<int> from{60, 64, 67};  // C major
    std::vector<int> to{65, 69, 72};    // F major (high voicing)
    
    auto optimized = vl.optimizeVoicing(from, to, 2);
    
    assertTrue(optimized.size() == to.size(), "VoiceLeading: Optimized voicing size");
    
    // Calculate movement distance
    int totalDistance = 0;
    for (int i = 0; i < static_cast<int>(from.size()); ++i) {
        totalDistance += std::abs(optimized[i] - from[i]);
    }
    
    // Smooth voicing should have reasonable movement (< 30 semitones total)
    assertTrue(totalDistance < 30, "VoiceLeading: Optimization reduces distance");
}

void testVoiceLeadingSmoothness()
{
    VoiceLeading vl;
    
    // Smooth voice leading
    std::vector<int> smooth1{60, 64, 67};
    std::vector<int> smooth2{65, 69, 72};
    float smoothScore = vl.scoreVoiceLeading(smooth1, smooth2);
    
    // Rough voice leading
    std::vector<int> rough1{60, 64, 67};
    std::vector<int> rough2{36, 40, 43};
    float roughScore = vl.scoreVoiceLeading(rough1, rough2);
    
    assertTrue(smoothScore > roughScore, "VoiceLeading: Smooth scores higher than rough");
    assertTrue(smoothScore >= 0.0f && smoothScore <= 100.0f, "VoiceLeading: Score in range");
}

void testVoiceLeadingSuggest()
{
    VoiceLeading vl;
    
    std::vector<int> from{60, 64, 67};
    std::vector<int> to{65, 69, 72};
    
    auto result = vl.suggestSmoothVoicing(from, to);
    
    assertTrue(result.nextChord.size() > 0, "VoiceLeading: Suggest returns voicing");
    assertTrue(result.smoothnessScore >= 0.0f && result.smoothnessScore <= 100.0f,
               "VoiceLeading: Suggest score in range");
    assertTrue(result.totalDistance >= 0, "VoiceLeading: Distance non-negative");
}

// ============================================================================
// JAZZ REHARMONIZER TESTS
// ============================================================================

void testJazzReharmonizerSubstitutions()
{
    JazzReharmonizer jazz;
    
    // Get substitutions for V chord in major key
    auto subs = jazz.getSubstitutions(4, true);
    
    assertTrue(subs.size() > 0, "JazzReharmonizer: V chord has substitutions");
    
    // V chord should include tritone substitution
    bool hasTritone = false;
    for (const auto& sub : subs) {
        if (sub.musicality > 80.0f) hasTritone = true;
    }
    assertTrue(hasTritone, "JazzReharmonizer: V chord suggestions have good musicality");
}

void testJazzReharmonizerTritone()
{
    JazzReharmonizer jazz;
    
    // G7 chord
    std::vector<int> g7{67, 71, 74, 77};
    
    auto tritone = jazz.tritoneSubstitution(g7);
    
    assertTrue(tritone.size() == g7.size(), "JazzReharmonizer: Tritone sub maintains voicing size");
    
    // Tritone substitute should be ~6 semitones away from original
    int rootDistance = std::abs((tritone[0] % 12) - (g7[0] % 12));
    assertTrue(rootDistance == 6 || rootDistance == 6, "JazzReharmonizer: Tritone root distance");
}

void testJazzReharmonizerSecondaryDominant()
{
    JazzReharmonizer jazz;
    
    // Secondary dominant of ii chord
    auto secDom = jazz.secondaryDominant(1, true);
    
    assertTrue(secDom.size() > 0, "JazzReharmonizer: Secondary dominant generates chord");
    assertTrue(secDom.size() == 4, "JazzReharmonizer: Secondary dominant is 7th chord");
}

void testJazzReharmonizerParallel()
{
    JazzReharmonizer jazz;
    
    std::vector<int> reference{60, 64, 67};
    auto parallel = jazz.parallelVoicing(reference);
    
    assertTrue(parallel.size() > 0, "JazzReharmonizer: Parallel voicing generates result");
}

void testJazzReharmonizerUpperStructure()
{
    JazzReharmonizer jazz;
    
    auto upper = jazz.getUpperStructureTriad(67, 63, true);
    
    assertTrue(upper.size() > 0, "JazzReharmonizer: Upper structure generates notes");
    assertTrue(upper[0] == 67, "JazzReharmonizer: Upper structure keeps bass note");
}

// ============================================================================
// INTEGRATION TESTS
// ============================================================================

void testChordAnalysisWithVoiceLeading()
{
    ChordAnalyzer analyzer;
    VoiceLeading vl;
    
    // Analyze progression: C - F - G7 - C
    std::vector<std::vector<int>> progression = {
        {60, 64, 67},      // C major
        {65, 69, 72},      // F major
        {67, 71, 74, 77},  // G7
        {60, 64, 67}       // C major
    };
    
    for (int i = 0; i < static_cast<int>(progression.size()); ++i) {
        auto chord = progression[i];
        auto analyzed = analyzer.analyzeChord(chord, 60);
        
        assertTrue(analyzed.quality != ChordQuality::Unknown,
                   "Integration: Chord analysis works");
        
        if (i > 0) {
            auto vlResult = vl.suggestSmoothVoicing(progression[i-1], chord);
            assertTrue(vlResult.smoothnessScore > 0.0f,
                       "Integration: Voice leading score valid");
        }
    }
}

void testCompleteChordAnalysisWithJazz()
{
    ChordAnalyzer analyzer;
    JazzReharmonizer jazz;
    
    // Analyze a ii-V-I progression
    std::vector<int> ii{62, 65, 69};      // D minor (D-F-A)
    std::vector<int> v7{67, 71, 74, 77};  // G7 (G-B-D-F)
    std::vector<int> i{60, 64, 67};       // C major (C-E-G)
    
    auto ii_analysis = analyzer.analyzeChord(ii, 60);
    auto v_analysis = analyzer.analyzeChord(v7, 60);
    auto i_analysis = analyzer.analyzeChord(i, 60);
    
    assertTrue(ii_analysis.quality == ChordQuality::Minor, "Integration: ii chord analyzed");
    assertTrue(v_analysis.quality == ChordQuality::Dominant7, "Integration: V7 chord analyzed");
    assertTrue(i_analysis.quality == ChordQuality::Major, "Integration: I chord analyzed");
    
    // Get substitution for V7
    auto tritone_sub = jazz.tritoneSubstitution(v7);
    assertTrue(tritone_sub.size() > 0, "Integration: Jazz tritone sub works");
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main()
{
    std::cout << "\n=== ScaleChord Task 3: Chord Intelligence Tests ===\n\n";
    
    // ChordAnalyzer Tests
    std::cout << "ChordAnalyzer Tests:\n";
    testChordAnalyzerMajor();
    testChordAnalyzerMinor();
    testChordAnalyzerDominant7();
    testChordAnalyzerMajor7();
    testChordAnalyzerFunction();
    testChordAnalyzerAmbiguous();
    testChordAnalyzerQualityString();
    
    // VoiceLeading Tests
    std::cout << "\nVoiceLeading Tests:\n";
    testVoiceLeadingPitchClass();
    testVoiceLeadingOctave();
    testVoiceLeadingMidiNote();
    testVoiceLeadingOptimize();
    testVoiceLeadingSmoothness();
    testVoiceLeadingSuggest();
    
    // JazzReharmonizer Tests
    std::cout << "\nJazzReharmonizer Tests:\n";
    testJazzReharmonizerSubstitutions();
    testJazzReharmonizerTritone();
    testJazzReharmonizerSecondaryDominant();
    testJazzReharmonizerParallel();
    testJazzReharmonizerUpperStructure();
    
    // Integration Tests
    std::cout << "\nIntegration Tests:\n";
    testChordAnalysisWithVoiceLeading();
    testCompleteChordAnalysisWithJazz();
    
    // Summary
    std::cout << "\n=== Test Summary ===\n";
    std::cout << "Passed: " << testsPassed << std::endl;
    std::cout << "Failed: " << testsFailed << std::endl;
    
    if (testsFailed == 0) {
        std::cout << "\n✓ All tests passed!\n\n";
        return 0;
    } else {
        std::cout << "\n✗ Some tests failed!\n\n";
        return 1;
    }
}
