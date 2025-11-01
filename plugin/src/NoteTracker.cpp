// Note tracking implementation
#include "NoteTracker.h"
#include <algorithm>

namespace scalechord {

void NoteTracker::trackNoteOn(int inputNote, const std::vector<int>& generatedChord, 
                               int velocity, int samplePosition) {
    ActiveNote note;
    note.inputNote = inputNote;
    note.generatedNotes = generatedChord;
    note.velocity = velocity;
    note.samplePosition = samplePosition;
    note.envelopePhase = 0.0f;
    
    activeNotes_[inputNote] = note;
}

void NoteTracker::trackNoteOff(int inputNote, int samplePosition) {
    auto it = activeNotes_.find(inputNote);
    if (it != activeNotes_.end()) {
        if (sustainPedalActive_) {
            // Keep note alive, mark for sustain
            it->second.holdPedalActive = true;
            releasedButSustainedNotes_.push_back(inputNote);
        } else {
            activeNotes_.erase(it);
        }
    }
}

std::vector<int> NoteTracker::getAllActiveGeneratedNotes() const {
    std::vector<int> result;
    for (const auto& pair : activeNotes_) {
        for (int note : pair.second.generatedNotes) {
            result.push_back(note);
        }
    }
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

std::vector<int> NoteTracker::getNoteOffsForInputNote(int inputNote) const {
    auto it = activeNotes_.find(inputNote);
    if (it != activeNotes_.end()) {
        return it->second.generatedNotes;
    }
    return {};
}

bool NoteTracker::isNotePlaying(int inputNote) const {
    return activeNotes_.find(inputNote) != activeNotes_.end();
}

void NoteTracker::reset() {
    activeNotes_.clear();
    releasedButSustainedNotes_.clear();
}

void NoteTracker::updateEnvelopes(float sampleRate) {
    (void)sampleRate;  // For future envelope update logic
    
    // Remove sustained notes if pedal is released
    if (!sustainPedalActive_) {
        for (int inputNote : releasedButSustainedNotes_) {
            activeNotes_.erase(inputNote);
        }
        releasedButSustainedNotes_.clear();
    }
}

std::vector<ActiveNote> NoteTracker::getActiveNotes() const {
    std::vector<ActiveNote> result;
    for (const auto& pair : activeNotes_) {
        result.push_back(pair.second);
    }
    return result;
}

} // namespace scalechord
