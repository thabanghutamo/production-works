#include "JazzReharmonizer.h"
#include <algorithm>

JazzReharmonizer::JazzReharmonizer() = default;

std::vector<Substitution> JazzReharmonizer::getSubstitutions(
    int scaleDegree, bool majorKey) const
{
    std::vector<Substitution> subs;
    scaleDegree = scaleDegree % 7;

    if (majorKey) {
        // Major key substitutions
        switch (scaleDegree) {
            case 0:  // I - tonic
                subs.push_back({0, ChordQuality::Major, ChordQuality::Major7, "Maj7", 95.0f});
                subs.push_back({0, ChordQuality::Major, ChordQuality::Maj9, "Maj9", 85.0f});
                break;

            case 1:  // ii - supertonic
                subs.push_back({1, ChordQuality::Minor7, ChordQuality::Minor7, "m7", 90.0f});
                subs.push_back({1, ChordQuality::Minor, ChordQuality::Min9, "m9", 80.0f});
                break;

            case 2:  // iii - mediant
                subs.push_back({2, ChordQuality::Minor7, ChordQuality::Minor7, "m7", 85.0f});
                subs.push_back({2, ChordQuality::Minor, ChordQuality::HalfDim7, "m7b5", 70.0f});
                break;

            case 3:  // IV - subdominant
                subs.push_back({3, ChordQuality::Major, ChordQuality::Major7, "Maj7", 92.0f});
                subs.push_back({3, ChordQuality::Major, ChordQuality::Maj9, "Maj9", 82.0f});
                subs.push_back({3, ChordQuality::Major, ChordQuality::Sus4, "Sus4", 65.0f});
                break;

            case 4:  // V - dominant
                subs.push_back({4, ChordQuality::Dominant7, ChordQuality::Dominant7, "7", 95.0f});
                subs.push_back({4, ChordQuality::Dominant7, ChordQuality::Dom9, "9", 88.0f});
                subs.push_back({4, ChordQuality::Dominant7, ChordQuality::Dom11, "11", 75.0f});
                // Tritone substitution (bII7)
                subs.push_back({4, ChordQuality::Dominant7, ChordQuality::Dominant7, "bII7 (tritone)", 85.0f});
                break;

            case 5:  // vi - relative minor
                subs.push_back({5, ChordQuality::Minor7, ChordQuality::Minor7, "m7", 90.0f});
                subs.push_back({5, ChordQuality::Minor, ChordQuality::Min9, "m9", 80.0f});
                break;

            case 6:  // vii° - leading tone
                subs.push_back({6, ChordQuality::HalfDim7, ChordQuality::HalfDim7, "m7b5", 80.0f});
                subs.push_back({6, ChordQuality::Diminished, ChordQuality::Diminished, "°", 75.0f});
                break;
        }
    } else {
        // Natural minor key substitutions
        switch (scaleDegree) {
            case 0:  // i - tonic minor
                subs.push_back({0, ChordQuality::Minor, ChordQuality::Minor7, "m7", 90.0f});
                subs.push_back({0, ChordQuality::Minor, ChordQuality::Min9, "m9", 80.0f});
                break;

            case 1:  // ii° - supertonic diminished
                subs.push_back({1, ChordQuality::HalfDim7, ChordQuality::HalfDim7, "m7b5", 85.0f});
                break;

            case 2:  // III - mediant major (relative major)
                subs.push_back({2, ChordQuality::Major, ChordQuality::Major7, "Maj7", 88.0f});
                subs.push_back({2, ChordQuality::Major, ChordQuality::Maj9, "Maj9", 78.0f});
                break;

            case 3:  // iv - subdominant minor
                subs.push_back({3, ChordQuality::Minor7, ChordQuality::Minor7, "m7", 90.0f});
                subs.push_back({3, ChordQuality::Minor, ChordQuality::Min9, "m9", 80.0f});
                break;

            case 4:  // v - dominant minor (often V7 in jazz)
                subs.push_back({4, ChordQuality::Minor7, ChordQuality::Dominant7, "7", 92.0f});
                subs.push_back({4, ChordQuality::Minor, ChordQuality::Dom9, "9", 82.0f});
                break;

            case 5:  // VI - submediant major
                subs.push_back({5, ChordQuality::Major, ChordQuality::Major7, "Maj7", 88.0f});
                subs.push_back({5, ChordQuality::Major, ChordQuality::Maj9, "Maj9", 78.0f});
                break;

            case 6:  // VII - subtonic major
                subs.push_back({6, ChordQuality::Major, ChordQuality::Dominant7, "7", 85.0f});
                break;
        }
    }

    return subs;
}

