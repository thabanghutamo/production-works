#pragma once

#include <vector>
#include <array>
#include <cstring>

/**
 * @brief Chord quality enumeration for analyzer results
 * 
 * Identifies the quality/type of chord from pitch analysis:
 * - Major: root + major 3rd + perfect 5th (0, 4, 7)
 * - Minor: root + minor 3rd + perfect 5th (0, 3, 7)
 * - Dominant7: Major triad + minor 7th (0, 4, 7, 10)
 * - Major7: Major triad + major 7th (0, 4, 7, 11)
 * - And many more extended/altered chord types
 */
enum class ChordQuality {
    Major,           // 0, 4, 7
    Minor,           // 0, 3, 7
    Dominant7,       // 0, 4, 7, 10
    Major7,          // 0, 4, 7, 11
    Minor7,          // 0, 3, 7, 10
    HalfDim7,        // 0, 3, 6, 10 (minor 7b5)
    Diminished,      // 0, 3, 6
    Augmented,       // 0, 4, 8
    Sus2,            // 0, 2, 7
    Sus4,            // 0, 5, 7
    Maj9,            // 0, 4, 7, 11, 2
    Min9,            // 0, 3, 7, 10, 2
    Dom9,            // 0, 4, 7, 10, 2
    Maj11,           // 0, 4, 7, 11, 5
    Min11,           // 0, 3, 7, 10, 5
    Dom11,           // 0, 4, 7, 10, 5
    Unknown          // Unable to match pattern
};

/**
 * @brief Functional harmony role of detected chord
 * 
 * Describes the harmonic function in a key:
 * - Tonic (I): Home chord, stable
 * - Subdominant (IV): Moving away from home
 * - Dominant (V): Tension, pulls toward tonic
 * - Relative (vi): Related to relative major/minor
 * - Extended: Non-functional or chromatic chords
 */
enum class ChordFunction {
    Tonic,           // I - stability
    Subdominant,     // IV - preparation
    Dominant,        // V - tension
    Relative,        // vi - relative major/minor
    Extended         // Other harmonic functions
};

/**
 * @brief Result of chord analysis
 * 
 * Contains complete information about analyzed chord:
 * - Root note (MIDI note number)
 * - Quality (major, minor, 7th, etc.)
 * - Function (I, IV, V, vi, etc.)
 * - Interval pattern (semitones from root)
 * - Confidence score (0.0-1.0)
 */
struct ChordInfo {
    int root;                              // MIDI note of root (0-127)
    ChordQuality quality;                  // Chord type
    ChordFunction function;                // Harmonic function
    std::vector<int> intervals;            // Semitone intervals from root
    float confidence;                      // 0.0 (uncertain) to 1.0 (certain)
    
    ChordInfo() : root(0), quality(ChordQuality::Unknown), 
                  function(ChordFunction::Extended), confidence(0.0f) {}
};

/**
 * @class ChordAnalyzer
 * @brief Analyzes chord quality from collections of MIDI notes
 * 
 * Detects chord type from pitch collection using interval pattern matching.
 * Supports 15+ chord qualities (major, minor, 7th, 9th, 11th, altered, etc.)
 * with confidence scoring and ambiguous chord detection.
 * 
 * Usage:
 * @code
 * ChordAnalyzer analyzer;
 * std::vector<int> notes{60, 64, 67};  // C, E, G
 * ChordInfo result = analyzer.analyzeChord(notes, 60);
 * // result.quality == ChordQuality::Major
 * // result.confidence == 1.0f
 * @endcode
 */
class ChordAnalyzer {
public:
    /**
     * @brief Constructor - initializes chord analyzer
     */
    ChordAnalyzer();

    /**
     * @brief Analyze a single interpretation of a chord
     * 
     * Takes a collection of MIDI notes and finds the best matching
     * chord quality and root position. Returns most likely interpretation.
     * 
     * @param notes MIDI note numbers (0-127)
     * @param baseKey Reference key for context (0-11, where 0=C)
     * @return ChordInfo with quality, root, function, and confidence
     * 
     * @example
     * @code
     * std::vector<int> chord{64, 60, 67};  // E, C, G (C major, 2nd inversion)
     * ChordInfo info = analyzer.analyzeChord(chord, 0);  // Key of C
     * // info.root = 60 (C)
     * // info.quality = ChordQuality::Major
     * // info.confidence = 0.95f
     * @endcode
     */
    ChordInfo analyzeChord(const std::vector<int>& notes, int baseKey) const;

    /**
     * @brief Get all possible chord interpretations (sorted by confidence)
     * 
     * Returns multiple possible chord analyses, useful for ambiguous chords
     * that could be interpreted multiple ways (e.g., C-Eb-G could be
     * C minor or Eb major in first inversion).
     * 
     * @param notes MIDI note numbers
     * @param baseKey Reference key (0-11)
     * @return Vector of ChordInfo sorted by confidence (highest first)
     */
    std::vector<ChordInfo> analyzeChordAmbiguous(
        const std::vector<int>& notes, int baseKey) const;

