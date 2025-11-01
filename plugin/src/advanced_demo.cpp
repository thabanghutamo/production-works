// Advanced demo showcasing all new features
#include <iostream>
#include <iomanip>
#include "ScaleMapper.h"
#include "ChordVoicer.h"
#include "Envelope.h"
#include "NoteTracker.h"
#include "MIDIEffects.h"

using namespace scalechord;

void demo_advanced_scales() {
    std::cout << "\n=== ADVANCED SCALES DEMO ===\n" << std::endl;
    
    // Test all 15 scales
    const std::vector<std::pair<ScaleType, const char*>> scales = {
        {ScaleType::Ionian, "C Ionian (Major)"},
        {ScaleType::Dorian, "C Dorian"},
        {ScaleType::Phrygian, "C Phrygian"},
        {ScaleType::Lydian, "C Lydian"},
        {ScaleType::Mixolydian, "C Mixolydian"},
        {ScaleType::Aeolian, "C Aeolian (Natural Minor)"},
        {ScaleType::Locrian, "C Locrian"},
        {ScaleType::HarmonicMinor, "C Harmonic Minor"},
        {ScaleType::MelodicMinor, "C Melodic Minor"},
        {ScaleType::MajorPentatonic, "C Major Pentatonic"},
        {ScaleType::MinorPentatonic, "C Minor Pentatonic"},
        {ScaleType::MajorBlues, "C Major Blues"},
        {ScaleType::MinorBlues, "C Minor Blues"},
        {ScaleType::WholeTone, "C Whole Tone"},
        {ScaleType::Diminished, "C Diminished"},
    };
    
    for (const auto& [scaleType, name] : scales) {
        MapperSettings ms;
        ms.rootNote = 0;  // C
        ms.scale = scaleType;
        ScaleMapper mapper(ms);
        
        auto degrees = mapper.getScaleSemitones();
        std::cout << std::setw(25) << name << " -> ";
        for (int deg : degrees) {
            std::cout << deg << " ";
        }
        std::cout << std::endl;
    }
}

void demo_envelope() {
    std::cout << "\n=== ENVELOPE (ADSR) DEMO ===\n" << std::endl;
    
    EnvelopeSettings settings;
    settings.attack = 100.0f;
    settings.decay = 150.0f;
    settings.sustain = 0.7f;
    settings.release = 200.0f;
    settings.velocitySensitivity = 0.5f;
    
    Envelope env(settings);
    env.setSampleRate(44100.0f);
    
    std::cout << "ADSR: A=" << settings.attack << "ms, D=" << settings.decay 
              << "ms, S=" << settings.sustain << ", R=" << settings.release << "ms\n" << std::endl;
    
    // Simulate note with 100ms attack, 150ms decay, 100ms sustain, 200ms release
    env.noteOn(100);  // velocity 100
    std::cout << "Processing 1000 samples (44100Hz = ~22.7ms):\n";
    
    for (int i = 0; i < 1000; ++i) {
        float amp = env.process();
        if (i % 100 == 0) {
            std::cout << "  Sample " << i << ": amplitude = " << std::fixed << std::setprecision(3) << amp << std::endl;
        }
    }
    
    env.noteOff();
    std::cout << "After note-off (release phase):\n";
    for (int i = 0; i < 300; ++i) {
        float amp = env.process();
        if (i % 100 == 0) {
            std::cout << "  Sample " << i << ": amplitude = " << std::fixed << std::setprecision(3) << amp << std::endl;
        }
    }
}

void demo_note_tracker() {
    std::cout << "\n=== NOTE TRACKER (POLYPHONIC) DEMO ===\n" << std::endl;
    
    NoteTracker tracker;
    
    // Simulate user playing C (note 60) with chord C-E-G
    std::cout << "User plays C (MIDI 60) -> generates C-E-G (60, 64, 67)\n";
    tracker.trackNoteOn(60, {60, 64, 67}, 100);
    
    // User plays E (note 64) with chord E-G#-B
    std::cout << "User plays E (MIDI 64) -> generates E-G#-B (64, 68, 71)\n";
    tracker.trackNoteOn(64, {64, 68, 71}, 90);
    
    std::cout << "\nActive notes: " << tracker.getActiveNoteCount() << std::endl;
    std::cout << "All generated notes: ";
    auto allNotes = tracker.getAllActiveGeneratedNotes();
    for (int n : allNotes) std::cout << n << " ";
    std::cout << std::endl;
    
    // User releases C
    std::cout << "\nUser releases C -> note-offs for 60, 64, 67\n";
    auto noteOffs = tracker.getNoteOffsForInputNote(60);
    std::cout << "Note-offs: ";
    for (int n : noteOffs) std::cout << n << " ";
    std::cout << std::endl;
    
    tracker.trackNoteOff(60);
    std::cout << "Active notes now: " << tracker.getActiveNoteCount() << std::endl;
}

void demo_arpeggiator() {
    std::cout << "\n=== ARPEGGIATOR DEMO ===\n" << std::endl;
    
    // Create arpeggiator with up mode
    ArpeggiatorSettings settings;
    settings.mode = ArpeggiatorMode::Up;
    settings.octaveRange = 2;
    settings.tempoHz = 4.0f;  // 4 notes per second
    
    Arpeggiator arp(settings);
    
    // Set chord notes: C-E-G
    std::vector<int> chord = {60, 64, 67};
    arp.setChordNotes(chord);
    
    std::cout << "Arpeggiator: Up mode, 2 octaves, 4 notes/sec\n";
    std::cout << "Chord: C-E-G (60, 64, 67)\n";
    std::cout << "Generated sequence (octave doubled):\n";
    
    for (int i = 0; i < 20; ++i) {
        int note = arp.process(44100.0f, 120.0f);  // 120 BPM
        if (note >= 0) {
            std::cout << "  Step " << i << ": MIDI note " << note << std::endl;
        }
    }
}

void demo_humanizer() {
    std::cout << "\n=== HUMANIZER DEMO ===\n" << std::endl;
    
    HumanizerSettings settings;
    settings.enabled = true;
    settings.velocityVariation = 0.05f;  // ±5%
    settings.timingVariation = 0.02f;    // ±2%
    settings.tuneDeviation = 0.02f;      // ±2 cents
    
    Humanizer humanizer(settings);
    
    std::cout << "Humanizer enabled with ±5% velocity, ±2% timing, ±2 cents\n";
    std::cout << "Original velocity: 100\n";
    std::cout << "Humanized velocities: ";
    for (int i = 0; i < 5; ++i) {
        int humanized = humanizer.humanizeVelocity(100);
        std::cout << humanized << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "ScaleChord - ADVANCED FEATURES DEMO (v0.2)" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    demo_advanced_scales();
    demo_envelope();
    demo_note_tracker();
    demo_arpeggiator();
    demo_humanizer();
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "All demos completed successfully!" << std::endl;
    std::cout << std::string(60, '=') << "\n" << std::endl;
    
    return 0;
}
