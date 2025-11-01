#pragma once

#include <vector>
#include <cmath>
#include "ChordAnalyzer.h"

/**
 * @brief Result of voice leading analysis
 * 
 * Describes the quality and characteristics of a voice leading
 * from one chord to another:
 * - Smoothness score (0-100, higher is smoother)
 * - Total semitone distance moved
 * - Whether common tones exist between chords
 * - Suggested voicing for smoother transition
 */
struct VoiceLeadingResult {
    std::vector<int> previousChord;       // Original first chord
    std::vector<int> nextChord;           // Optimized second chord
    float smoothnessScore;                // 0-100, higher = smoother
    int totalDistance;                    // Total semitones moved
    bool hasCommonTones;                  // True if at least one tone shared
    int commonToneCount;                  // Number of shared pitch classes
};

/**
 * @class VoiceLeading
 * @brief Optimizes chord transitions for smooth voice leading
 * 
 * Voice leading is the art of transitioning smoothly between chords
 * by minimizing the distance notes travel. This class implements:
 * 
 * 1. **Smoothness Optimization**: Revoices chords to minimize total
 *    note movement while preserving chord quality
 * 
 * 2. **Common Tone Retention**: Keeps shared notes between consecutive
 *    chords to increase smoothness and coherence
 * 
 * 3. **Range Management**: Ensures notes stay within practical ranges
 *    and avoids extreme octave jumps
 * 
 * Usage:
 * @code
 * VoiceLeading voiceLeading;
 * 
 * // First chord: C major (C-E-G)
 * std::vector<int> chord1{60, 64, 67};
 * 
 * // Second chord (raw): F major (F-A-C) in high octave
 * std::vector<int> chord2{65, 69, 72};
 * 
 * // Get optimized voicing
 * auto result = voiceLeading.suggestSmoothVoicing(chord1, chord2);
 * 
 * // result.nextChord might be {60, 65, 69} - F major but voiced lower
 * // This minimizes note movement: 0 semitones + 1 semitone + 2 semitones
 * // vs. raw 5 + 5 + 5 semitones
 * @endcode
 */
class VoiceLeading {
public:
    /**
     * @brief Constructor - initializes voice leading optimizer
     */
    VoiceLeading();

    /**
     * @brief Find optimal voicing of a target chord given a source chord
     * 
     * Revoices the target chord to create smooth transition from source.
     * Strategy:
     * 1. Retain common tones in same octave if possible
     * 2. Move other notes by smallest interval
     * 3. Maintain within specified octave range
     * 4. Avoid voice crossings where possible
     * 
     * @param currentChord Source chord notes (MIDI numbers)
     * @param targetChord Target chord notes (MIDI numbers)
     * @param octaveRange Maximum octaves to span (default 2)
     * @return Optimized voicing of targetChord
     * 
     * @example
     * @code
     * std::vector<int> cmaj{60, 64, 67};     // C major
     * std::vector<int> fmaj{65, 69, 72};     // F major (high voicing)
     * 
     * auto optimized = voiceLeading.optimizeVoicing(cmaj, fmaj, 2);
     * // Might return {60, 65, 69} - F major but voiced lower for smoothness
     * @endcode
     */
    std::vector<int> optimizeVoicing(
        const std::vector<int>& currentChord,
        const std::vector<int>& targetChord,
        int octaveRange = 2) const;

    /**
     * @brief Score voice leading smoothness (0-100)
     * 
     * Calculates smoothness based on:
     * - Total semitone distance moved (lower = smoother)
     * - Presence of common tones (higher = smoother)
     * - Voice crossing prevention (avoiding = smoother)
     * 
     * Formula: 100 - (distance_penalty * 0.5) + (common_tone_bonus * 10)
     * Result clamped to 0-100 range.
     * 
     * @param from First chord (MIDI notes)
     * @param to Second chord (MIDI notes)
     * @return Smoothness score (0 = rough, 100 = smooth)
     * 
     * @example
     * @code
     * std::vector<int> smooth1{60, 64, 67};
     * std::vector<int> smooth2{65, 69, 72};  // F major
     * float score1 = voiceLeading.scoreVoiceLeading(smooth1, smooth2);
     * // score1 ≈ 85-95 (good voice leading)
     * 
     * std::vector<int> rough1{60, 64, 67};
     * std::vector<int> rough2{36, 43, 50};   // Way too far
     * float score2 = voiceLeading.scoreVoiceLeading(rough1, rough2);
     * // score2 ≈ 10-20 (poor voice leading)
     * @endcode
     */
    float scoreVoiceLeading(
        const std::vector<int>& from,
        const std::vector<int>& to) const;

    /**
     * @brief Suggest smoother voicing for chord transition
     * 
     * Comprehensive analysis of chord transition providing:
     * - Original and optimized voicings
     * - Smoothness score
     * - Total movement distance
     * - Common tone analysis
     * 
     * This is the high-level interface combining optimization and scoring.
     * 
     * @param currentChord Source chord
     * @param targetChord Target chord
     * @return VoiceLeadingResult with detailed analysis
     * 
     * @example
     * @code
     * auto result = voiceLeading.suggestSmoothVoicing(cmaj, fmaj);
     * 
     * std::cout << "Smoothness: " << result.smoothnessScore << std::endl;
     * std::cout << "Distance: " << result.totalDistance << " semitones" << std::endl;
     * std::cout << "Common tones: " << result.commonToneCount << std::endl;
     * 
     * // Use the optimized voicing in playback
     * for (int note : result.nextChord) {
     *     playNote(note);
     * }
     * @endcode
     */
    VoiceLeadingResult suggestSmoothVoicing(
        const std::vector<int>& currentChord,
        const std::vector<int>& targetChord) const;

    /**
     * @brief Extract pitch class from MIDI note (0-11)
     * 
     * @param midiNote MIDI note number (0-127)
     * @return Pitch class (0-11, where 0=C, 1=C#, etc.)
     */
    static int getPitchClass(int midiNote);

    /**
     * @brief Get octave number of MIDI note
     * 
     * @param midiNote MIDI note number (0-127)
     * @return Octave number (middle C = 4)
     */
    static int getOctave(int midiNote);

    /**
     * @brief Create MIDI note from pitch class and octave
     * 
     * @param pitchClass Pitch class (0-11)
     * @param octave Octave number
     * @return MIDI note number (0-127)
     */
    static int makeMidiNote(int pitchClass, int octave);

private:
    /**
     * @brief Calculate minimum semitone distance between two pitch classes
     * 
     * Takes wrap-around into account (distance from B to C is 1, not 11)
     * 
     * @param from Pitch class from (0-11)
     * @param to Pitch class to (0-11)
     * @return Distance in semitones (always positive)
     */
    static int pitchClassDistance(int from, int to);

    /**
     * @brief Find best octave for target note given current note
     * 
     * Determines which octave of the target pitch class results in
     * smallest movement from current note while staying within range.
     * 
     * @param currentNote Current MIDI note
     * @param targetPitchClass Pitch class to move to (0-11)
     * @param octaveRange Maximum octaves to span
     * @return Best MIDI note in target pitch class
     */
    static int findBestOctave(
        int currentNote, int targetPitchClass, int octaveRange);

    /**
     * @brief Get pitch class set from chord (deduplicated)
     * @param chord Input MIDI notes
     * @return Sorted vector of unique pitch classes (0-11)
     */
    static std::vector<int> getPitchClassSet(const std::vector<int>& chord);
};
