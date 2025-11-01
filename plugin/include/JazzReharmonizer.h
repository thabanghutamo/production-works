#pragma once

#include <vector>
#include <array>
#include "ChordAnalyzer.h"

/**
 * @brief Jazz chord substitution suggestion
 * 
 * Describes an alternative voicing or chord that can replace
 * an original chord in a progression:
 * - Original chord quality
 * - Substitution chord quality
 * - Musicality score (how good it sounds)
 * - Name/description of the substitution type
 */
struct Substitution {
    int degree;                          // Scale degree (0-6, where 0=root)
    ChordQuality originalQuality;        // Original chord type
    ChordQuality substituteQuality;      // Alternative chord type
    const char* name;                    // Name (e.g., "Tritone Sub", "SecDom")
    float musicality;                    // 0-100, quality of substitution
};

/**
 * @class JazzReharmonizer
 * @brief Generates jazz chord substitutions and reharmonization
 * 
 * Jazz musicians use advanced techniques to reharmonize progressions:
 * 
 * 1. **Tritone Substitution** (bII chord): Replace V7 with its tritone
 *    equivalent (same tritone interval but different root)
 *    Example: V7 in C (G7) → bII7 (Db7)
 * 
 * 2. **Secondary Dominants**: Add V7/ii, V7/iii, V7/IV, etc. to create
 *    chromatic bass lines and harmonic excitement
 * 
 * 3. **Parallel Voicing**: Use same voicing shape on different roots
 *    for consistency and smoothness
 * 
 * 4. **Upper Structure Triads**: Add major or minor triads above
 *    altered dominants for complex sonorities
 * 
 * Usage:
 * @code
 * JazzReharmonizer jazz;
 * 
 * // Get tritone substitution for V7 chord
 * std::vector<int> v7{67, 71, 74, 77};    // G7 in C
 * auto tritone_sub = jazz.tritoneSubstitution(v7);
 * // tritone_sub ≈ {61, 65, 68, 71}  // Db7
 * 
 * // Get secondary dominant V7/ii for chord ii
 * auto sec_dom = jazz.secondaryDominant(1, true);  // ii chord in major
 * // Gets V7 of ii (C and G would be F# and C#)
 * 
 * // Get smooth parallel voicing
 * std::vector<int> reference{60, 64, 67};
 * auto parallel = jazz.parallelVoicing(reference);
 * // Same shape but on different root
 * @endcode
 */
class JazzReharmonizer {
public:
    /**
     * @brief Constructor - initializes jazz reharmonizer
     */
    JazzReharmonizer();

    /**
     * @brief Get substitutions available for a scale degree
     * 
     * Returns common jazz substitutions for a given chord scale degree
     * in a major or minor key. Example results:
     * 
     * For degree V (dominant) in major:
     * - Tritone substitution (bII7 instead of V7)
     * - V7#5 (augmented dominant)
     * - V7b9 (dominant with flat 9)
     * 
     * @param scaleDegree Scale degree (0-6: I, ii, iii, IV, V, vi, vii°)
     * @param majorKey True for major key, false for minor
     * @return Vector of Substitution options (highest musicality first)
     * 
     * @example
     * @code
     * auto subs = jazz.getSubstitutions(4, true);  // V in major
     * for (const auto& sub : subs) {
     *     std::cout << sub.name << ": " << sub.musicality << std::endl;
     * }
     * @endcode
     */
    std::vector<Substitution> getSubstitutions(
        int scaleDegree, bool majorKey) const;

    /**
     * @brief Generate tritone substitution for a dominant chord
     * 
     * Tritone substitution (also called "sub V" or "#IV7") replaces
     * a dominant 7th chord with another dominant 7th a tritone away.
     * 
     * This works because:
     * 1. Both chords have the same tritone (3 semitones from root)
     * 2. The tritone resolves to the same tones despite different roots
     * 3. Creates chromatic bass movement (descending half-steps)
     * 
     * Formula:
     * - Original tritone interval: root + 6 semitones (tritone) + 4 (maj 3) + 3 (perf 5)
     * - Substitute tritone interval: (root - 6) + 6 semitones = same tritone
     * - So bII7 has same tritone as V7
     * 
     * @param dominantChord V7 chord to substitute (e.g., G7 in C)
     * @return Revoiced tritone substitution (e.g., Db7)
     * 
     * @example
     * @code
     * // In C major, V7 is G7
     * std::vector<int> g7{67, 71, 74, 77};
     * auto db7 = jazz.tritoneSubstitution(g7);
     * 
     * // Both G7 and Db7 share tritone and resolve similarly:
     * // G7 (G-B-D-F) → C (typically C-E-G)
     * // Db7 (Db-F-Ab-Cb) → C (also leads to C resolution)
     * @endcode
     */
    std::vector<int> tritoneSubstitution(
        const std::vector<int>& dominantChord) const;

