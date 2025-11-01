#include "ChordAnalyzer.h"
#include <algorithm>
#include <cmath>
#include <cstdio>

ChordAnalyzer::ChordAnalyzer() = default;

std::vector<int> ChordAnalyzer::normalizePitchClasses(
    const std::vector<int>& notes, int baseKey) const
{
    if (notes.empty()) return {};

    std::vector<int> normalized;
    for (int note : notes) {
        int pitchClass = (note - baseKey + 12) % 12;
        normalized.push_back(pitchClass);
    }

    // Sort and deduplicate
    std::sort(normalized.begin(), normalized.end());
    normalized.erase(std::unique(normalized.begin(), normalized.end()), normalized.end());

    return normalized;
}

float ChordAnalyzer::matchPatternScore(
    const std::vector<int>& normalized,
    const ChordPattern& pattern) const
{
    // Must have at least the pattern notes
    if (normalized.size() < pattern.numNotes) {
        return 0.0f;
    }

    // Check if all intervals in pattern match normalized
    for (int i = 0; i < pattern.numNotes; ++i) {
        if (normalized[i] != pattern.intervals[i]) {
            return 0.0f;  // Not a match
        }
    }
    
    // If pattern size matches exactly, return full confidence
    if (normalized.size() == pattern.numNotes) {
        return pattern.confidence;
    }
    
    // If we have extra notes beyond the pattern, reduce confidence
    // unless we're specifically matching extended chords
    float confidence = pattern.confidence * 0.8f;  // Reduce for extensions
    return confidence;
}

ChordInfo ChordAnalyzer::analyzeChord(
    const std::vector<int>& notes, int baseKey) const
{
    ChordInfo result;

    if (notes.empty()) {
        result.quality = ChordQuality::Unknown;
        result.confidence = 0.0f;
        return result;
    }

    // Use lowest note as root
    result.root = *std::min_element(notes.begin(), notes.end());

    // Normalize to pitch classes from root
    std::vector<int> normalized = normalizePitchClasses(notes, result.root);

    // Try to match against chord patterns
    float bestScore = 0.0f;
    int bestPattern = -1;

    for (int i = 0; i < static_cast<int>(CHORD_PATTERNS.size()); ++i) {
        float score = matchPatternScore(normalized, CHORD_PATTERNS[i]);
        if (score > bestScore) {
            bestScore = score;
            bestPattern = i;
        }
    }

    if (bestPattern >= 0) {
        result.quality = CHORD_PATTERNS[bestPattern].quality;
        result.confidence = bestScore;
        result.intervals = normalized;
    } else {
        result.quality = ChordQuality::Unknown;
        result.confidence = 0.0f;
        result.intervals = normalized;
    }

    // Determine function based on root in key
    result.function = detectFunction(result.root % 12, baseKey, true);

    return result;
}

std::vector<ChordInfo> ChordAnalyzer::analyzeChordAmbiguous(
    const std::vector<int>& notes, int baseKey) const
{
    std::vector<ChordInfo> results;

    if (notes.empty()) return results;

    // Try each note as potential root
    for (int rootNote : notes) {
        ChordInfo info = analyzeChord(notes, rootNote);
        if (info.confidence > 0.5f) {  // Only include confident interpretations
            results.push_back(info);
        }
    }

    // Sort by confidence (highest first)
    std::sort(results.begin(), results.end(),
              [](const ChordInfo& a, const ChordInfo& b) {
                  return a.confidence > b.confidence;
              });

    return results;
}

ChordFunction ChordAnalyzer::detectFunction(
    int root, int keyRoot, bool isMajor) const
{
    // Normalize to 0-11 range
    root = ((root % 12) + 12) % 12;
    keyRoot = ((keyRoot % 12) + 12) % 12;

    int degree = (root - keyRoot + 12) % 12;

    // Map degree to function
    if (isMajor) {
        // Major key scale degrees: I=0, ii=2, iii=4, IV=5, V=7, vi=9, vii°=11
        switch (degree) {
            case 0: return ChordFunction::Tonic;        // I
            case 5: return ChordFunction::Subdominant;  // IV
            case 7: return ChordFunction::Dominant;     // V
            case 9: return ChordFunction::Relative;     // vi
            default: return ChordFunction::Extended;
        }
    } else {
        // Natural minor scale degrees: i=0, ii°=2, III=3, iv=5, v=7, VI=8, VII=10
        switch (degree) {
            case 0: return ChordFunction::Tonic;        // i
            case 5: return ChordFunction::Subdominant;  // iv
            case 7: return ChordFunction::Dominant;     // v (often V7)
            case 3: return ChordFunction::Relative;     // III
            default: return ChordFunction::Extended;
        }
    }
}

const char* ChordAnalyzer::qualityToString(ChordQuality q)
{
    switch (q) {
        case ChordQuality::Major: return "Major";
        case ChordQuality::Minor: return "Minor";
        case ChordQuality::Dominant7: return "Dom7";
        case ChordQuality::Major7: return "Maj7";
        case ChordQuality::Minor7: return "Min7";
        case ChordQuality::HalfDim7: return "HalfDim7";
        case ChordQuality::Diminished: return "Diminished";
        case ChordQuality::Augmented: return "Augmented";
        case ChordQuality::Sus2: return "Sus2";
        case ChordQuality::Sus4: return "Sus4";
        case ChordQuality::Maj9: return "Maj9";
        case ChordQuality::Min9: return "Min9";
        case ChordQuality::Dom9: return "Dom9";
        case ChordQuality::Maj11: return "Maj11";
        case ChordQuality::Min11: return "Min11";
        case ChordQuality::Dom11: return "Dom11";
        default: return "Unknown";
    }
}

const char* ChordAnalyzer::functionToString(ChordFunction f)
{
    switch (f) {
        case ChordFunction::Tonic: return "Tonic (I)";
        case ChordFunction::Subdominant: return "Subdominant (IV)";
        case ChordFunction::Dominant: return "Dominant (V)";
        case ChordFunction::Relative: return "Relative (vi)";
        case ChordFunction::Extended: return "Extended";
        default: return "Unknown";
    }
}

const char* ChordAnalyzer::getRomanNumeral(
    int degree, bool isMajor, ChordQuality quality)
{
    static char buffer[32];

    // Roman numerals
    const char* numerals[] = {"I", "ii", "iii", "IV", "V", "vi", "vii°"};
    const char* minor_numerals[] = {"i", "ii°", "III", "iv", "v", "VI", "VII"};

    const char* numeral = isMajor ? numerals[degree % 7] : minor_numerals[degree % 7];

    // Quality suffix
    const char* quality_str = "";
    if (quality == ChordQuality::Minor) quality_str = "m";
    else if (quality == ChordQuality::Dominant7) quality_str = "7";
    else if (quality == ChordQuality::Major7) quality_str = "maj7";
    else if (quality == ChordQuality::Minor7) quality_str = "m7";
    else if (quality == ChordQuality::HalfDim7) quality_str = "m7b5";
    else if (quality == ChordQuality::Diminished) quality_str = "°";

    snprintf(buffer, sizeof(buffer), "%s%s", numeral, quality_str);
    return buffer;
}