int JazzReharmonizer::getTritone(int pitch)
{
    return ((pitch % 12) + 6) % 12;
}

int JazzReharmonizer::normalizeToOctave(int note, int center)
{
    int pitchClass = ((note % 12) + 12) % 12;
    int centerOctave = center / 12;
    return centerOctave * 12 + pitchClass;
}

std::vector<int> JazzReharmonizer::tritoneSubstitution(
    const std::vector<int>& dominantChord) const
{
    if (dominantChord.empty()) return {};

    std::vector<int> result;

    // Get the tritone substitute root (6 semitones away)
    int originalRoot = dominantChord[0];
    int tritoneRoot = ((originalRoot + 6) % 12);

    // Build same chord quality on tritone root
    for (int note : dominantChord) {
        int interval = ((note - originalRoot + 12) % 12);
        int substituteNote = ((tritoneRoot + interval) % 12);

        // Place in similar octave as original
        int octave = note / 12;
        int fullNote = octave * 12 + substituteNote;

        result.push_back(fullNote);
    }

    return result;
}

std::vector<int> JazzReharmonizer::secondaryDominant(
    int targetDegree, bool majorKey) const
{
    // Secondary dominant is V7 of the target chord
    // In C major:
    // - V7/ii (V7 of D) = A7
    // - V7/iii (V7 of E) = B7
    // - V7/IV (V7 of F) = C7
    // - V7/V (V7 of G) = D7
    // - V7/vi (V7 of A) = E7

    targetDegree = targetDegree % 7;

    // Circle of fifths: F C G D A E B
    // For V7 of chord X, we go up a perfect 5th (7 semitones)
    int intervals[] = {5, 0, 7, 2, 9, 4, 11};  // IV, I, V, ii, vi, iii, vii°
    int dominantRoot = intervals[targetDegree];

    // Build dominant 7 chord: root, major 3rd, perfect 5th, minor 7th
    std::vector<int> result = {
        dominantRoot,
        (dominantRoot + 4) % 12,
        (dominantRoot + 7) % 12,
        (dominantRoot + 10) % 12
    };

    // Convert to MIDI notes around middle octave (60 = C4)
    for (int& note : result) {
        note = 60 + note;
    }

    return result;
}

std::vector<int> JazzReharmonizer::parallelVoicing(
    const std::vector<int>& referenceChord) const
{
    // Parallel voicing maintains interval structure on new root
    // This function just returns the reference - actual transposition
    // would be done by caller specifying new root

    if (referenceChord.size() < 2) return referenceChord;

    // Calculate intervals from root
    std::vector<int> intervals;
    intervals.push_back(0);  // Root

    for (int i = 1; i < static_cast<int>(referenceChord.size()); ++i) {
        int interval = referenceChord[i] - referenceChord[0];
        intervals.push_back(interval);
    }

    // To build parallel voicing on new root, caller would do:
    // for (int interval : intervals) newVoicing.push_back(newRoot + interval);

    return referenceChord;  // Return intervals implicitly in structure
}

std::vector<int> JazzReharmonizer::getUpperStructureTriad(
    int rootNote, int upperRoot, bool isMajor) const
{
    std::vector<int> result;

    // Add bass note
    result.push_back(rootNote);

    // Build triad on upper root
    // Major: root, major 3rd, perfect 5th
    // Minor: root, minor 3rd, perfect 5th

    int intervals[] = {0, isMajor ? 4 : 3, 7};

    for (int interval : intervals) {
        int note = (upperRoot + interval) % 12;
        // Place in octave above bass
        int octave = (rootNote / 12) + 1;
        result.push_back(octave * 12 + note);
    }

    return result;
}