    /**
     * @brief Detect functional harmony role in a given key
     * 
     * Determines if chord functions as I, IV, V, vi, or other in a key.
     * Uses scale degree analysis to identify harmonic function.
     * 
     * @param root Root note (0-11)
     * @param keyRoot Tonic of key (0-11)
     * @param isMajor True for major key, false for minor
     * @return ChordFunction (Tonic, Subdominant, Dominant, Relative, Extended)
     * 
     * @example
     * @code
     * ChordFunction func = analyzer.detectFunction(
     *     5,     // G (root)
     *     0,     // C (key)
     *     true   // C major
     * );
     * // func == ChordFunction::Dominant (V chord in C major)
     * @endcode
     */
    ChordFunction detectFunction(int root, int keyRoot, bool isMajor) const;

    /**
     * @brief Get Roman numeral representation of chord degree
     * 
     * Converts scale degree to Roman numeral with quality indicators:
     * - "I" for major tonic
     * - "vi" for relative minor
     * - "V7" for dominant seventh
     * 
     * @param degree Scale degree (0-6, where 0=root)
     * @param isMajor True for major key, false for minor
     * @param quality Chord quality for notation
     * @return String representation (e.g., "V7", "vi", "IVmaj7")
     */
    static const char* getRomanNumeral(
        int degree, bool isMajor, ChordQuality quality);

    /**
     * @brief Convert chord quality enum to human-readable string
     * 
     * @param q ChordQuality enum value
     * @return Readable string (e.g., "Major", "Minor7", "Diminished")
     */
    static const char* qualityToString(ChordQuality q);

    /**
     * @brief Convert function enum to human-readable string
     * 
     * @param f ChordFunction enum value
     * @return Readable string (e.g., "Tonic", "Dominant", "Subdominant")
     */
    static const char* functionToString(ChordFunction f);

private:
    /**
     * @brief Internal chord pattern for matching
     */
    struct ChordPattern {
        std::array<int, 6> intervals;  // Semitone intervals from root
        int numNotes;                   // Number of notes in pattern
        ChordQuality quality;           // Quality if matched
        float confidence;               // Matching confidence (0-1)
    };

    /**
     * @brief Array of all chord patterns for matching
     */
    static constexpr std::array<ChordPattern, 16> CHORD_PATTERNS = {{
        // Major triad: root, major 3rd, perfect 5th
        {{0, 4, 7, 0, 0, 0}, 3, ChordQuality::Major, 1.0f},
        
        // Minor triad: root, minor 3rd, perfect 5th
        {{0, 3, 7, 0, 0, 0}, 3, ChordQuality::Minor, 1.0f},
        
        // Dominant 7: major triad + minor 7th
        {{0, 4, 7, 10, 0, 0}, 4, ChordQuality::Dominant7, 0.95f},
        
        // Major 7: major triad + major 7th
        {{0, 4, 7, 11, 0, 0}, 4, ChordQuality::Major7, 0.95f},
        
        // Minor 7: minor triad + minor 7th
        {{0, 3, 7, 10, 0, 0}, 4, ChordQuality::Minor7, 0.95f},
        
        // Half-diminished 7: diminished triad + minor 7th
        {{0, 3, 6, 10, 0, 0}, 4, ChordQuality::HalfDim7, 0.95f},
        
        // Diminished: root, minor 3rd, diminished 5th
        {{0, 3, 6, 0, 0, 0}, 3, ChordQuality::Diminished, 0.9f},
        
        // Augmented: root, major 3rd, augmented 5th
        {{0, 4, 8, 0, 0, 0}, 3, ChordQuality::Augmented, 0.9f},
        
        // Suspended 2: root, major 2nd, perfect 5th
        {{0, 2, 7, 0, 0, 0}, 3, ChordQuality::Sus2, 0.85f},
        
        // Suspended 4: root, perfect 4th, perfect 5th
        {{0, 5, 7, 0, 0, 0}, 3, ChordQuality::Sus4, 0.85f},
        
        // Major 9: major 7 + major 9
        {{0, 4, 7, 11, 2, 0}, 5, ChordQuality::Maj9, 0.85f},
        
        // Minor 9: minor 7 + major 9
        {{0, 3, 7, 10, 2, 0}, 5, ChordQuality::Min9, 0.85f},
        
        // Dominant 9: dominant 7 + major 9
        {{0, 4, 7, 10, 2, 0}, 5, ChordQuality::Dom9, 0.85f},
        
        // Major 11: major 9 + perfect 11
        {{0, 4, 7, 11, 2, 5}, 6, ChordQuality::Maj11, 0.80f},
        
        // Minor 11: minor 9 + perfect 11
        {{0, 3, 7, 10, 2, 5}, 6, ChordQuality::Min11, 0.80f},
        
        // Dominant 11: dominant 9 + perfect 11
        {{0, 4, 7, 10, 2, 5}, 6, ChordQuality::Dom11, 0.80f},
    }};

    /**
     * @brief Normalize notes to pitch classes (0-11 range)
     * @param notes Input MIDI notes
     * @param baseKey Reference key
     * @return Vector of pitch classes (0-11), sorted and deduplicated
     */
    std::vector<int> normalizePitchClasses(
        const std::vector<int>& notes, int baseKey) const;

    /**
     * @brief Check if normalized pitch class pattern matches chord pattern
     * @param normalized Normalized pitch classes from normalizePitchClasses
     * @param pattern Chord pattern to test
     * @return Match score (0.0 = no match, 1.0 = perfect match)
     */
    float matchPatternScore(
        const std::vector<int>& normalized,
        const ChordPattern& pattern) const;
};