    /**
     * @brief Generate secondary dominant for a target chord
     * 
     * Secondary dominants (V/ii, V/iii, V/IV, etc.) are dominant 7th
     * chords that resolve to chords other than the tonic. They create
     * temporary tonal centers and add harmonic interest.
     * 
     * For chord X, V/X is a dominant chord whose tritone/root points to X.
     * 
     * Formula: V/X = V7 chord built on (degree of X - 1 step in circle of 5ths)
     * 
     * @param targetDegree Scale degree of destination chord (0-6)
     * @param majorKey True for major key, false for minor
     * @return Secondary dominant chord notes (V7 of the target)
     * 
     * @example
     * @code
     * // In C major, what's V/ii (V7 of ii chord)?
     * auto v_of_ii = jazz.secondaryDominant(1, true);  // ii is degree 1
     * // Result: C7 (V7 of ii = V7 of D minor = C7)
     * // Because V7 of D = A7, but contextualized as C7 in C major
     * 
     * // Actually V7 of ii = V7 of ii in the key
     * // In C major: ii is Dm, V7 of Dm = A7
     * // But jazz often uses: V7 resolves to ii
     * // So for V/ii chord (Dm), use A7 or altered variations
     * @endcode
     */
    std::vector<int> secondaryDominant(
        int targetDegree, bool majorKey) const;

    /**
     * @brief Generate parallel voicing (same shape, different root)
     * 
     * Parallel voicing (or "quartal/quintal voicing") uses the same
     * intervallic shape built on different root notes. Creates:
     * - Consistent sonority across progression
     * - Smooth voice leading due to parallelism
     * - Modal/contemporary jazz feel
     * 
     * @param referenceChord Voicing shape to use as template
     * @return Same intervallic pattern built on root of original (unmodified)
     * 
     * Note: This is more of a voicing organizer. Typically you would
     * call this to understand the intervals, then build the same shape
     * on a different root:
     * 
     * @example
     * @code
     * // C major triad: C-E-G (0-4-7)
     * std::vector<int> cmaj{60, 64, 67};
     * auto shape = jazz.parallelVoicing(cmaj);
     * 
     * // Build same shape (0-4-7) on F root:
     * // F-A-C (65, 69, 72)
     * 
     * // Result is smooth parallel motion:
     * // C-E-G → F-A-C (all voices move 5 semitones)
     * @endcode
     */
    std::vector<int> parallelVoicing(
        const std::vector<int>& referenceChord) const;

    /**
     * @brief Get upper structure triad for altered dominant
     * 
     * Upper structure triads are major or minor triads played above
     * an altered dominant bass. Creates extended altered dominant colors.
     * 
     * Examples:
     * - G7#5#9 = G-B-D-F + major triad a minor 3rd above = G + (B-D#-F#)
     * - G7b9 = G-B-D-F + major triad a major 3rd above = G + (B-D#-F#)
     * 
     * @param rootNote Root of dominant chord
     * @param upperRoot Root of upper structure triad
     * @param isMajor True for major triad, false for minor
     * @return Combined notes (bass + upper structure)
     * 
     * @example
     * @code
     * // Create G7#5#9 feel with upper structure
     * auto altered = jazz.getUpperStructureTriad(67, 63, true);
     * // 67=G (bass) + D#-F#-A# above = altered dominant sound
     * @endcode
     */
    std::vector<int> getUpperStructureTriad(
        int rootNote, int upperRoot, bool isMajor) const;

private:
    /**
     * @brief Get tritone of a pitch (tritone = 6 semitones)
     * @param pitch MIDI note (0-127)
     * @return MIDI note 6 semitones higher (wrapped within octave)
     */
    static int getTritone(int pitch);

    /**
     * @brief Normalize note to octave around center
     * @param note MIDI note
     * @param center Target octave center
     * @return MIDI note in same pitch class as center octave
     */
    static int normalizeToOctave(int note, int center);

    /**
     * @brief Circle of fifths degrees in major scale
     * Order: F C G D A E B (descending fifths: -1, 0, 1, 2, 3, 4, 5)
     */
    static constexpr std::array<int, 7> CIRCLE_OF_FIFTHS = {3, 0, 4, 1, 5, 2, 6};
};
