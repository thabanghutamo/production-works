#include "VoiceLeading.h"
#include <algorithm>
#include <cmath>

VoiceLeading::VoiceLeading() = default;

int VoiceLeading::getPitchClass(int midiNote)
{
    return ((midiNote % 12) + 12) % 12;
}

int VoiceLeading::getOctave(int midiNote)
{
    return midiNote / 12 - 1;  // Middle C (60) is octave 4
}

int VoiceLeading::makeMidiNote(int pitchClass, int octave)
{
    return (octave + 1) * 12 + (pitchClass % 12);
}

int VoiceLeading::pitchClassDistance(int from, int to)
{
    from = ((from % 12) + 12) % 12;
    to = ((to % 12) + 12) % 12;

    int up = (to - from + 12) % 12;
    int down = (from - to + 12) % 12;

    return std::min(up, down);
}

int VoiceLeading::findBestOctave(
    int currentNote, int targetPitchClass, int octaveRange)
{
    targetPitchClass = ((targetPitchClass % 12) + 12) % 12;
    int currentPitch = getPitchClass(currentNote);
    int currentOctave = getOctave(currentNote);

    int bestNote = makeMidiNote(targetPitchClass, currentOctave);
    int bestDistance = std::abs(bestNote - currentNote);

    // Try adjacent octaves within range
    for (int octOff = -octaveRange; octOff <= octaveRange; ++octOff) {
        int testOctave = currentOctave + octOff;
        int testNote = makeMidiNote(targetPitchClass, testOctave);

        // Keep within MIDI range
        if (testNote < 0 || testNote > 127) continue;

        int distance = std::abs(testNote - currentNote);
        if (distance < bestDistance) {
            bestDistance = distance;
            bestNote = testNote;
        }
    }

    return bestNote;
}

std::vector<int> VoiceLeading::getPitchClassSet(const std::vector<int>& chord)
{
    std::vector<int> pitchClasses;
    for (int note : chord) {
        pitchClasses.push_back(getPitchClass(note));
    }

    std::sort(pitchClasses.begin(), pitchClasses.end());
    pitchClasses.erase(
        std::unique(pitchClasses.begin(), pitchClasses.end()),
        pitchClasses.end());

    return pitchClasses;
}

std::vector<int> VoiceLeading::optimizeVoicing(
    const std::vector<int>& currentChord,
    const std::vector<int>& targetChord,
    int octaveRange) const
{
    if (targetChord.empty()) return targetChord;

    std::vector<int> result;

    // Get pitch classes
    auto currentPitches = getPitchClassSet(currentChord);
    auto targetPitches = getPitchClassSet(targetChord);

    // Start with lowest note in target chord as reference
    int baseNote = targetChord[0];
    int baseOctave = getOctave(baseNote);

    // Try to retain common tones in same position
    std::vector<bool> used(currentChord.size(), false);

    for (int targetPitch : targetPitches) {
        int bestCurrentNote = -1;
        int bestDistance = 1000;

        // Look for common tone in current chord
        for (int i = 0; i < static_cast<int>(currentChord.size()); ++i) {
            if (used[i]) continue;
            if (getPitchClass(currentChord[i]) == targetPitch) {
                int distance = 0;  // Common tone = no movement
                if (distance < bestDistance) {
                    bestDistance = distance;
                    bestCurrentNote = currentChord[i];
                }
            }
        }

        // If no common tone, find best octave
        if (bestCurrentNote == -1) {
            bestCurrentNote = currentChord.empty() ?
                makeMidiNote(targetPitch, baseOctave) : currentChord.back();
        }

        // Find best octave of target pitch
        int optimizedNote = findBestOctave(bestCurrentNote, targetPitch, octaveRange);
        result.push_back(optimizedNote);
    }

    // Sort result in ascending order
    std::sort(result.begin(), result.end());

    return result;
}

float VoiceLeading::scoreVoiceLeading(
    const std::vector<int>& from,
    const std::vector<int>& to) const
{
    if (from.empty() || to.empty()) return 50.0f;

    // Calculate total semitone distance
    int totalDistance = 0;
    int maxDistance = std::max(
        static_cast<int>(from.size()),
        static_cast<int>(to.size()));

    for (int i = 0; i < maxDistance; ++i) {
        int fromNote = i < static_cast<int>(from.size()) ? from[i] : 60;
        int toNote = i < static_cast<int>(to.size()) ? to[i] : 60;
        totalDistance += std::abs(toNote - fromNote);
    }

    // Count common tones
    auto fromPitches = getPitchClassSet(from);
    auto toPitches = getPitchClassSet(to);

    int commonTones = 0;
    for (int pitch : fromPitches) {
        if (std::find(toPitches.begin(), toPitches.end(), pitch) != toPitches.end()) {
            commonTones++;
        }
    }

    // Calculate smoothness score
    // Penalize distance proportionally to chord size
    float avgDistance = totalDistance / static_cast<float>(maxDistance);
    float distancePenalty = std::min(avgDistance * 2.0f, 80.0f);
    float commonToneBonus = commonTones * 8.0f;

    float score = 100.0f - distancePenalty + commonToneBonus;
    return std::max(0.0f, std::min(100.0f, score));
}

VoiceLeadingResult VoiceLeading::suggestSmoothVoicing(
    const std::vector<int>& currentChord,
    const std::vector<int>& targetChord) const
{
    VoiceLeadingResult result;

    result.previousChord = currentChord;
    result.nextChord = optimizeVoicing(currentChord, targetChord, 2);
    result.smoothnessScore = scoreVoiceLeading(currentChord, result.nextChord);

    // Calculate total distance
    result.totalDistance = 0;
    int maxSize = std::max(currentChord.size(), result.nextChord.size());
    for (int i = 0; i < maxSize; ++i) {
        int from = i < static_cast<int>(currentChord.size()) ? currentChord[i] : 60;
        int to = i < static_cast<int>(result.nextChord.size()) ? result.nextChord[i] : 60;
        result.totalDistance += std::abs(to - from);
    }

    // Check for common tones
    auto fromPitches = getPitchClassSet(currentChord);
    auto toPitches = getPitchClassSet(result.nextChord);

    result.commonToneCount = 0;
    for (int pitch : fromPitches) {
        if (std::find(toPitches.begin(), toPitches.end(), pitch) != toPitches.end()) {
            result.commonToneCount++;
        }
    }

    result.hasCommonTones = result.commonToneCount > 0;

    return result;
}
