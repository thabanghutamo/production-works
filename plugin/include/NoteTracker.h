// Advanced note tracking system for polyphonic MIDI handling
#pragma once

#include <vector>
#include <map>
#include <cstdint>

namespace scalechord {

struct ActiveNote {
    int inputNote;                    // Original MIDI note from user
    std::vector<int> generatedNotes;  // Chord notes generated for this input
    int velocity;                     // Original velocity
    float envelopePhase;              // Envelope position (0.0-1.0)
    bool holdPedalActive = false;     // Whether note is held by sustain pedal
    int samplePosition = 0;           // When the note started
};

class NoteTracker {
public:
    NoteTracker() = default;
    
    // Track a new note
    void trackNoteOn(int inputNote, const std::vector<int>& generatedChord, 
                     int velocity, int samplePosition = 0);
    
    // Remove a note
    void trackNoteOff(int inputNote, int samplePosition = 0);
    
    // Apply sustain pedal (CC 64)
    void setSustainPedal(bool active) { sustainPedalActive_ = active; }
    
    // Get all currently active generated notes
    std::vector<int> getAllActiveGeneratedNotes() const;
    
    // Get notes that should be released
    std::vector<int> getNoteOffsForInputNote(int inputNote) const;
    
    // Check if input note is currently playing
    bool isNotePlaying(int inputNote) const;
    
    // Get note count
    int getActiveNoteCount() const noexcept { return activeNotes_.size(); }
    
    // Clear all notes
    void reset();
    
    // Update envelope phases (call every processing block)
    void updateEnvelopes(float sampleRate);
    
    // Get all active notes (for UI/monitoring)
    std::vector<ActiveNote> getActiveNotes() const;
    
private:
    std::map<int, ActiveNote> activeNotes_;  // indexed by input MIDI note
    bool sustainPedalActive_ = false;
    std::vector<int> releasedButSustainedNotes_;  // notes waiting for sustain pedal release
};

} // namespace scalechord
